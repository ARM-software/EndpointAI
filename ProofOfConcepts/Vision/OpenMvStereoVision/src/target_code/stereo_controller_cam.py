import image, network, omv, rpc, sensor, struct, time
from pyb import Pin


# variables that can be changed
save_to_SD = True
sensor_format = sensor.RGB565
#sensor_format = sensor.GRAYSCALE



# initialize the spi to communicate with the remote cam
interface = rpc.rpc_spi_master(cs_pin="P3", freq=10000000, clk_polarity=1, clk_phase=0)

# initialize the pin that will control the remote cam
pin4 = Pin('P4', Pin.OUT_PP, Pin.PULL_NONE)
pin4.value(0)

# here we always choose the QVGA format (320x240) inside a VGA image
#if this is changed, the camera have to calibrated again
# also, the logic of mask_height should be checked
img_width = 320
img_height = 240

#additionnal data for the mask height
if sensor_format == sensor.RGB565:
    mask_height = int(img_height /8)
else:
    mask_height = int(img_height / 4)

sensor.reset()

sensor_size = sensor.VGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height))

# wait a bit a get a snapshot
sensor.skip_frames(time = 2000)
sensor.snapshot()

#get the gains and exposure
gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
print("exposure is " + str(exposure_us))
rgb_gain_db = sensor.get_rgb_gain_db()

# send them to the remote cam so that it has exactly the same settings
result = interface.call("sensor_config", struct.pack("<fIfff", gain_db, exposure_us,
                                                     rgb_gain_db[0], rgb_gain_db[1], rgb_gain_db[2]))
if result is not None:
    gain_db, remote_exposure_us, r_gain_db, g_gain_db, b_gain_db = struct.unpack("<fIfff", result)
    if remote_exposure_us != exposure_us:
        print("ret is " + str(exposure_us) + " but we got " + str(remote_exposure_us))
        exit(1)
else:
    print("Could not configure the remote cam!")
    exit(1)

# deactivate the auto settings, use the given gains and exposure and wait for a bit
# this is exactly what the remote is doing on its side
sensor.set_auto_gain(False, gain_db)
sensor.set_auto_exposure(False, exposure_us)
sensor.set_auto_whitebal(False, rgb_gain_db)
sensor.skip_frames(time = 2000)

# save the ref image used for the diff
print("About to save background image...")
ref_img = sensor.alloc_extra_fb(img_width, img_height, sensor_format)
data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
ref_img.replace(sensor.snapshot().remap(data_fb, right=False))
sensor.dealloc_extra_fb()
print("Saved background image - Now frame differencing!")

# now add an additionnal part that will convey the mask info
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height+ mask_height))
time.sleep(500)
clock = time.clock()
idx = 0
while True:
    clock.tick()

    # trigger the snapshot on the remote camera (left side)
    pin4.value(1)
    # get the image and undistort it
    data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
    img = sensor.snapshot().remap(data_fb, right=False)
    # diff it with the ref image that has also been undistorted
    img.difference_special(ref_img, data_fb, 25, 40, 400, 2000)
    sensor.dealloc_extra_fb()
    # alloc a new image after the diff, otherwise there is not enough memory for the diff
    left_image = sensor.alloc_extra_fb(img_width, img_height+ mask_height, sensor_format)
    left_image.replace(img)
    # receive the left image
    right_image = image.Image(img_width, img_height + mask_height, sensor_format, copy_to_fb=True)
    time.sleep(500)
    pin4.value(0)

    result = interface.call("raw_image_read")
    if result is None:
        print("no pic :(")
        #now that we are done, deallocate the right image
        sensor.dealloc_extra_fb()
        continue

    interface.get_bytes(right_image.bytearray(), 5000) # timeout
    print("yeah!")

    # at this point, we have got the right and the left images that have

    if save_to_SD:
        right_image.save("temp/"  + "3ddiff_" + str(idx) + "_right")
        left_image.save("temp/"  + "3ddiff_" + str(idx) + "_left")
        idx += 1

    src = left_image.bytearray()
    dst = right_image.bytearray()
    for i in range(0, int(img_width/2)):
        for j in range(0, int((img_height + mask_height))):
            dst[j*img_width + i] = src[j*img_width +i]


    #now that we are done, deallocate the right image
    sensor.dealloc_extra_fb()


    print(clock.fps())

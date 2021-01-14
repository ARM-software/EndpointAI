import image, network, omv, rpc, sensor, struct, time
from pyb import Pin

interface = rpc.rpc_spi_master(cs_pin="P3", freq=10000000, clk_polarity=1, clk_phase=0)

# initialize the pin that will control the remote cam synchronisation
pin4 = Pin('P4', Pin.OUT_PP, Pin.PULL_NONE)
pin4.value(0)

# here we always choose the QVGA format (320x240) inside a VGA image
img_width = 320
img_height = 240
sensor.reset()
sensor_format = sensor.GRAYSCALE # Grayscale is enough to find a chessboard
sensor_size = sensor.VGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
if img_width != sensor.width() or img_height != sensor.height():
    sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height))
sensor.skip_frames(time = 2000)

# get the current the current exposure and gains and send them to the remote cam so that the
# 2 cams have the same image settings
sensor.snapshot()
gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
print("exposure is " + str(exposure_us))
rgb_gain_db = sensor.get_rgb_gain_db()
sensor.set_auto_gain(False, gain_db)
sensor.set_auto_exposure(False, exposure_us)
sensor.set_auto_whitebal(False, rgb_gain_db)

result = interface.call("sensor_config", struct.pack("<fIfff", gain_db, exposure_us,
                                                     rgb_gain_db[0], rgb_gain_db[1], rgb_gain_db[2]))
if result is not None:
    gain_db, exposure_us, r_gain_db, g_gain_db, b_gain_db = struct.unpack("<fIfff", result)
    print("ret is " + str(exposure_us))
else:
    # apparently something went wrong with the remote cam
    # stopping there in this case
    print("The remote cam did not respond correcly. Stopping here...")
    exit(1)

clock = time.clock()
idx = 0
img1 = sensor.alloc_extra_fb(img_width, img_height, sensor_format)
while True:
    clock.tick()

    #GPIO sync
    pin4.value(1)
    # get a snapshot and put them image directly in a different buffer
    img1.replace(sensor.snapshot())
    # instanciate an image that will receive the remote iamge
    img2 = image.Image(img_width, img_height, sensor_format, copy_to_fb=True)
    # wait a little to be sure the remote cam is ready
    time.sleep(100)
    pin4.value(0)

    # ask for the remote image
    result = interface.call("raw_image_read")
    if result is None:
        print("no pic :(")
        continue

    # get the remote image
    interface.get_bytes(img2.bytearray(), 5000) # timeout
    print("yeah!")

    # Saving the 2 images to the SD card. A temp folder should have been created prior to running this script
    img2.save("temp/"  + "3dtest_" + str(idx) + "_right")
    img1.save("temp/"  + "3dtest_" + str(idx) + "_left")
    idx += 1

    # Showing 1 half of each frame, so that we can know more or less where the chessboard is
    # Note that the display does not always shows it properly
    src = img1.bytearray()
    dst = img2.bytearray()
    for i in range(0, int(img_width)):
        for j in range(0, int(img_height/2)):
            dst[j*img_width + i] = src[j*img_width +i]

    print(clock.fps())

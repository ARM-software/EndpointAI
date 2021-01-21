import image, network, omv, rpc, sensor, struct, time, pyb
from pyb import Pin

# variables that can be changed
save_to_SD = False
show_timings = False

# initialize the spi to communicate with the remote cam
interface = rpc.rpc_spi_master(cs_pin="P3", freq=20000000, clk_polarity=1, clk_phase=0)

# initialize the pin that will control the remote cam synchronisation
pin4 = Pin('P4', Pin.OUT_PP, Pin.PULL_NONE)
pin4.value(0)

# here we always choose the QQVGA format (160x120) inside a QVGA image
#if this is changed, the camera have to calibrated again
# also, the logic of mask_height should be checked
img_width = 160
img_height = 120
sensor_format = sensor.RGB565

#additionnal data for the mask height
mask_height = 0
if sensor_format == sensor.RGB565:
    mask_height = int(img_height /8)
else:
    mask_height = int(img_height / 4)

# looks like we need to have an image with an even height
if mask_height & 1:
    mask_height += 1

sensor.reset()
sensor_size = sensor.QVGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
# Note that there is a -2 compared to QVGA as there looks to be a little misalignement when moving from QVGA to QQQVGA
# As we use the same remap arrays for both definitions, it is important that both are aligned
sensor.set_windowing((int((sensor.width()-img_width)/2)-2,int((sensor.height()-img_height)/2),img_width,img_height))

# wait a bit a get a snapshot
sensor.skip_frames(time = 2000)
sensor.snapshot()

#get the gains and exposure
gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
rgb_gain_db = sensor.get_rgb_gain_db()
print("gain_db is " + str(gain_db))
print("exposure is " + str(exposure_us))
print("rgb_gain_db is " + str(rgb_gain_db))

# Set the gain and exposure as fixed (not concerned about the values)
sensor.set_auto_gain(False, gain_db)
sensor.set_auto_exposure(False, exposure_us)
sensor.set_auto_whitebal(False, rgb_gain_db)

# Setup contrast, brightness and saturation
sensor.set_contrast(0) # range -3 to +3
sensor.set_brightness(0) # range -3 to +3
sensor.set_saturation(0) # range -3 to +3

# Disable night mode (auto frame rate) and black level calibration (BLC)
sensor.__write_reg(0x0E, 0b00000000) # Disable night mode
sensor.__write_reg(0x3E, 0b00000000) # Disable BLC

sensor.__write_reg(0x13, 0b00000000) # disable automated gain

gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
rgb_gain_db = sensor.get_rgb_gain_db()
print("gain_db is " + str(gain_db))
print("exposure is " + str(exposure_us))
print("rgb_gain_db is " + str(rgb_gain_db))

reg_list = [0x00, 0x01, 0x02, 0x03, 0x08, 0x10, 0x2d, 0x2e, 0x2f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x37, 0x38 ]
sensor_config = []
for i in range(0, 16):
        sensor_config += [sensor.__read_reg(reg_list[i])]

result = interface.call("sensor_config", struct.pack("<16I", *sensor_config))

if result is not None:
    remote_gain_db, remote_exposure_us, remote_r_gain_db, remote_g_gain_db, remote_b_gain_db = struct.unpack("<fIfff", result)
    print("sending back: " + str(remote_gain_db) + ", "  + str(remote_exposure_us) + ", "  + str(remote_r_gain_db) + ", "  + str(remote_g_gain_db) + ", "  + str(remote_b_gain_db))
    if remote_exposure_us != exposure_us:
        print("exposure_us is " + str(exposure_us) + " but we got " + str(remote_exposure_us))
        exit(1)
    if remote_gain_db != gain_db:
        print("gain_db is " + str(gain_db) + " but we got " + str(remote_gain_db))
        exit(1)
    if rgb_gain_db[0] != remote_r_gain_db or rgb_gain_db[1] != remote_g_gain_db or rgb_gain_db[2] != remote_b_gain_db:
        print("rgb_gain_db is " + str(rgb_gain_db) + " but we got [" + str(remote_r_gain_db) + "," + str(remote_g_gain_db) + ","+ str(remote_b_gain_db) + "]")
        exit(1)

else:
    print("Could not configure the remote cam!")
    exit(1)

# skip frames as this is exactly what the remote is doing on its side
sensor.skip_frames(time = 2000)

# save the ref image used for the diff
print("About to save background image...")
ref_img = sensor.alloc_extra_fb(img_width, img_height, sensor_format)
data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
ref_img.replace(sensor.snapshot().remap(data_fb, right=False, upside_down=True))
sensor.dealloc_extra_fb()
print("Saved background image - Now frame differencing!")

# now add an additional part that will convey the mask info
sensor.set_windowing((int((sensor.width()-img_width)/2)-2,int((sensor.height()-img_height)/2),img_width,img_height+ mask_height))
time.sleep(500)
clock = time.clock()
idx = 0

data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
right_image = image.Image(img_width, img_height + mask_height, sensor_format, copy_to_fb=False)

while True:
    clock.tick()

    if show_timings:
        timestamps = [0, 0, 0, 0, 0]
        prev_ts = pyb.micros()

    # trigger the snapshot on the remote camera (right side)
    pin4.value(1)
    # get the image and undistort it
    left_image = sensor.snapshot()

    if show_timings:
        curr_ts = pyb.micros()
        timestamps[0] += curr_ts - prev_ts
        prev_ts = curr_ts
    # remove the distortion and apply the stereo calibration
    # also flipping the image (up/down)
    left_image.remap(data_fb, right=False, upside_down=True)

    if show_timings:
        curr_ts = pyb.micros()
        timestamps[1] += curr_ts - prev_ts
        prev_ts = curr_ts
    # diff it with the ref image that has also been undistorted (check image_difference_special.py to understand the meaning of the arguments)
    left_image.difference_special(ref_img, data_fb, 25, 40, 100, 200)

    if show_timings:
        curr_ts = pyb.micros()
        timestamps[2] += curr_ts - prev_ts
        prev_ts = curr_ts
    # wait a bit to make sure the remote cam is ready
    time.sleep(25)
    pin4.value(0)

    # ask for the right iamge
    result = interface.call("raw_image_read")
    if result is None:
        print("no pic :(")
        #now that we are done, deallocate the right image
        sensor.dealloc_extra_fb()
        continue

    # get the right image
    interface.get_bytes(right_image.bytearray(), 5000) # timeout
    #print("yeah!")

    if show_timings:
        curr_ts = pyb.micros()
        timestamps[3] += curr_ts - prev_ts
        prev_ts = curr_ts

    gain_db = sensor.get_gain_db()
    exposure_us = sensor.get_exposure_us()
    rgb_gain_db = sensor.get_rgb_gain_db()
    #print("gain_db is " + str(gain_db))
    #print("exposure is " + str(exposure_us))
    #print("rgb_gain_db is " + str(rgb_gain_db))


    # at this point, we have got the right and the left images
    if save_to_SD:
        right_image.save("temp/"  + "debug_" + str(idx) + "_right")
        left_image.save("temp/"  + "debug_" + str(idx) + "_left")
        idx += 1

    # run the stereo algorithm
    # explanation of the arguments
    # 1: right image
    # 2: colormap minimum value: all the depth values that are less than this value are set to the color
    # corresponding to the minimum depth
    # 3: colormap maximum value: all the depth values (-colormap_min) that are greater or equal to this argument
    # are to the color corresponding to the maximum depth

    # 4: filter width, must be even (warning no check is currently done in the fw)
    # 5: filter height, must be even (warning no check is currently done in the fw)
    # 6: weight type: currently unused
    # 7: colormap type: 0: gray, 1:colored(blue,red,yellow), 2: back or white (debug mode)
    # example with colormap_min 5, colormap_max 100 and colormap_type =1
    # 0----- >5                      55                              105------->max
    # DARK BLUE                      RED                             BRIGHT YELLOW
    # 8: mode_mask
    #    MODE_PIX_THRESHOLD 0x01: if this flag is set, each right pixel candidate is compared to the current left pixel.
    #    if not close enough (in color terms), the computing is skipped and a next candidate is tested (speeds up the process a lot)
    #    LIMIT_DIFF         0x0e: if bits in this mask are set, the max diff between 2 pixels is capped to (mode & LIMIT_DIFF)<<3
    #    Note that in this case, the colors are handled as 6bits each and the computed diff is the sum of the color channels diffs (max 3*63 = 189)
    #    SEGMENT            0x10: if this flag is set, the segmented algo is used else the multiple windows algo
    left_image.stereo(right_image, data_fb, 5, 100, 5, 5, 0, 1, 0x15)

    if show_timings:
        curr_ts = pyb.micros()
        timestamps[4] += curr_ts - prev_ts
        total_time = sum(timestamps)

        timestamp_str = "time " + str(total_time) + ": "
        timestamp_pc = "percent: "
        for ts in timestamps:
            timestamp_str += str(ts) + ", "
            timestamp_pc += str(ts*100.0/total_time) + "%, "
        print(timestamp_str)
        print(timestamp_pc)

    print(clock.fps())

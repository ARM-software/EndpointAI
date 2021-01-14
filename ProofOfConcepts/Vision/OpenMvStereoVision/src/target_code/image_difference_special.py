import sensor, image, pyb, os, time

# variables that can be changed
save_to_SD = False
#only RGB565 and GRAYSCALE are managed by this diff function
sensor_format = sensor.RGB565
#sensor_format = sensor.GRAYSCALE


# here we always choose the QVGA format (320x240) inside a VGA image
#if this is changed, the camera have to calibrated again
# also, the logic of mask_height should be checked
img_width = 320
img_height = 240

#additionnal data for the diff mask
if sensor_format == sensor.RGB565:
    mask_height = int(img_height /8)
else:
    mask_height = int(img_height / 4)

# Initialize the camera sensor
sensor.reset()
sensor_size = sensor.VGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
# use a QVGA image at the center of the VGA image
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height))

# wait a bit a get a snapshot
sensor.skip_frames(time = 2000)
sensor.snapshot()

#get the gains and exposure
gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
print("exposure is " + str(exposure_us))
rgb_gain_db = sensor.get_rgb_gain_db()

# deactivate the auto settings, use the given gains and exposure and wait for a bit
sensor.set_auto_gain(False, gain_db)
sensor.set_auto_exposure(False, exposure_us)
sensor.set_auto_whitebal(False, rgb_gain_db)
sensor.skip_frames(time = 2000)

# allocate extra buffers to store the ref image and to
data_fb  = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
ref_image = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)

# save the ref image used for the diff function
print("About to save background image...")
img = sensor.snapshot()
img.remap(data_fb, right=True)
ref_image.replace(img)
print("Saved background image - Now frame differencing!")

# now add an additional part that will convey the mask info
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height+ mask_height))
time.sleep(500)
clock = time.clock()
count = 0
idx = 0
while True:
    clock.tick() # Track elapsed milliseconds between snapshots().
    img = sensor.snapshot()
    img.remap(data_fb, right=True) # Take a picture and remap the image.

    # Compare 2 different settings by changing them every 100 images
    # The last 3 arguments are currently unused
    # They could be used if the algorithm is improved and needs more inputs

    # the first used 6 arguments are:
    # 1: the reference image used to compare with the current image
    # 2: an extra buffer that should be the same size as the ref image. It stores data used by the diff function
    # this could be allocated internally but this is still useful in a debug phase where you want to compare with a desktop version
    # 3: threshold1: for each pixel, if the pixel difference for at least 1 color channel is greater than the threshold, then the pixel
    # is considered as different from the background and kept in the resulting image.
    # 4: threshold2: for a colored pixel, if the sum of the channel differences is greater than the threshold, then the
    # pixel is considered as different from the background and kept in the resulting image.
    # In the end, the pixels that are not considered as different are set to black.
    # 5: white blob size: all the white blobs in the resulting mask that are smaller than this size will be set to black
    # 6: black blob size: all the black blobs in the resulting mask that are smaller than this size will be set to white
    if count <100:
        print("small blobs 200/2000")
        img.difference_special(ref_image, data_fb, 25, 40, 200, 2000, 0, 0, 0)
    elif count <200:
        print("small blobs 100/200")
        img.difference_special(ref_image, data_fb, 18, 40, 100, 200, 0, 0, 0)

    count += 1
    if count == 200:
        count = 0

    # If needed, Save them to the SD card. You will need to create a "temp" folder at the SD card root
    if save_to_SD:
        extra_fb.save("temp/"  + "diff_" + str(idx) + "_ref")
        img.save("temp/"  + "diff_" + str(idx) + "_actual")
        idx += 1

    print(clock.fps())

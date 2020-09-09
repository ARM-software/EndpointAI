
import sensor, image, tf, time

# cup size that is going to be detected
# 13x19 corresponds to the real life cup ratio when a pixel is 1x1
cup_width = 13
cup_height = 19
cup_column_number = 8
cup_raw_number = 1

# steps used to inspect the whole image
# steps lower than the actual cup size is what we need but the smaller they are, the slower the algo will be and the more
# you will have to manage overlapped detections
wstep = 4
hstep = 4

# sensor settings
sensor.reset()                              # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)         # Set pixel format to RGB565
sensor.set_framesize(sensor.QVGA)           # Set frame size to QVGA (320x240)
sensor.skip_frames(time=1000)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False)

woff = round((320 - cup_column_number*cup_width)/2)
hoff = round((240 - cup_raw_number*cup_height)/2)
im_width = cup_column_number*cup_width
im_height = cup_raw_number*cup_height

sensor.set_windowing((woff, hoff, im_width, im_height))    # Limitate the pic size to 160x120 centered

count  = 0
clock = time.clock()                # Create a clock object to track the FPS.
while(True):
    clock.tick()                    # Update the FPS clock.

    sensor.snapshot().save("example_cups" + str(count) + ".bmp") # or "example.bmp" (or others)

    time.sleep(500)
    count += 1

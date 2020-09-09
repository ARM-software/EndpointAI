
import sensor, image, tf, time

# cup size that is going to be detected
# 13x19 corresponds to the real life cup ratio when a pixel is 1x1
cup_width = 13
cup_height = 19

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
sensor.set_windowing((80, 60, 160, 120))    # Limitate the pic size to 160x120 centered

#thresholds used by find_blobgs
thresholds_cups = [ (20, 100, 30, 128, 0, 128), (20, 80, -100, -20, 10, 80), ] # generic red and green thresholds

# tflite loading part
modelname = "model_cup_quantized_256.tflite"
mytfmodel = tf.load(modelname, True)

clock = time.clock()                # Create a clock object to track the FPS.
while(True):
    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()

    red_cup_rect = []
    green_cup_rect = []
    woff = 0
    # while inside the pic
    while woff + cup_width <= img.width():
        hoff = 0
        # while inside the pic
        while hoff + cup_height <= img.height():
            # invoke the tfmodel and loop on the resulting objets
            # no downscaling applied
            for obj in mytfmodel.classify(img, (woff, hoff, cup_width, cup_height), min_scale=1.0, scale_mul=0.5,
                                        x_overlap=0.0, y_overlap=0.0):
                tf_result = obj.output()
                max_result_value = max(tf_result)
                most_likely_idx = tf_result.index(max_result_value)

                if most_likely_idx == 1:  # red cup
                    if max_result_value > 0.00:  # with a minimum of confidence
                        red_cup_rect += [(obj.rect()[0], obj.rect()[1])]
                elif most_likely_idx == 2:  # green cup
                    if max_result_value > 0.00:  # with a minimum of confidence
                        green_cup_rect += [(obj.rect()[0], obj.rect()[1])]
            hoff += hstep
        woff += wstep

    co_img = "co"
    for rr in red_cup_rect:
        co_img += "_" + str(rr[0]) + "_" + str(rr[1])
    print(co_img)
    img.save("temp/" + co_img)
    ze_img = "ze"
    for rr in green_cup_rect:
        ze_img += "_" + str(rr[0]) + "_" + str(rr[1])
    print(ze_img)
    img.save("temp/" + ze_img)

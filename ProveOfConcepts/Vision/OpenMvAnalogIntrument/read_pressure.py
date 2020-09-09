import sensor, image, time, math

sensor.reset()
sensor.set_pixformat(sensor.GRAYSCALE) # grayscale is faster
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False)
sensor.set_contrast(3)
clock = time.clock()

thresholds = (0, 80)  # looking for dark blobs
sensor.set_windowing((40, 20, 80, 80))  # looking at the center only

smoothing_steps = 32
smothing_index = 0
smoothing_values = [0.0 for x in range(smoothing_steps)]
initial_value = 66.6  # random value
initial_value_set = False
pluspi_value = 7.8
image_count = 0
while(True):
    clock.tick()
    img_orig = sensor.snapshot()

    img = img_orig.copy()

    # hide the center
    img.draw_circle(40, 40, 15, color = 255, thickness = 2, fill = True)

    line_coord = []
    elongation = []

    # look for the 2 sides of the clock hand
    for blob in img.find_blobs([thresholds], pixels_threshold=20, area_threshold=30, merge=True, margin=1):
        elongation += [blob.elongation()]
        line_coord += [(blob.cx(), blob.cy())]
        img.draw_rectangle(blob.rect(), color=127)

    # calculate the value only if the 2 parts of the clock hand have been found
    if len(elongation) == 2:

        img.draw_line(line_coord[0][0],line_coord[0][1], line_coord[1][0],line_coord[1][1],color=255)
        direction = 1
        other = 0
        # check which part is the slimest
        if elongation[0] > elongation[1]:
            direction =0
            other = 1
        # draw the direction, just to check
        img.draw_cross(line_coord[direction][0],line_coord[direction][1], color=127)

        # calculate the angle
        dy = line_coord[direction][0] - line_coord[other][0]
        if dy:
            rot = math.atan(((line_coord[direction][1] - line_coord[other][1]) / dy))
        # smoothing the value
        smoothing_values[smothing_index] = rot
        smothing_index = (smothing_index + 1)%smoothing_steps
        rot_smoothed = sum(smoothing_values)/len(smoothing_values)
        bar = (rot_smoothed - initial_value)* pluspi_value / math.pi

        # print the computed value
        if initial_value_set:
            img_orig.draw_string(0, 0, '{:2.2f}bars'.format(bar), color = 0)
            print(bar)

        # the following is just used at start: setting the initial value
        # in this case we assume the initial value is 0
        if image_count == smoothing_steps and not initial_value_set:
            initial_value = rot_smoothed
            initial_value_set = True
        else:
            image_count += 1
    else:
        print(len(elongation))

    print("FPS %f" % clock.fps())

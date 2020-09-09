import sensor, image, tf, time, pyb

# image size
img_width = 160
img_height = 120
img_pixels = img_width * img_height

# cup size that is going to be detected
# 13x19 corresponds to the real life cup ratio when a pixel is 1x1
cup_width = 13
cup_height = 19

# steps used to inspect the whole image
# steps lower than the actual cup size is what we need but the smaller they are, the slower the algo will be and the more
# you will have to manage overlapped detections
wstep = 2
hstep = 4

# inspect only the blobs
use_find_blob_optimisation = True
# trying to manage the overlapped detections
overlapped_detection_experiments = True
# trying to correct the image colours
image_color_correction = False

def calculate_max_white(npimg, width, height, woff, hoff, max_white):
    white = [0,0,0]
    max_white_value = sum(max_white)
    for i in range(height):
        for j in range(width):
            mypel = npimg.get_pixel(j+woff,i+hoff,True)
            for k in range(3):
                white[k] +=  mypel[k]
    for k in range(3):
        white[k] =  round(white[k]/(width*height))

    white_value = sum(white)
    if white_value > max_white_value:
        for k in range(3):
            max_white[k] = white[k]
        return True
    return False

def get_min_black(npimg, width, height, woff, hoff, min_black):
    cur_black = [255,255,255]
    min_found = False
    x = y = 0
    min_val = sum(min_black)
    for i in range(height):
        for j in range(width):
            mypel = npimg.get_pixel(j+woff, i+hoff, True)
            for k in range(3):
                cur_black[k] = mypel[k]
            val =  sum(cur_black)
            if val < min_val:
                for k in range(3):
                    min_black[k] = cur_black[k]
                min_val = sum(min_black)    # update the min black value
                x = j
                y = i
                min_found = True

    return min_found, x, y

def correct_image(npimg, width, height, woff, hoff, white_rect, white_target, black_rect, black_target):
    cur_white = [0, 0, 0]
    calculate_max_white(npimg, white_rect[2], white_rect[3], white_rect[0], white_rect[1], cur_white)
    cur_black = [255, 255, 255]
    get_min_black(npimg, black_rect[2], black_rect[3], black_rect[0], black_rect[1], cur_black)

    print(cur_white)
    print(cur_black)

    white_ratio = [1.0,1.0,1.0]
    black_ratio = [0,0,0]
    for k in range(3):
        black_ratio[k] = black_target[k] - cur_black[k]
        white_ratio[k] = white_target[k] / (cur_white[k] + black_ratio[k])
        white_ratio[k] = (white_ratio[k]+1.0)/2
        print(white_ratio[k])

    for i in range(height):
        for j in range(width):
            mypel = npimg.get_pixel(j, i, True)
            p = [0,0,0]
            for k in range(3):
                p[k] = mypel[k]
                p[k] = p[k] + black_ratio[k]
                if p[k] < 0:
                    p[k] = 0
                p[k] = round(float(p[k]) * white_ratio[k])
                if p[k] > 255:
                    p[k] = 255
                npimg.set_pixel(j,i,p)


# sensor settings
sensor.reset()                              # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565)         # Set pixel format to RGB565
sensor.set_framesize(sensor.QVGA)           # Set frame size to QVGA (320x240)
sensor.skip_frames(time=1000)
sensor.set_auto_gain(False)
sensor.set_auto_exposure(False)
sensor.set_windowing((round((sensor.width() -img_width)/2), round((sensor.height() - img_height)/2),
                      img_width, img_height))    # Limitate the pic size to 160x120 centered

#thresholds used by find_blobs
thresholds_cups = [ (20, 100, 30, 128, 0, 128), (20, 80, -100, -20, 10, 80), ] # generic red and green thresholds

# tflite loading part
modelname = "model_cup_quantized_256.tflite"
mytfmodel = tf.load(modelname, True)

final_count_red_cup = 0
final_count_green_cup = 0
detection_list = []

avg_timestamps = [0, 0, 0, 0]
avg_count = 0
avg_total_time = 0

if image_color_correction:
    wdiff = [238, 235, 250]
    bdiff = [41, 0, 0]
    black_rect = (58, 53, 10, 10)
    white_rect = (44, 55, 5, 15)

clock = time.clock()                # Create a clock object to track the FPS.
while(True):

    timestamps = [0, 0, 0, 0]
    total_invocation = 0
    prev_ts = pyb.micros()

    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()
    if image_color_correction:
        correct_image(img, 160, 120, 0, 0, white_rect, wdiff, black_rect, bdiff)
        img.draw_rectangle(black_rect, (255, 255, 0))
        img.draw_rectangle(white_rect, (0, 255, 255))

    curr_ts = pyb.micros()
    timestamps[0] += curr_ts - prev_ts
    prev_ts = curr_ts

    detection_list = []
    if use_find_blob_optimisation:
        searched_area = 0
        blobs = img.find_blobs(thresholds_cups, pixels_threshold=70, area_threshold=80,  merge=True, margin=7)
        curr_ts = pyb.micros()
        timestamps[1] += curr_ts - prev_ts
        prev_ts = curr_ts

        for blob in blobs:
            #add margins to the blob rectangle
            rx = blob.rect()[0]
            ry = blob.rect()[1]
            rw = blob.rect()[2]
            rh = blob.rect()[3]

            wmargin = 2
            hmargin = 4
            extra_top_margin = 6

            woff = rx - wmargin
            if woff < 0:
                woff = 0
            hoff = ry - hmargin - extra_top_margin
            if hoff < 0:
                hoff = 0

            rw2 = rw + wmargin * 2
            rh2 = rh + (hmargin * 2 + extra_top_margin)

            img.draw_rectangle((woff,hoff,rw2, rh2),(255,255,255))
            searched_area += rw2 * rh2

            # while inside the pic and inside the blob rectangle (width)
            while woff + cup_width <= img.width() and woff + cup_width <= rx + rw + wmargin:
                hoff = ry - hmargin
                if hoff < 0:
                    hoff = 0

                # while inside the pic and inside the blob rectangle (height)
                while hoff + cup_height <= img.height()  and hoff + cup_height <= ry + rh + hmargin:
                    # invoke the tfmodel and loop on the resulting objets
                    #no downscaling applied
                    total_invocation += 1
                    for obj in mytfmodel.classify(img, (woff, hoff, cup_width, cup_height), min_scale=1.0, scale_mul=0.5, x_overlap=0.0, y_overlap=0.0):
                        tf_result = obj.output()
                        max_result_value = max(tf_result)
                        most_likely_idx = tf_result.index(max_result_value)

                        if most_likely_idx == 1:  # red cup
                            if max_result_value > 0.05:  # with a minimum of confidence
                                #img.draw_rectangle(obj.rect(), (255,0,0))  # draw a red rectangle
                                if overlapped_detection_experiments:
                                    detection_list += [(int(woff + cup_width/2), int(hoff + cup_height/2), 0, max_result_value)]
                        elif most_likely_idx == 2:  # green cup
                            if max_result_value > 0.05:  # with a minimum of confidence
                                #img.draw_rectangle(obj.rect(), (0,255,0))  # draw a green rectangle
                                if overlapped_detection_experiments:
                                    detection_list += [(int(woff + cup_width/2), int(hoff + cup_height/2), 1, max_result_value)]
                    hoff += hstep
                woff += wstep
    else:
        searched_area = img_pixels  # the full image is being searched
        woff = 0
        # while inside the pic
        while woff + cup_width <= img.width():
            hoff = 0
            # while inside the pic
            while hoff + cup_height <= img.height():
                # invoke the tfmodel and loop on the resulting objets
                # no downscaling applied
                total_invocation += 1
                for obj in mytfmodel.classify(img, (woff, hoff, cup_width, cup_height), min_scale=1.0, scale_mul=0.5,
                                            x_overlap=0.0, y_overlap=0.0):
                    tf_result = obj.output()
                    max_result_value = max(tf_result)
                    most_likely_idx = tf_result.index(max_result_value)

                    if most_likely_idx == 1:  # red cup
                        if max_result_value > 0.05:  # with a minimum of confidence
                            #img.draw_rectangle(obj.rect(), (255, 0, 0))  # draw a red rectangle
                            if overlapped_detection_experiments:
                                detection_list += [(int(woff + cup_width/2), int(hoff + cup_height/2), 0, max_result_value)]
                    elif most_likely_idx == 2:  # green cup
                        if max_result_value > 0.05:  # with a minimum of confidence
                            #img.draw_rectangle(obj.rect(), (0, 255, 0)) # draw a green rectangle
                            if overlapped_detection_experiments:
                                detection_list += [(int(woff + cup_width/2), int(hoff + cup_height/2), 1, max_result_value)]
                hoff += hstep
            woff += wstep

    curr_ts = pyb.micros()
    timestamps[2] += curr_ts - prev_ts
    prev_ts = curr_ts

    if overlapped_detection_experiments:
        final_count_red_cup = 0
        final_count_green_cup = 0
        detection_list = sorted(detection_list, key=lambda detection: detection[3], reverse=True)
        while detection_list:

            x = detection_list[0][0]
            y = detection_list[0][1]
            # we got one
            if detection_list[0][2] == 1:
                # mark the center with a black cross for green cup
                img.draw_cross(x,y,(0,0,0),5,1)
                final_count_green_cup += 1
            else:
                # mark the center with a big white cross for red cup
                img.draw_cross(x,y,(255,255,255),7,3)
                final_count_red_cup += 1

            #now remove the blobs in the same physical space (the current blob will be removed with this algo which
            # is also what we want)
            len_detection = len(detection_list)
            for i in range(len_detection):
                rev_i = len_detection - i - 1
                comp_x = detection_list[rev_i][0]
                comp_y = detection_list[rev_i][1]
                if abs(x - comp_x) < round(cup_width *3/4) and abs(y - comp_y) < round(cup_height *3/4):
                    detection_list.pop(rev_i)

    curr_ts = pyb.micros()
    timestamps[3] += curr_ts - prev_ts
    total_time = sum(timestamps)

    # averaging the values we've got from the beginning

    for i in range(len(timestamps)):
        avg_timestamps[i] = (avg_timestamps[i] * avg_count + timestamps[i]) / (avg_count+1)
    avg_total_time = (avg_total_time * avg_count + total_time) / (avg_count + 1)
    avg_count +=1

    timestamp_str = "time " + str(total_time) + ": "
    timestamp_pc = "percent: "
    for ts in timestamps:
        timestamp_str += str(ts) + ", "
        timestamp_pc += str(ts*100.0/total_time) + "%, "
    print(timestamp_str)
    print(timestamp_pc)
    print("area searched: " + str(round(searched_area*100.0/img_pixels)) + "%" + ", invocations: " + str(total_invocation))

    timestamp_str = "avg time " + str(avg_total_time) + ": "
    timestamp_pc = "avg percent: "
    for ts in avg_timestamps:
        timestamp_str += str(ts) + ", "
        timestamp_pc += str(ts*100.0/avg_total_time) + "%, "
    print(timestamp_str)
    print(timestamp_pc)

    # finally display how many cups we found
    print("total_red_cups : " + str(final_count_red_cup))
    print("total_green_cups : " + str(final_count_green_cup))
    img.draw_string(0, 0, str(final_count_red_cup), color = 0, scale=3)
    img.draw_string(0, 24, str(final_count_green_cup), color = 0, scale=3)
    clock_fps = clock.fps()
    print("FPS %f" % clock_fps)

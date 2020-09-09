
import sensor, image, time, math

# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...
thresholds_cups = [ (20, 100, 30, 128, 0, 128), (20, 80, -100, -20, 10, 80), ] # generic red and green thresholds

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_gain(False) # must be turned off for color tracking
sensor.set_auto_whitebal(False) # must be turned off for color tracking
clock = time.clock()

cup_width = 6.5
cup_height = 11.5
ref_ratio = cup_height / cup_width

correct_rotation = True
detect_merged_cup = True

while(True):
    deg = []
    clock.tick()
    img = sensor.snapshot()
    for blob in img.find_blobs(thresholds, pixels_threshold=200, area_threshold=200,  merge=True, margin=7):
        if blob.elongation() > 0.5:
            img.draw_edges(blob.min_corners(), color=(255,0,0))
            img.draw_line(blob.major_axis_line(), color=(0,255,0))
        # These values are stable all the time.
        img.draw_rectangle(blob.rect())
        img.draw_cross(blob.cx(), blob.cy())
        # Note - the blob rotation is unique to 0-180 only.
        img.draw_keypoints([(blob.cx(), blob.cy(), int(math.degrees(blob.rotation())))], size=20)

        if detect_merged_cup:
            r = blob.rect()
            ratio = float(r[3]) /float(r[2])
            cup_number = 1
            while ( True):
                cur_ref_ratio = (ref_ratio/cup_number + ref_ratio/(cup_number+1))/2
                if ratio > cur_ref_ratio:
                    break
                cup_number +=1

            for  sep in range(cup_number-1):
                xx =r[0]+ float(r[2]*(sep+1))/cup_number
                img.draw_line(round(xx),r[1],round(xx), r[1]+r[3],(0,0,255))

        if correct_rotation:
            deg += [blob.rotation_deg() - 90]

    if correct_rotation:
        averaged_deg = sum(deg) / len(deg)
        img = img.rotation_corr(z_rotation = -averaged_deg)

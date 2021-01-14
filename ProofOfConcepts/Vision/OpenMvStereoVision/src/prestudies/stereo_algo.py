import cv2
import numpy as np
import os
import argparse
import glob


def colormap_color(val, colormap_min_val, colormap_max_val):

    mapped_val = 0
    if  val > colormap_min_val:
        mapped_val = val - colormap_min_val

    if mapped_val >= colormap_max_val:
        mapped_val = 47
    else:
        mapped_val = int((mapped_val * 47 + (colormap_max_val/2)) / colormap_max_val)

    if mapped_val < 16 :
        # blue decreasing 31 to 16
        # red increasing 0 to 15
        blue = 31 - mapped_val
        red = mapped_val
        value_656 = ( (red<<11) | (blue))

    elif mapped_val < 32:
        #  val >= 16
        # blue decreasing 15 to 0
        # red increasing 16 to 31
        # green increasing 0 to 15
        blue = 31 - mapped_val
        red = mapped_val
        green = mapped_val -16
        value_656 = ( (red<<11) | (green <<6) | (blue)) # using the green 5 MSbits

    elif mapped_val < 48:
        #  val >= 31
        # blue increasing 0 to 15
        # red at 31
        # green increasing 16 to 31
        blue = mapped_val -31
        green = mapped_val -16
        value_656 = ( (0x1f<<11) | (green <<6) | (blue)) # using the green 5 MSbits

    else:
        value_656 =  0xffff

    return [((value_656 & 0x001f) << 3), ((value_656 & 0x07e0) >> 3), ((value_656 & 0xf800) >> 8)]


def are_pixel_similar(p1, p2, threshold):

    for ch in range(0,3):
        tmp = abs(p1[ch] - p2[ch])
        tmp >>= 2  # to match what is in the fw
        if tmp > threshold:
            return False

    return True

def get_mask_value(val, shift):
    pix = val[2] << 8 | val[1] << 3 | val[0] >> 3
    if pix & (1 << shift):
        return 255
    else:
        return 0


def inside_mask(j, i, mask_img):

    if int(mask_img[j, i]) == 0:
        # print("not in mask")
        return False

    return True


def compare_pixel_simple(p1, p2, max_diff):

    diff = 0
    for ch in range(0,3):
        tmp = int(p1[ch]) - int(p2[ch])
        tmp >>= 2  # to match what is in the fw
        if tmp > 0:
            diff += tmp
        else:
            diff -= tmp

    if diff > max_diff:
        return max_diff
    else:
        return diff


MODE_PIX_THRESHOLD = 0x01
LIMIT_DIFF         = 0x0e
SEGMENT            = 0x10

def get_right_x_v2(j , i, w, h, img_left, mask_left, img_right, mask_right, filter_width, filter_height, weights, mode):

    best_score = 1000000000
    best_rx = -1

    max_diff = 64*3
    if mode & LIMIT_DIFF:
        mode_diff = (mode & LIMIT_DIFF)>>1
        max_diff = mode_diff * 16

    #filter width div 2
    fwd2 = int((filter_width-1)/2)
    #filter height div 2
    fhd2 = int((filter_height-1)/2)

    filter_total_width = 4* fwd2 + 1
    filter_total_height = 4* fhd2 + 1
    filter_total_size = filter_total_width * filter_total_height
    diff_array = [0] * filter_total_size
    # print("filter width %d height %d, total_width %d, total_height %d " % (filter_width, filter_height, filter_total_width, filter_total_height))

    if j < fhd2:
        # bottom filter only
        filter_h_start = 2
        filter_h_end = 3

    elif j < filter_height-1:
        # bottom and center filter only
        filter_h_start = 1
        filter_h_end = 3

    elif j < h -filter_height+1:
        # all the filters on
        filter_h_start = 0
        filter_h_end = 3

    elif j < h -fhd2:
        # bottom filter not possible
        filter_h_start = 0
        filter_h_end = 2
    else:
        # top filter only
        filter_h_start = 0
        filter_h_end = 1

    fh_start_idx = (filter_h_start -2)*fhd2
    fh_end_idx = (filter_h_end -1)*fhd2 +1

    data_offset = w*(j+ fh_start_idx)
    diff_offset = filter_h_start*fhd2*filter_total_width

    '''if mode & SEGMENT:
        if filter_h_start == 0 and filter_h_end == 3:
            if  (i >= filter_width-1) and (i < w -filter_width+1):
                segment_image(img_left, j, i, weights, filter_total_width, filter_total_height, diff_array)
    '''

    # checking only the pixels before i
    for rx in range(0,i): #= 0 rx < i rx++:

        if mode & MODE_PIX_THRESHOLD:
            if not are_pixel_similar(img_left[w*j + i], img_right[w*j +rx], 20):
                continue

        if rx < fwd2: # no need to test i as it is bigger than rx
            # right filter only
            filter_w_start = 2
            filter_w_end = 3

        elif rx < filter_width-1: # no need to test i as it is bigger than rx
            # right and center filter only
            filter_w_start = 1
            filter_w_end = 3

        elif i < w -filter_width+1: # no need to test i as irx is smaller than i
            # all the filters on
            filter_w_start = 0
            filter_w_end = 3

        elif i < w -fwd2:
            # right filter not possible
            filter_w_start = 0
            filter_w_end = 2

        else:
            # left filter only
            filter_w_start = 0
            filter_w_end = 1

        fw_start_idx = (filter_w_start -2)*fwd2
        fw_end_idx = (filter_w_end -1)*fwd2 +1

        ldata_offset = data_offset + i + fw_start_idx
        rdata_offset = data_offset +rx + fw_start_idx
        rxdiff_offset = diff_offset + filter_w_start*fwd2


        for kj in range(fh_start_idx, fh_end_idx):
            for ki in range(fw_start_idx, fw_end_idx):
                try:
                    diff_array[rxdiff_offset + ki + kj*filter_total_width] = compare_pixel_simple(img_left[ldata_offset + ki + kj*w], img_right[rdata_offset + ki + kj*w], max_diff)
                except:
                    print(ki)
                    print(kj)
                    print(rxdiff_offset + ki + kj*filter_total_width)
                    print(ldata_offset + ki + kj*w)
                    print(rdata_offset + ki + kj * w)
                    raise

        # check all the blocks
        if not (mode & SEGMENT):
            for bj in range(filter_h_start, filter_h_end):
                #diff_ptr = diff_array + bj*fhd2*filter_total_width
                block_diff_offset = bj*fhd2*filter_total_width
                for bi in range(filter_w_start, filter_w_end):
                    #dptr = diff_ptr + bi*fwd2
                    bdiff_offset = block_diff_offset + bi*fwd2
                    score = 0
                    for kj in range(0, filter_height):
                        for ki in range(0, filter_width):
                            score += diff_array[bdiff_offset + ki + kj*filter_total_width]
                            #score += *(dptr + ki)
                        if score > best_score:
                            break  # give up, its already not good enough
                        # dptr += filter_total_width

                    if score < best_score:
                        best_score = score
                        best_rx = rx

        #segmented search
        '''if mode & SEGMENT:
            if filter_h_start == 0 && filter_h_end == 3 && filter_w_start == 0 && filter_w_end == 3:
                score = 0
                k = 0
                for kj = 0 kj < filter_total_height kj++:
                    for ki = 0 ki < filter_total_width ki++:
                        score += diff_array[k] * weights[k]
                        k++
                    
                    if score > best_score)
                        break //give up, its already not good enough
                

                if score < best_score:
                    best_score = score
                    best_rx = rx
        '''

    return best_rx, best_score


def stereo_algo(image_path, filter_width=5, filter_height=5, mode=0x05, colormap_min=5, colormap_max=100):

    path_right = image_path + '_right.bmp'
    path_left = image_path + '_left.bmp'

    if not os.path.exists(path_right) or not os.path.exists(path_left):
        print("images not found")
        return

    img_right = cv2.imread(path_right)
    img_left = cv2.imread(path_left)

    cv2.imshow('Right', img_right)
    cv2.imshow('Left', img_left)


    image_shape = img_right.shape[::-1]
    img_width = image_shape[1]
    img_height = image_shape[2]
    print("image shape: " + str(image_shape))


    # recreate the masks on a full image
    mask_right = cv2.imread(path_right, 0)
    mask_left = cv2.imread(path_left, 0)
    if img_width == 320:
        mask_height = 240
        mask_width = 320
    else:
        mask_height = 120
        mask_width = 160

    for j in range(0, mask_height):
        mj = mask_height + int(j/16)
        for i in range(0, mask_width):
            mi = int(i/16) + int(mask_width/16) * (j%16)
            ms = i % 16

            val = img_right[mj, mi]
            mask_right[j, i] = get_mask_value(val, ms)
            val = img_left[mj, mi]
            mask_left[j, i] = get_mask_value(val, ms)

    cv2.imshow('mask_right', mask_right)
    cv2.imshow('mask_left', mask_left)

    right_stereo = cv2.imread(path_right, 0)
    left_stereo = cv2.imread(path_left, 0)
    left_stereo_colormap = cv2.imread(path_left)
    left_stereo_score = cv2.imread(path_left, 0)

    # set image to black
    for j in range(0, mask_height):
        for i in range(0, mask_width):
            right_stereo[j, i] = 0
            left_stereo[j, i] = 0
            left_stereo_colormap[j, i] = [0, 0, 0]
            left_stereo_score[j, i] = 0

    i_start = int(mask_width/32)
    i_start <<= 3

    # weights unused at the moment
    weights_array = []

    linear_img_left  = []
    linear_img_right = []
    for j in range(0, mask_height):
        for i in range(0, mask_width):
            linear_img_left  += [ [ int(img_left[j,i][0]),  int(img_left[j,i][1]),  int(img_left[j,i][2])] ]
            linear_img_right += [ [int(img_right[j,i][0]), int(img_right[j,i][1]), int(img_right[j,i][2])] ]

    pixel_total = 0
    pixel_processed = 0
    pixel_depth_found = 0
    score_array = []
    max_pic_score = 0
    for j in range(0, mask_height):
        for i in range(i_start, mask_width):

            pixel_total += 1
            if not inside_mask(j, i, mask_left):
                score_array += [0]
                continue

            pixel_processed += 1

            right_x, score = get_right_x_v2(j, i, mask_width, mask_height, linear_img_left, mask_left, linear_img_right, mask_right, filter_width, filter_height, weights_array, mode)
            if right_x is not None:
                this_shift = i - right_x
                left_stereo[j, i] = this_shift
                left_stereo_colormap[j, i] = colormap_color(this_shift, colormap_min, colormap_max)
                pixel_depth_found += 1
                score_array += [score]
                if score > max_pic_score:
                    max_pic_score = score
            else:
                score_array += [0]
                pass
                #print("could not get a score")

    #remove the meta info
    print(img_height)
    for j in range(mask_height, img_height):
        for i in range(0, mask_width):
            right_stereo[j, i] = 0
            left_stereo[j, i] = 0


    count = 0
    for j in range(0, mask_height):
        for i in range(i_start, mask_width):
            left_stereo_score[j, i] = round(score_array[count]*255/max_pic_score)
            count +=1


    # normalize it to make it as visible as possible
    #cv2.normalize(src=right_stereo, dst=right_stereo, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX)
    cv2.normalize(src=left_stereo, dst=left_stereo, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX)
    #cv2.imshow('right_stereo', right_stereo)
    cv2.imshow('left_stereo', left_stereo)
    cv2.imshow('left_stereo_colormap', left_stereo_colormap)
    cv2.imshow('left_stereo_score', left_stereo_score)
    print("max pic score %d" % (max_pic_score))
    print("done! Total %d Processed %d, found %d" %(pixel_total, pixel_processed, pixel_depth_found))
    cv2.waitKey(10000000)

def main(args=None):

    p = argparse.ArgumentParser()

    p.add_argument('-i', '--image', help='path to the image', default='../../images/stereo_debug/debug_30')
    pargs = p.parse_args(args)

    stereo_algo(pargs.image)

if __name__ == '__main__':
    main()
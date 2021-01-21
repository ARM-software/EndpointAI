import cv2
import numpy as np
import os
import argparse

def test(path='../../images/diff/', basename="diff_", count_max=120 , rgb_threshold = [18, 18, 18], white_blob_size=2000, black_blob_size=2000 ):

    ref_image = None
    count = 0
    while True:
        img_path = path + basename + str(count) + '.bmp'
        count += 1
        if count == count_max:
            count = 0

        if not os.path.exists(img_path):
            # print( img_path + ' not found')
            continue

        # the first image is taken as the reference image
        if ref_image is None:
            print('choosing the ref image: ' + img_path)
            ref_image = cv2.imread(img_path)
            print(ref_image.shape)
            if len(ref_image.shape) == 2:
                print("grayscale image are not supported")
                exit(1)
            else:
                num_ch = ref_image.shape[2]
                img_width = ref_image.shape[1]
                img_height = ref_image.shape[0]
                print("number of color channels: " + str(num_ch))
                if num_ch != 3:
                    print("should be 3!")
                    exit(1)
            # back to the while loop to read another image
            continue

        # read the new image and display it along with ref
        image = cv2.imread(img_path)
        cv2.imshow('ref_image', ref_image)
        cv2.imshow('image', image)

        # create a new image that will show the diff mask
        diff_image = cv2.imread(img_path)

        # First part of the algo
        # comparing each pixel of the current pic with the corresponding one on the ref pic
        # if they are similar, set the pixel in resulting diff mask to [0,0,0] else [255,0,0]
        for x in range(0, img_width):
            for y in range(0, img_height):
                val = [0, 0, 0]
                color_val_sum = 0
                for ch in range(0, 3):
                    color_val = abs(int(image[y,x][ch]) - int(ref_image[y,x][ch]))
                    color_val_sum += color_val
                    if color_val > rgb_threshold[ch]:
                        val = [255, 0, 0]
                        break
                if color_val_sum > 40:
                    val = [255, 0, 0]
                diff_image[y, x] = val

        # Second part of the algorithm
        # removing small blobs (white or black)
        if True:
            for x in range(0, img_width):
                for y in range(0, img_height):

                    # we use the second color channel of each pixel in the diff image, to know
                    # if is has already been checked or not
                    if diff_image[y, x][1] == 0:  # not checked yet
                        # we found a pixel that has not been checked yet
                        # storing its value (0 or 255)
                        # and adding its coordinates to the pix_list array that will group all the pixels within the same blob
                        pix_val =  int(diff_image[y, x][0])
                        pix_list = [[x,y]]
                        diff_image[y, x][1] = 255 # set this pixel as checked now
                        # function that is going to check the neighbours of the current pixel
                        # if they have the same value, they are added to the pix_list and their neighbours are then being checked (and so on)
                        # if they don't have the same value, nothing is done
                        find_close_pixels(diff_image, [x,y], pix_list, pix_val)

                        if pix_val == 255:  #white
                            # if the white blob is too small, remove it (set all its pixels to black)
                            if len(pix_list) < white_blob_size:
                                for pix in pix_list:
                                    diff_image[pix[1], pix[0]][0] = 0
                        else:
                            # if the black blob is too small, remove it (set all its pixels to white)
                            if len(pix_list) < black_blob_size:
                                for pix in pix_list:
                                    diff_image[pix[1], pix[0]][0] = 255


        cv2.imshow('diff_mask', diff_image)

        # create a new image that will show the resulting image
        resulting_image = cv2.imread(img_path)
        for x in range(0, img_width):
            for y in range(0, img_height):
                # for each pixel, depending on the mask, we set the pixel value to black or to the original value
                if diff_image[y, x][0] == 0:
                    resulting_image[y,x] = [0, 0, 0]
                else:
                    resulting_image[y, x] = image[y,x]

        cv2.imshow('resulting_image', resulting_image)
        cv2.waitKey(1000)


def find_close_pixels(diff_image, coord, pix_list, pix_val):

    check_neighbour_list = [coord]
    # look into the neighbour list
    while check_neighbour_list:
        to_check = check_neighbour_list.pop(-1)
        x = to_check[0]
        y = to_check[1]
        # check if it is inside the pixel
        if x > 1 and x < 320-1 and y > 1 and y < 240 -1:
            for i in range(x - 1, x + 2):
                for j in range(y - 1, y + 2):
                    # if this the same value as the original one, set it check and add it to the list
                    if diff_image[j, i][1] == 0 and  diff_image[j, i][0] == pix_val:
                        diff_image[j, i][1] = 255
                        pix_list += [[i,j]]
                        check_neighbour_list += [[i,j]]
        else:
            # be more careful about the image limits here but do exactly the same as before
            for i in range(x - 1, x + 2):
                for j in range(y - 1, y + 2):
                    if i < 0 or i >= 320 or j <0 or j >=240:
                        continue
                    if diff_image[j, i][1] == 0 and  diff_image[j, i][0] == pix_val:
                        diff_image[j, i][1] = 255
                        pix_list += [[i,j]]
                        check_neighbour_list += [[i,j]]

def main(args=None):

    p = argparse.ArgumentParser()
    p.add_argument('-p', '--path', help='path to the images to diff, the first one is the ref', default='../../images/diff/')
    p.add_argument('-bn', '--basename', help='basename of the images, they all should be in the format "basename" + index + ".bmp"',
                   default='diff_')
    p.add_argument('-cm', '--count_max', help='image max count. Uses images with index 0 to count_max', default=120)
    p.add_argument('-wbs', '--white_blob_size', help='minimum size for a white blob not to be remove', default=2000)
    p.add_argument('-bbs', '--black_blob_size', help='minimum size for a black blob not to be remove', default=2000)
    p.add_argument('-rgbt', '--rgb_threshold', help='threshold to determine if a pixel is comparable to the ref pixel or not', default=[18, 18, 18])

    pargs = p.parse_args(args)

    test(pargs.path, pargs.basename, pargs.count_max, pargs.rgb_threshold,  int(pargs.white_blob_size), int(pargs.black_blob_size))

if __name__ == '__main__':
    main()



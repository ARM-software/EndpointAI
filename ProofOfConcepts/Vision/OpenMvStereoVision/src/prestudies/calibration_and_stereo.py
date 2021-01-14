import cv2
import numpy as np
import os
import argparse
import glob


FIXED_SHIFT = 4
FIXED_MULT = 1 << FIXED_SHIFT
FIXED_MASK = FIXED_MULT - 1
FIXED_INTERPOL_SHIFT = FIXED_SHIFT * 2
FIXED_INTERPOL_ROUND = 1 << (FIXED_INTERPOL_SHIFT - 1)

# Use OpenCV to get the needed map correction
def calibrate(path, chess_width, chess_height, print_arrays=False, draw_chess=False):

    # termination criteria
    criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)
    stereo_criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER, 30, 0.001)

    # prepare object points
    objp = np.zeros((chess_width * chess_height, 3), np.float32)
    objp[:, :2] = np.mgrid[0:chess_width, 0:chess_height].T.reshape(-1, 2)

    # Arrays to store object points and image points from all the images.
    objpoints = []  # 3d point in real world space
    imgpoints_right = []  # 2d points in image plane.
    imgpoints_left = []  # 2d points in image plane.

    print("using chessboard path: " + path)
    pics_path = glob.glob(os.path.join(path, "*_left.bmp"))
    chess_count = 0
    image_shape = img_width = img_height = None
    # look for all the images in the selected path
    # there must be images ending with _left.bmp an the corresponding one ending with _right.bmp
    for path_left in pics_path:
        path_right = path_left.split("_left")[0] + "_right.bmp"
        if not os.path.exists(path_right):
            continue

        # looking for the chessboard
        img_right = cv2.imread(path_right, 0)
        img_left = cv2.imread(path_left, 0)
        ret, corners_right = cv2.findChessboardCorners(img_right, (chess_width, chess_height), None)
        if not ret:
            print('could not find the chessboard on the image ' + path_right)
            continue

        ret, corners_left = cv2.findChessboardCorners(img_left, (chess_width, chess_height), None)
        if not ret:
            print('could not find the chessboard on the image ' + path_left)
            continue

        if draw_chess:
            # Draw and display the corners
            cv2.drawChessboardCorners(img_right, (chess_width, chess_height), corners_right, ret)
            cv2.imshow('right stereo pic', img_right)
            cv2.waitKey(500)

        objpoints.append(objp)

        cv2.cornerSubPix(img_right, corners_right, (11, 11), (-1, -1), criteria)
        imgpoints_right.append(corners_right)

        cv2.cornerSubPix(img_left, corners_left, (11, 11), (-1, -1), criteria)
        imgpoints_left.append(corners_left)

        chess_count += 1
        if image_shape is None:
            image_shape = img_right.shape[::-1]
            print(image_shape)
            img_width = image_shape[0]
            img_height = image_shape[1]
        else:
            shape = img_right.shape[::-1]
            if img_width != shape[0] or img_height != shape[1]:
                print("size mismatch!")
                return None

    if image_shape is None:
        print("No Calibration pictures were found!!")
        return None

    print("based on " + str(chess_count) + " chessboards")
    print('=====================calibrate results right')
    ret, mtx_right, dist_right, rvecs_right, tvecs_right = cv2.calibrateCamera(objpoints, imgpoints_right,
                                                                               image_shape, None, None)

    print('=====================calibrate results left')
    ret, mtx_left, dist_left, rvecs_left, tvecs_left = cv2.calibrateCamera(objpoints, imgpoints_left,
                                                                           image_shape, None, None)

    print('=====================calibrate results stereo')
    flags = 0
    flags |= cv2.CALIB_FIX_INTRINSIC
    ret, stereo_matrix_left, stereo_dist_left, stereo_matrix_right, stereo_dist_right, r, t, e, f = cv2.stereoCalibrate(
        objpoints, imgpoints_left, imgpoints_right, mtx_left, dist_left,
        mtx_right, dist_right, image_shape, stereo_criteria, flags)

    print('matrix left')
    print(stereo_matrix_left)
    print('disto left')
    print(stereo_dist_left)
    print('matrix right')
    print(stereo_matrix_right)
    print('disto right')
    print(stereo_dist_right)
    print('R')
    print(r)
    print('T')
    print(t)

    print('=====================rectify stereo')
    rectify_scale = 0  # if 0 image croped, if 1 image nor croped
    rectify_left, rectify_right, proj_left, proj_right, q, roi_left, roi_right = cv2.stereoRectify(
        stereo_matrix_left, stereo_dist_left, stereo_matrix_right, stereo_dist_right,
        image_shape, r, t, rectify_scale, (0, 0))

    print('rectify left')
    print(rectify_left)
    print('rectify right')
    print(rectify_right)
    print('Proj left')
    print(proj_left)
    print('Proj right')
    print(proj_right)
    print('Q')
    print(q)
    print('ROI left')
    print(roi_left)
    print('ROI right')
    print(roi_right)

    print('=====================undistort')
    map1_left, map2_left = cv2.initUndistortRectifyMap(stereo_matrix_left, stereo_dist_left, rectify_left,
                                                       proj_left, image_shape, cv2.CV_32FC2)

    map1_right, map2_right = cv2.initUndistortRectifyMap(stereo_matrix_right, stereo_dist_right, rectify_right,
                                                         proj_right,  image_shape, cv2.CV_32FC2)

    const_str = ''
    count = 0
    pymap_left = []
    # if wanted, print the arrays that are needed by the openMV FW
    if print_arrays:
        print('static const short left_map[' + str(img_height) + '][' + str(img_width) + '][2] = {\n')

    # for each remapped, pixel we are going to a 2 shorts (1 for x, 1 for y).
    # we used the fixed point technique, 12bits for the integer part and 4bits for the decimal part
    for y in range(0, img_height):
        const_str += '{'
        ww = []
        for x in range(0, img_width):
            xs = x << FIXED_SHIFT
            ys = y << FIXED_SHIFT

            # actual position on the distorted image: this is the one we want after reconstruction
            xd = int(map1_left[y][x][0] * FIXED_MULT + 0.5)
            yd = int(map1_left[y][x][1] * FIXED_MULT + 0.5)

            # positional difference with the perfect image
            xdiff = xd - xs
            ydiff = yd - ys

            const_str += '{'+ str(xdiff) + ', ' + str(ydiff) + '}, '
            ww += [[xdiff, ydiff]]
            count += 1
            if count == 16:
                if x == img_width - 1:
                    const_str += '},'
                if print_arrays:
                    print(const_str)
                const_str = ''
                count = 0
        pymap_left += [ww]
    if print_arrays:
        print('};\n')

    const_str = ''
    pymap_right = []
    if print_arrays:
        print('static const short right_map[' + str(img_height) + '][' + str(img_width) + '][2] = {\n')
    for y in range(0, img_height):
        const_str += '{'
        ww = []
        for x in range(0, img_width):
            xs = x << FIXED_SHIFT
            ys = y << FIXED_SHIFT

            # actual position on the distorted image: this is the one we want after reconstruction
            xd = int(map1_right[y][x][0] * FIXED_MULT + 0.5)
            yd = int(map1_right[y][x][1] * FIXED_MULT + 0.5)

            # positional difference with the perfect image
            xdiff = xd - xs
            ydiff = yd - ys

            const_str += '{'+ str(xdiff) + ', ' + str(ydiff) + '}, '
            ww += [[xdiff, ydiff]]
            count += 1
            if count == 16:
                if x == img_width - 1:
                    const_str += '},'
                if print_arrays:
                    print(const_str)
                const_str = ''
                count = 0
        pymap_right += [ww]
    if print_arrays:
        print('};')

    # very basic bilinear
    interpol = []
    if print_arrays:
        print('static const unsigned int interpolation_coeff[256][4] = {\n')
    for j in range(0, FIXED_MULT):
        up_weight = FIXED_MULT - j
        bottom_weight = j
        for i in range(0, FIXED_MULT):
            left_weight = FIXED_MULT - i
            right_weight = i
            a = left_weight*up_weight
            b = right_weight*up_weight
            c = left_weight*bottom_weight
            d = right_weight*bottom_weight
            if print_arrays:
                print('{%d, %d, %d, %d},' %(a, b, c, d))
            interpol += [[a, b, c, d]]

    if print_arrays:
        print('};\n')

    return map1_left, map2_left, map1_right, map2_right, pymap_left, pymap_right, img_height, img_width


def run_stereo(test_path, map1_left, map2_left, map1_right, map2_right, pymap_left, pymap_right, img_height, img_width,
               test_our_calibration, test_theory, test_our_stereo, skip_calibration):

    # Create StereoSGBM and prepare all parameters
    window_size = 3
    min_disp = 2
    num_disp = 130 - min_disp
    stereo = cv2.StereoSGBM_create(minDisparity=min_disp, numDisparities=num_disp,
                                   blockSize=window_size, uniquenessRatio=10,
                                   speckleWindowSize=100, speckleRange=32,
                                   disp12MaxDiff=5, P1=8 * 3 * window_size ** 2,
                                   P2=32 * 3 * window_size ** 2)

    # Used for the filtered image
    stereo_right = cv2.ximgproc.createRightMatcher(stereo)  # Create another stereo for right this time

    # WLS FILTER Parameters
    lmbda = 80000
    sigma = 1.8
    # visual_multiplier = 1.0

    wls_filter = cv2.ximgproc.createDisparityWLSFilter(matcher_left=stereo)
    wls_filter.setLambda(lmbda)
    wls_filter.setSigmaColor(sigma)

    print("using test path: " + test_path)
    pics_path = glob.glob(os.path.join(test_path, "*_left.bmp"))
    for path_left in pics_path:
        # look for all the images in the selected path
        # there must be images ending with _left.bmp an the corresponding one ending with _right.bmp
        path_right = path_left.split("_left")[0] + "_right.bmp"
        if not os.path.exists(path_right):
            continue

        img_right = cv2.imread(path_right)
        img_left = cv2.imread(path_left)

        cv2.imshow('Right', img_right)
        cv2.imshow('Left', img_left)


        if skip_calibration:
            # remapping is skipping, simply copy the images
            left_rectified = img_left
            right_rectified = img_right
            this_image_shape = right_rectified.shape[::-1]
            this_img_height = this_image_shape[2]
            if img_height != this_img_height:
                #this is very likely to be images with masks coming from the OpenMV modified fw, removing the mask here
                for x in range(0, img_width):
                    for y in range(img_height, this_img_height):
                        left_rectified[y,x] = [0, 0, 0]
                        right_rectified[y, x] = [0, 0, 0]
        else:
            # Rectify the images on rotation and alignment
            left_rectified = cv2.remap(img_left, map1_left, map2_left, cv2.INTER_LANCZOS4, cv2.BORDER_CONSTANT, 0)
            right_rectified = cv2.remap(img_right, map1_right, map2_right, cv2.INTER_LANCZOS4, cv2.BORDER_CONSTANT, 0)
            cv2.imshow('Right_rectified', right_rectified)
            cv2.imshow('Left_rectified', left_rectified)

        if test_our_calibration:
            # use the map that we just creation in the calibration function
            our_right_rectified = cv2.imread(path_right)

            # BadImage => undist => NiceImage
            num_ch = len(img_right[0,0  ])
            for x in range(0, img_width):
                for y in range(0, img_height):

                    xd = pymap_right[y][x][0] >> FIXED_SHIFT
                    yd = pymap_right[y][x][1] >> FIXED_SHIFT
                    interp_x = pymap_right[y][x][0] & FIXED_MASK
                    interp_y = pymap_right[y][x][1] & FIXED_MASK

                    xddi = x + xd
                    yddi = y + yd
                    if xddi < 0 or xddi >= img_width or yddi <0 or yddi >= img_height:
                        # the pixel to get is outside the image, simply set a black then
                        our_right_rectified[y, x] = [0, 0, 0]
                    else:
                        pix = []
                        if xddi == img_width -1:
                            interp_x = 0
                        if yddi == img_height -1:
                            interp_y = 0

                        if interp_x:
                            if interp_y:

                                interp = interp_x + (interp_y << FIXED_SHIFT)
                                for ch in range(0, num_ch):
                                    # basic 2D interpolation
                                    val = img_right[yddi,   xddi  ][ch] * interpol[interp][0] + \
                                          img_right[yddi,   xddi+1][ch] * interpol[interp][1] + \
                                          img_right[yddi+1, xddi  ][ch] * interpol[interp][2] + \
                                          img_right[yddi+1, xddi+1][ch] * interpol[interp][3]
                                    pix += [(val + FIXED_INTERPOL_ROUND)>> FIXED_INTERPOL_SHIFT]
                            else:
                                # basic 1D interpolation
                                for ch in range(0, num_ch):
                                    val = (img_right[yddi,   xddi  ][ch] * (FIXED_MULT - interp_x) +
                                        img_right[yddi,   xddi+1][ch] * interp_x + (1 << (FIXED_SHIFT-1))) >> FIXED_SHIFT
                                    pix += [val]
                        else:
                            if interp_y:
                                # basic 1D interpolation
                                for ch in range(0, num_ch):
                                    val = (img_right[yddi,   xddi  ][ch] * (FIXED_MULT - interp_y) +
                                      img_right[yddi+1, xddi  ][ch] * interp_y + (1 << (FIXED_SHIFT-1))) >> FIXED_SHIFT
                                    pix += [val]
                            else:
                                # no decimal part, we are exactly on pixel, so no interpolation
                                pix = img_right[yddi, xddi]
                        our_right_rectified[y, x] = pix

            cv2.imshow('our_right_rectified', our_right_rectified)

        # check what OpenCV is capaable of, regarding stereovision
        if test_theory:
            # Convert from color(BGR) to gray
            right_gray= cv2.cvtColor(right_rectified,cv2.COLOR_BGR2GRAY, 1)
            left_gray= cv2.cvtColor(left_rectified,cv2.COLOR_BGR2GRAY, 1)

            # Compute the 2 images for the Depth_image
            disparity_left = stereo.compute(left_gray,right_gray)
            disparity_right = stereo_right.compute(right_gray,left_gray)
            disparity_left= np.int16(disparity_left)
            disparity_right= np.int16(disparity_right)

            # Using the WLS filter
            filtered_left= wls_filter.filter(disparity_left,left_gray,None,disparity_right)
            filtered_left = cv2.normalize(src=filtered_left, dst=filtered_left, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX)
            filtered_left = np.uint8(filtered_left)
            filtered_right= wls_filter.filter(disparity_right,right_gray,None,disparity_left)
            filtered_right = cv2.normalize(src=filtered_right, dst=filtered_right, beta=0, alpha=255, norm_type=cv2.NORM_MINMAX)
            filtered_right = np.uint8(filtered_right)

            cv2.imshow('filtered_left', filtered_left)
            cv2.imshow('filtered_right', filtered_right)
            #cv2.waitKey(1000)

            # Colors map
            colored_right= cv2.applyColorMap(filtered_left,cv2.COLORMAP_JET)
            colored_left = cv2.applyColorMap(filtered_right, cv2.COLORMAP_JET)
            cv2.imshow('RightColor',colored_right)
            cv2.imshow('LeftColor', colored_left)
            # cv2.waitKey(5000)

        # very basic filtering technique to start with
        if test_our_stereo:
            # Convert from color(BGR) to gray
            right_gray= cv2.cvtColor(right_rectified,cv2.COLOR_BGR2GRAY, 1)
            left_gray= cv2.cvtColor(left_rectified,cv2.COLOR_BGR2GRAY, 1)
            cv2.imshow('right_gray', right_gray)
            cv2.imshow('left_gray', left_gray)

            mystereo = cv2.imread(path_right)
            for x in range(0, img_width):
                for y in range(0, img_height):

                    bestt = 0
                    bestscore = 99999999999
                    for t in range(0,x):
                        score = get_score(right_gray, left_gray, t, x, y, 1, 1)
                        if score < bestscore:
                            bestscore = score
                            bestt = t
                    mystereo[y, x] = x - bestt

            cv2.imshow('mystereo', mystereo)
            #cv2.waitKey(2000)
        cv2.waitKey(2000)

def get_score(right, left, t, x, y, w, h):

    score = 0
    for i in range(x-w, x+w+1):
        for j in range(y - h, y + h +1):
            if i < 0 or i >= 320:
                return 99999999999
            if j < 0 or j >= 240:
                return 99999999999
            if i + t >= 320:
                return 99999999999

            score += abs(int(right[j, i]) - int(left[j, i + t]))

    return score

def main(args=None):

    p = argparse.ArgumentParser()
    p.add_argument('-cp', '--chess_path', help='path to the images of chessboard for calibration', default='../../images/3dprint_chessboard_QVGA/')
    p.add_argument('-cw', '--chess_width', help='chessboard width', default=10)
    p.add_argument('-ch', '--chess_height', help='chessboard height', default=7)
    p.add_argument('-pa', '--print_arrays', help='print the remap arrays to be used in the OpenMV firmware', default=False)

    p.add_argument('-tp', '--test_path', help='path to the test images', default='../../images/3dprint_test_QVGA/')
    p.add_argument('-toc', '--test_our_calibration', help='Test our own calibration map while testing the stereo', default=False)
    p.add_argument('-sc', '--skip_calibration', help='Dont apply calibration on the images',default=False)
    p.add_argument('-tt', '--test_theory', help='Test stereo opencv theory', default=False)
    p.add_argument('-tos', '--test_our_stereo', help='Test our stereo algorithm', default=False)

    pargs = p.parse_args(args)

    # Calibration part
    ml1, ml2, mr1, mr2, myl, myr, h, w = calibrate(pargs.chess_path, int(pargs.chess_width), int(pargs.chess_height),
                                                   pargs.print_arrays, False)

    # main part
    if pargs.test_our_calibration or pargs.test_theory or pargs.test_our_stereo:
        run_stereo(pargs.test_path, ml1, ml2, mr1, mr2, myl, myr, h, w, pargs.test_our_calibration,
                   pargs.test_theory, pargs.test_our_stereo, pargs.skip_calibration)

if __name__ == '__main__':
    main()

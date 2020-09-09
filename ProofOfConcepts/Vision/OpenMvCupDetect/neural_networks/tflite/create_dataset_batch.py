
# standard libs
import os, random
import glob
import argparse

# math and images libs
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image


def create_my_subarray(npimg, width, height, woff, hoff):
    my_array = []
    for i in range(height):
        row = []
        for j in range(width):
            row += [[npimg.item((i+hoff,j+woff,0)), npimg.item((i+hoff,j+woff,1)), npimg.item((i+hoff,j+woff,2))]]
        my_array += [row]
    return my_array


def main(args=None):
    '''
    Before using this script, you should have saved a bunch of pictures with the OpenMV cam where a lot of cups are aligned perfectly
    and for which you know the classification.
    :param args:
    :return:
    '''

    p = argparse.ArgumentParser(description='Add pictures to the dataset to be used to train the tf model.')
    p.add_argument('--image_blob',
                   help='path to list of images to process',
                   default="cups_batch/a*.bmp")
    p.add_argument('--dataset_path',
                   help='path containing the resuling dataset',
                   default="cups_dataset/")
    p.add_argument('--manual_validation',
                   help='Visualize and manually check if the image is fine or not',
                   action = 'store_true')
    p.add_argument('--height',
                   help="Set the cup height. The input images' height should be a multiple of this value.",
                   default=19)
    p.add_argument('--width',
                   help="Set the cup width. The input images' width should be a multiple of this value.",
                   default=13)
    p.add_argument('--scale',
                   help='Scale the batch image (useful to add image with zoom algo applied)',
                   action = 'store_true')
    p.add_argument('--scaled_height',
                   help="Set the height to scale to.",
                   default=19)
    p.add_argument('--scaled_width',
                   help="Set the width to scale to",
                   default=104)
    p.add_argument('--class_array',
                   nargs='+',
                   help="array of classification values for the cup images that will be found in the image.",
                   default=[0, 0, 1, 2, 0, 2, 1, 0])

    count = 0
    file_count = 0

    pargs = p.parse_args(args)

    imglist = glob.glob(pargs.image_blob)

    if pargs.manual_validation:
        fig, ax = plt.subplots()

    for path in imglist:
        print(path)

        img = Image.open(path)
        if pargs.scale and  (pargs.scaled_height != img.height or pargs.scaled_width != img.width):
            print('rescale')
            img = img.resize((pargs.scaled_width, pargs.scaled_height), Image.BICUBIC)
        else:
            img.load()

        npimg = np.asarray(img, dtype="uint8")

        cup_number = (img.width / pargs.width) * (img.height / pargs.height)
        if cup_number != len(pargs.class_array):
            print('number of classification values not equal to number of cups in the image')
            exit(1)

        i = 0
        hoff = 0
        while hoff + pargs.height <= img.height:
            woff = 0
            while woff + pargs.width < img.width:
                ia = create_my_subarray(npimg, pargs.width, pargs.height, woff, hoff)

                if pargs.manual_validation:
                    ax.imshow(ia)
                    plt.show(block=False)
                    value = " "
                    while value != "y" and value != "n":
                        value = input("Image classified as " + str(pargs.class_array[i]) + "? y:save image, n: reject image : ")
                    if value == 'n':
                        i += 1
                        woff += pargs.width
                        continue

                npia = np.asarray(ia, dtype="uint8")
                img_to_save = Image.fromarray(npia)
                while glob.glob(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_*.bmp')):
                    file_count +=1
                img_to_save.save(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_' + str(pargs.class_array[i]) +'.bmp'))
                count += 1

                i+= 1
                woff += pargs.width
            hoff += pargs.height

if __name__ == '__main__':
    main()

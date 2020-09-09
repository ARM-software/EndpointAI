
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


def load_image_u8( infilename) :
    img = Image.open( infilename )
    img.load()
    data = np.asarray(img, dtype="uint8")
    return data


def main(args=None):
    '''
    This script randomly selects areas from an image and ask which label they should have
    :param args:
    :return:
    '''

    p = argparse.ArgumentParser(description='Add pictures to the dataset to be used to train the tf model.')
    p.add_argument('--image_blob',
                   help='path to list of images to process',
                   default="cups_feedback/*.bmp")
    p.add_argument('--dataset_path',
                   help='path containing the resuling dataset',
                   default="cups_dataset/")
    p.add_argument('--height',
                   help="Set the cup height. The input images' height should be a multiple of this value.",
                   default=19)
    p.add_argument('--width',
                   help="Set the iamge width. The input images' width should be a multiple of this value.",
                   default=13)
    p.add_argument('--img_height',
                   help="Set the image height.",
                   default=19)
    p.add_argument('--img_width',
                   help="Set the cup width.",
                   default=104)
    p.add_argument('--max_rand_per_pic',
                   help="Number of randomly seclected image to take from each image.",
                   default=10)
    count = 0
    file_count = 0

    pargs = p.parse_args(args)

    imglist = glob.glob(pargs.image_blob)
    fig, ax = plt.subplots()

    for path in imglist:
        print(path)
        img = load_image_u8(path)
        for i in range(pargs.max_rand_per_pic):
            x = random.randrange(0, pargs.img_width - pargs.width)
            y = random.randrange(0, pargs.img_height - pargs.height)
            ia = create_my_subarray(img, pargs.width, pargs.height, x, y)
            ax.imshow(ia)
            plt.show(block=False)
            value = " "
            while(value != "0" and value != "1" and value != "2" and value != "3" and value != "4"):
                value = input("0:others, 1:red, 2:green, 3:nothing, 4:discard? ")

            if(value != "4"):
                npia = np.asarray(ia, dtype="uint8")
                img_to_save = Image.fromarray(npia)
                while glob.glob(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_*.bmp')):
                    file_count += 1
                img_to_save.save(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_' + value + '.bmp'))
                count += 1


if __name__ == '__main__':
    main()

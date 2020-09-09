
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
    This script is paired with a script to be run on the OpenMV cam that generates pics with rectangle coordinates
    in their names that need to be manually verified
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
                   default=27)
    p.add_argument('--width',
                   help="Set the cup width. The input images' width should be a multiple of this value.",
                   default=19)
    count = 0
    file_count = 0

    pargs = p.parse_args(args)

    imglist = glob.glob(pargs.image_blob)
    fig, ax = plt.subplots()
    for path in imglist:
        print(path)
        name = os.path.basename(path)
        coord = name.split('.')[0].split("_")
        num_coord = (len(coord) -1) / 2

        img = load_image_u8(path)
        for i in range(int(num_coord)):
            x = int(coord[2*i+1])
            y = int(coord[2 * i + 2])
            ia = create_my_subarray(img, pargs.width, pargs.height, x, y)
            ax.imshow(ia)
            plt.show(block=False)
            value = " "
            while(value != "0" and value != "1" and value != "2" and value != "3"):
                value = input("0:others, 1:red, 2:green, 3:nothing ? ")

            npia = np.asarray(ia, dtype="uint8")
            img_to_save = Image.fromarray(npia)
            while glob.glob(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_*.bmp')):
                file_count += 1
            img_to_save.save(os.path.join(pargs.dataset_path, 'dataset_' + str(file_count) + '_' + value + '.bmp'))
            count += 1


if __name__ == '__main__':
    main()

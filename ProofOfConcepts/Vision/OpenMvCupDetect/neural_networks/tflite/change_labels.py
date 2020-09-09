# standard libs
import os, random
import glob
import argparse

# math and images libs
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image

def load_image_u8( infilename) :
    img = Image.open( infilename )
    img.load()
    data = np.asarray(img, dtype="uint8")
    return img, data.copy()


def main(args=None):

    p = argparse.ArgumentParser()
    p.add_argument('--input',
                   help='path to the dataset to use to train the model',
                   default="dataset_improved3/dataset_*.bmp")
    p.add_argument('--output',
                   help='path to the dataset to use to train the model',
                   default="dataset_improved3_looser")

    plt.ion()
    pargs = p.parse_args(args)

    imglist = glob.glob(pargs.input)
    imglist = sorted(imglist)
    total_count = len(imglist)
    cur_count = 0
    for path in imglist:
        basename = os.path.basename(path)
        print(str(cur_count) + "/" + str(total_count) + ": " + path)
        cur_count += 1
        #name should have the correct width and height and be in the format imagename_labelvalue.bmp

        name, ext = basename.split('.')
        names = name.split("_")

        label = names[-1]
        virgin_name = "_".join(names[:-1])

        pil_img, img = load_image_u8(path)
        if label == "3":
            plt.imshow(img)
            plt.pause(0.01)
            label = " "
            while (label != "0" and label != "1" and label != "2" and label != "3"):
                label = input("0:others, 1:red, 2:green, 3:nothing? ")

        pil_img.save(os.path.join(pargs.output, virgin_name + '_' + label + '.bmp'))

if __name__ == '__main__':
    main()

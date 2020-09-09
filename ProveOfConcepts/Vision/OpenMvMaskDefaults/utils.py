from PIL import Image
import numpy as np
import os

def calculate_class_weight(labels):
    """Calculates the inverse of the class cardinalities and
       normalizes the weights such that the minimum is equal to 1.
    
    Args:
        labels: List of integers representing class labels

    Returns:
        Numpy array with weight for each class

    """
    labels = np.array(labels)
    unique = sorted(np.unique(labels))
    counts = np.zeros(len(unique))
    for i, label in enumerate(unique):
        counts[i] = np.sum(labels == label)
    weight = 1. / counts
    weight = weight / weight.min()
    return weight


def load_image(infilename, insize, dtype='float32'):
    """Loads an image and returns the content as numpy array

    Args:
        infilename: Filename of the image to load
        insize: Size to which the image should be reshaped
        dtype: Type of the numpy array

    Returns:
        Numpy array of shape insize x insize x 3

    """
    img = Image.open( infilename )
    img.load()
    data = np.asarray(
        img.resize(
            (insize, insize),
            Image.ANTIALIAS
        ),
        dtype=dtype
    )
    return data


def exists_jpg_extension(files):
    """Checks whether any file in files is a jpg image

    Args:
        files: List of filenames

    Returns:
        True if any file is a jpg otherwise False

    """
    has_jpg_extension = ['jpg' in fname for fname in files]
    return any(has_jpg_extension)


def masks_dataset_gen(path, input_dim):
    """Generator for mask dataset

    Args:
        path: Path to directory of the masks dataset
        input_dim: Input dimension of the classifier

    Returns:
        Generator object which yields mask images with labels

    """
    label_lookup = {
        'bad': 0,
        'good': 1,
        'no_mask': 2,
    }

    for dir_path, _, files in os.walk(path):

        if not exists_jpg_extension(files) or len(files) == 0:
            continue
 
        for fname in files:

            full_path = os.path.join(dir_path, fname)
            data = load_image(full_path, input_dim).astype(np.uint8)
            label = label_lookup[os.path.basename(dir_path)]

            yield [data], [label]
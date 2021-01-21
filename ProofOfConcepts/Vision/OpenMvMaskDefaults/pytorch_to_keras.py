import argparse

import lightly.models as models
import torch
import torch.nn as nn

from classifier import Classifier
from pytorch2keras import pytorch_to_keras

parser = argparse.ArgumentParser()
parser.add_argument('--finetuned', type=str, default='checkpoints/finetuned.pt',
                    help='Path to the finetuned model checkpoint')
parser.add_argument('--keras_model', type=str, default='checkpoints/keras_model.h5',
                    help='Path to the keras model checkpoint (output)')
parser.add_argument('--model', type=str, default='resnet-9', 
                    help='ResNet version')
parser.add_argument('--width', type=float, default=0.125,
                    help='Width of the ResNet')
parser.add_argument('--num_ftrs', type=int, default=16,
                    help='Dimension of the feature representations.')
parser.add_argument('--input_dim', type=int, default=64,
                    help='Input dimension of the training images')
args = parser.parse_args()

# load finetuned model
resnet = models.ResNetGenerator(args.model, args.width)
last_conv_channels = list(resnet.children())[-1].in_features
resnet = nn.Sequential(
    models.batchnorm.get_norm_layer(3, 0),
    *list(resnet.children())[:-1],
    nn.Conv2d(last_conv_channels, args.num_ftrs, 1),
    nn.AdaptiveAvgPool2d(1),
)
model = models.SimCLR(
    resnet,
    num_ftrs=args.num_ftrs,
)

classifier = Classifier(model)
state_dict = torch.load(args.finetuned)
classifier.load_state_dict(state_dict)
classifier.eval()

# dummy
dummy_input = torch.ones(1, 3, args.input_dim, args.input_dim)

# pytorch to keras
keras_model = pytorch_to_keras(
    classifier,
    dummy_input,
    [(3, args.input_dim, args.input_dim)],
    change_ordering=True
)
keras_model.summary()

# save checkpoint
print('Conversion finished! Storing checkpoint at %s' % (args.keras_model))
keras_model.save(args.keras_model)

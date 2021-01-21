import sys
import argparse

import torch
import torch.nn as nn
import torchvision

import numpy as np

import lightly.data as data
import lightly.models as models

from classifier import Classifier
from utils import calculate_class_weight

# parse arguments
parser = argparse.ArgumentParser()
parser.add_argument('--data', type=str, default='',
                    help='Path to directory which contains training images')
parser.add_argument('--test_data', type=str, default='',
                    help='Path to directory which contains test images')
parser.add_argument('--pretrained', type=str, default='checkpoints/pretrained.ckpt',
                    help='Path to the pretrained model checkpoint')
parser.add_argument('--finetuned', type=str, default='checkpoints/finetuned.pt',
                    help='Path to the finetuned model checkpoint (output)')
parser.add_argument('--batch_size', type=str, default=200,
                    help='Batch size for training')
parser.add_argument('--epochs', type=int, default=100,
                    help='Number of iterations through the whole dataset')
parser.add_argument('--model', type=str, default='resnet-9', 
                    help='ResNet version')
parser.add_argument('--width', type=float, default=0.125,
                    help='Width of the ResNet')
parser.add_argument('--input_dim', type=int, default=64,
                    help='Input dimension of the training images')
parser.add_argument('--output_dim', type=int, default=3,
                    help='Output dimension of the ResNet (number of logits)')
parser.add_argument('--classifier_lr', type=float, default=1e-0,
                    help='Learning rate for the linear layer of the classifier')
parser.add_argument('--features_lr', type=float, default=1e-5,
                    help='Learning rate for the (pretrained) feature extractor')
parser.add_argument('--momentum', type=float, default=0.9,
                    help='Momentum for SGD')
parser.add_argument('--weight_decay', type=float, default=5e-4,
                    help='L2 regularization')
parser.add_argument('--color_jitter', type=float, default=0.25,
                    help='Strength of color-jitter transformation of training image')
parser.add_argument('--min_scale', type=float, default=0.9,
                    help='Minimum scale for randomized resized crop of training image')
parser.add_argument('--p_horizontal_flip', type=float, default=0.5,
                    help='Probability of flipping training image horizontally')
parser.add_argument('--p_vertical_flip', type=float, default=0.5,
                    help='Probability of flipping training image vertically')
parser.add_argument('--seed', type=int, default=1234,
                    help='Random seed.')
args = parser.parse_args()

if not args.data:
    print('Please specify training data using the --data argument.')
    print('Example: python finetune.py --data path/to/training/data')
    sys.exit()

# reseed
np.random.seed(args.seed)
torch.manual_seed(args.seed)
torch.cuda.manual_seed(args.seed)
torch.backends.cudnn.deterministic = True
torch.backends.cudnn.benchmark = False

# cuda?
device = 'cuda' if torch.cuda.is_available() else 'cpu'
print('Finetuning on device: %s' % (device))

# helper
def filter_state_dict(state_dict):
    new_state_dict = {}
    for key, item in state_dict.items():
        new_key = '.'.join(key.split('.')[1:])
        new_state_dict[new_key] = item
    return new_state_dict
    
# initialize pretrained resnet
resnet = models.ResNetSimCLR(name=args.model, width=args.width).to(device)
if args.pretrained:
    state_dict = filter_state_dict(torch.load(args.pretrained)['state_dict'])
    resnet.load_state_dict(state_dict)
else:
    print('No checkpoint was specified, weights will be initialized randomly')

# initialize classifier
classifier = Classifier(
    resnet,
    input_dim=args.input_dim,
    output_dim=args.output_dim
).to(device)

# augmentations
augmentations = [
    torchvision.transforms.ColorJitter(
        brightness=args.color_jitter,
        contrast=args.color_jitter,
        saturation=args.color_jitter,
        hue=args.color_jitter,
    ),
    torchvision.transforms.RandomResizedCrop(
        args.input_dim,
        scale=(args.min_scale, 1.)
    ),
    torchvision.transforms.RandomHorizontalFlip(
        p=args.p_horizontal_flip
    ),
    torchvision.transforms.RandomVerticalFlip(
        p=args.p_vertical_flip
    ),
    torchvision.transforms.ToTensor(),
]
transforms = torchvision.transforms.Compose(augmentations)

# load training data
dataset = data.LightlyDataset(
    from_folder=args.data,
    transform=transforms)
dataloader = torch.utils.data.DataLoader(
    dataset,
    batch_size=args.batch_size,
    shuffle=True
)

# class weights
labels = [label for _, label, _ in dataset]
weight = calculate_class_weight(labels)
weight = torch.FloatTensor(weight).to(device)

# criterion 
criterion = nn.CrossEntropyLoss(weight=weight)


# optimizer
parameter_groups = [
    {
        'params': classifier.classifier.parameters(),
        'lr': args.classifier_lr
    },
    {
        'params': classifier.features.parameters(),
        'lr': args.features_lr
    },
]
optimizer = torch.optim.SGD(
    parameter_groups,
    lr=0.1,
    momentum=args.momentum,
    weight_decay=args.weight_decay,
)

# training
def train_epoch(avg_loss, avg_accuracy):

    for batch, target, _ in dataloader:

        batch = batch.to(device)
        target = target.to(device)

        optimizer.zero_grad()
        output = classifier(batch)
        loss = criterion(output, target)
        loss.backward()
        optimizer.step()

        _, label = output.max(-1)
        accuracy = (target == label).float().mean().item()

        avg_loss = loss.item() if avg_loss is None else avg_loss
        avg_loss =  0.9 * avg_loss + 0.1 * loss.item()
        avg_accuracy = accuracy if avg_accuracy is None else avg_accuracy
        avg_accuracy = 0.9 * avg_accuracy + 0.1 * accuracy
    
    return avg_loss, avg_accuracy

loss, accuracy = None, None
for e in range(1, args.epochs + 1):
    loss, accuracy = train_epoch(loss, accuracy)
    print('[Epoch %3d] Loss (running avg) %.3f, Accuracy (running avg) %.3f' %
          (e, loss, accuracy))

# save checkpoint
print('Finetuning finished! Storing checkpoint at %s' % (args.finetuned))
classifier.eval()
torch.save(classifier.state_dict(), args.finetuned)

# test
if not args.test_data:
    print('If you want to test the finetuned model, specify a test dataset with --test_data')
    print('Example: python finetune.py --data path/to/training/data --test_data path/to/test/data')
    sys.exit()

transformations = [
    torchvision.transforms.Resize(args.input_dim),
    torchvision.transforms.ToTensor(),
]
transforms = torchvision.transforms.Compose(transformations)
testset = data.LightlyDataset(
    from_folder=args.test_data,
    transform=transforms,
)
testloader = torch.utils.data.DataLoader(
    testset,
    batch_size=1,
)

with torch.no_grad():
    test_loss, test_accuracy = 0., 0.
    for batch, target, _ in testloader:

        batch = batch.to(device)
        target = target.to(device)

        output = classifier(batch)
        loss = criterion(output, target)
        _, label = output.max(-1)
        accuracy = (target == label).float().mean().item()

        test_loss += loss.item() / len(testset)
        test_accuracy += accuracy / len(testset)
    
print('[Test] Loss %.3f, Accuracy %.3f' % (test_loss, test_accuracy))

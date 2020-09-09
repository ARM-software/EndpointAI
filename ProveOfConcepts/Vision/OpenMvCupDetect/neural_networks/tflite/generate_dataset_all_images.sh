#!/bin/bash 

# Generate all dataset images from batch images
# Update the start and end number for your files

START=10
END=59

for i in $(seq $START $END);
	do echo "Loop iteration "$i;
	python create_dataset_batch.py --image_blob ./cups_batch/example_cups"$i".bmp
done

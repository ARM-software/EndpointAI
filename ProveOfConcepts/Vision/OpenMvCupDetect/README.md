# OpenMV

Smart Vision Projects based on the OpenMV H7 Camera.

## Cups recognizer

This project is a POC on how to detect red and green cups on a shelf.
It currently has 2 parts:
* omv_python: the python scripts to be run on the OpenMV Camera.
* neural_networks/tflite: the scripts used to create and train the tensorflow lite neural network used to detect the cups.

### Creating the neural network training dataset

Several scripts have been created to create the training dataset which should have as many examples as possible:
* coke_cups_batch.py on the camera side and create_dataset_batch.py on the post-treatment side alow to create training image by batch: a picture of several known cups aligned is taken and then divided in the correct number of cups images with the correct label.
* coke_cups_feedback.py on the camera side and create_feedback_dataset.py on the post-treatment side alow to run the neural_network and manually correct its mistake by adding the cups pics with the manually corrected label to the dataset.

### Creating and training the tflite neural network

create_and_train_model.py based on tensorflow creates and trains the neural network using the dataset that should have been already created.
The resulting tflite model should then be copied on the camera internal memory or its SD card.

### Running the cups detection

coke_cups_detection.py is used to detect cups on the OpenMV Camera

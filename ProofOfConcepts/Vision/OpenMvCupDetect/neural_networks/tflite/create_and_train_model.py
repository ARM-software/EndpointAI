
# standard libs
import os, random
import glob
import argparse

# tensorflow stuff
import tensorflow as tf
from tensorflow.keras import layers

# math and images libs
import numpy as np
import matplotlib.pyplot as plt
from PIL import Image


def load_image( infilename) :
    img = Image.open( infilename )
    img.load()
    data = np.asarray(img, dtype="float32")
    return data


def load_image_u8( infilename) :
    img = Image.open( infilename )
    img.load()
    data = np.asarray(img, dtype="uint8")
    return data


def create_my_cup_array(npimg, width, height):
    my_array = []
    for i in range(height):
        row = []
        for j in range(width):
            row += [[npimg.item((i,j,0)), npimg.item((i,j,1)), npimg.item((i,j,2))]]
        my_array += [row]
    return my_array


def float_pixel_to_8bits(x):
    return 255*x


def main(args=None):

    p = argparse.ArgumentParser()
    p.add_argument('--dataset_blob',
                   help='path to the dataset to use to train the model',
                   default="cups_dataset/dataset_*.bmp")
    p.add_argument('--model_name',
                   help='path and name of the resulting tflite model',
                   default="./model_cup_quantized_256.tflite")
    p.add_argument('--height',
                   help="Set the cup height. The input images' height should be a multiple of this value.",
                   default=19)
    p.add_argument('--width',
                   help="Set the cup width. The input images' width should be a multiple of this value.",
                   default=13)
    p.add_argument('--display_graphs',
                   help='Display graphs to check the quality of the model/training',
                   action = 'store_true')

    pargs = p.parse_args(args)

    # skip the first statistically invalid epochs
    epochs_skip = 10


    # declare the dataset (inages and their corresponging label) that is going to be used
    # for training
    train_img = []
    train_label = []
    # for validation
    val_img = []
    val_label = []
    #for test
    test_img = []
    test_label = []
    # for test when quantized
    np_test_img_u8 = []
    np_train_img = []

    # open all the images and put them in the arrays above
    # 60% are for training
    # 20% are for validation
    # 20% are for tests
    seed = random.randrange(0,1000000)
    random.seed(seed)
    print("the seed is: " + str(seed))
    count = 0
    imglist = glob.glob(pargs.dataset_blob)
    for path in imglist:
        name = os.path.basename(path)
        print(path)
        rndm = random.randrange(0, 10, 1)
        #name should have the correct width and height and be in the format imagename_labelvalue.bmp
        label = int(name.split('.')[0].split("_")[-1])

        if rndm % 10 == 0 or rndm % 10 == 1:
            val_img += [create_my_cup_array(load_image(path), pargs.width, pargs.height)]
            val_label += [label]
        elif rndm % 10 == 2 or rndm % 10 == 3:
            np_test_img_u8 += [load_image_u8(path)]
            test_img += [create_my_cup_array(load_image(path), pargs.width, pargs.height)]
            test_label += [label]
        else:
            np_train_img += [load_image_u8(path)]
            train_img += [create_my_cup_array(load_image(path), pargs.width, pargs.height)]
            train_label += [label]
        count += 1

    print("number of images used in the dataset is " + str(count))

    # model definition
    model_cup = tf.keras.Sequential()
    model_cup.add(layers.Flatten(input_shape=(pargs.height,pargs.width,3)))
    model_cup.add(layers.Dense(256, activation='relu'))
    model_cup.add(layers.Dense(4))
    model_cup.compile(optimizer='adam', loss=tf.keras.losses.SparseCategoricalCrossentropy(from_logits=True), metrics=['accuracy'])

    # Train the model on our training data while validating on our validation set
    history_1 = model_cup.fit(train_img, train_label, epochs=300, batch_size=32, validation_data=(val_img, val_label))

    if pargs.display_graphs:
        # Draw a graph of the loss, which is the distance between
        # the predicted and actual values during training and validation.
        loss = history_1.history['loss']
        val_loss = history_1.history['val_loss']
        epochs = range(1, len(loss) + 1)
        plt.plot(epochs, loss, 'g.', label='Training loss')
        plt.plot(epochs, val_loss, 'b', label='Validation loss')
        plt.title('Training and validation loss')
        plt.xlabel('Epochs')
        plt.ylabel('Loss')
        plt.legend()
        plt.show()

        # Exclude the first few epochs so the graph is easier to read
        plt.plot(epochs[epochs_skip:], loss[epochs_skip:], 'g.', label='Training loss')
        plt.plot(epochs[epochs_skip:], val_loss[epochs_skip:], 'b.', label='Validation loss')
        plt.title('Training and validation loss')
        plt.xlabel('Epochs')
        plt.ylabel('Loss')
        plt.legend()
        plt.show()

        # Draw a graph of accuracy, which is another way of
        # measuring the amount of error in the prediction.
        plt.clf()
        accuracy = history_1.history['accuracy']
        val_accuracy = history_1.history['val_accuracy']
        plt.plot(epochs[epochs_skip:], accuracy[epochs_skip:], 'g.', label='Training accuracy')
        plt.plot(epochs[epochs_skip:], val_accuracy[epochs_skip:], 'b.', label='Validation accuracy')
        plt.title('Training and validation mean absolute error')
        plt.xlabel('Epochs')
        plt.ylabel('accuracy')
        plt.legend()
        plt.show()

        probability_model = tf.keras.Sequential([model_cup, tf.keras.layers.Softmax(), tf.keras.layers.Lambda(float_pixel_to_8bits, dtype="uint8")])
        # Use the model to make predictions from our validation data
        predictions = probability_model.predict(test_img)

        max_prediction = []
        for i in range(len(predictions)):
            print("test label " + str(i) + ": " + str(test_label[i]))
            max_prediction += [np.argmax(predictions[i])]
            print("prediction " + str(i) + ": " + str(np.argmax(predictions[i])))
            print("prediction " + str(i) + ": " + str(predictions[i]))
            print("======================")

        # Plot the predictions along with to the test data
        plt.clf()
        plt.title('Training data predicted vs actual values')
        plt.plot(range(len(predictions)), test_label, 'bo', label='Actual')
        plt.plot(range(len(predictions)), max_prediction, 'rv', label='Predicted')
        plt.legend()
        plt.show()

    # save the model so that it cup be reopened and quantized
    tf.keras.models.save_model(
        model_cup, "saved_model", overwrite=True, include_optimizer=True, save_format=None,
        signatures=None, options=None
    )

    def representative_dataset_gen():
        img = 0
        trainlen = len(train_img)
        while (img < trainlen):
            img += 1
            yield [np_train_img[img-1]]

    # quantize the model so that it cup be usable by the OpenMV cam
    # this should be updated as soon as a better to do it is available
    converter = tf.compat.v1.lite.TFLiteConverter.from_keras_model_file("saved_model")
    converter.inference_type = tf.compat.v1.lite.constants.QUANTIZED_UINT8
    converter.default_ranges_stats = (0, 255)
    input_arrays = converter.get_input_arrays()
    converter.quantized_input_stats = {input_arrays[0]: (0., 1.)}  # mean, std_dev
    converter.representative_dataset = representative_dataset_gen
    tflite_model = converter.convert()

    # save the tflite model that will be used by the OpenMV cam
    open(pargs.model_name, "wb").write(tflite_model)

    if pargs.display_graphs:
        # Instantiate an interpreter for the model
        tf_model_quantized = tf.lite.Interpreter(pargs.model_name)

        # Allocate memory for the model
        tf_model_quantized.allocate_tensors()

        print("input")
        print(tf_model_quantized.get_input_details()[0])
        print("output")
        print(tf_model_quantized.get_output_details()[0])

        # Create arrays to store the results
        tf_model_quantized_predictions = np.empty(len(test_img))

        miss = 0
        # Run the model's interpreter for each value and store the results in arrays
        for i in range(len(test_img)):
            tf_model_quantized.set_tensor(tf_model_quantized.get_input_details()[0]['index'], [np_test_img_u8[i]])
            tf_model_quantized.invoke()
            result = tf_model_quantized.get_tensor(tf_model_quantized.get_output_details()[0]['index'])
            print(result)
            print("max " + str( np.argmax(result)))
            print("actual" + str(test_label[i]))
            if test_label[i] != np.argmax(result):
                miss+=1

            tf_model_quantized_predictions[i] = np.argmax(result)

        print("missed " + str(miss) + "/" + str(range(len(test_img))))

        # See how they line up with the data
        plt.clf()
        plt.title('Comparison of various models against actual values')
        plt.plot(range(len(test_img)), test_label, 'bo', label='Actual')
        plt.plot(range(len(test_img)), tf_model_quantized_predictions, 'gx', label='Lite quantized predictions')
        plt.legend()
        plt.show()

if __name__ == '__main__':
    main()

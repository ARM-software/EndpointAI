import argparse
import tensorflow as tf

from utils import masks_dataset_gen

parser = argparse.ArgumentParser()
parser.add_argument('--tflite_model', type=str, default='checkpoints/lightly_resnet9.tflite',
                    help='Path to the keras model checkpoint')
parser.add_argument('--test_data', type=str, default='data/test',
                    help='Path to directory which contains test images')
parser.add_argument('--input_dim', type=int, default=64,
                    help='Input dimension of the test images')
args = parser.parse_args()

# initialize interpreter
interpreter = tf.lite.Interpreter(model_path=args.tflite_model)
interpreter.allocate_tensors()

tensor_details = interpreter.get_tensor_details()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

# test
accuracy, count = 0., 0
dataset_generator = masks_dataset_gen(args.test_data, args.input_dim)
for data, label in dataset_generator:
    interpreter.set_tensor(input_details[0]['index'], data)
    interpreter.invoke()
    output_data = interpreter.get_tensor(output_details[0]['index'])
    accuracy += float(output_data.argmax(1) in label)
    count += 1

accuracy = accuracy / count
print('Test accuracy of quantized model on %d images: %.3f' % (count, accuracy))
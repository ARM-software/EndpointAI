import argparse
import tensorflow.compat.v1.lite as lite

parser = argparse.ArgumentParser()
parser.add_argument('--keras_model', type=str, default='checkpoints/keras_model.h5',
                    help='Path to the keras model checkpoint')
parser.add_argument('--tflite_model', type=str, default='checkpoints/whattolabel_resnet9.tflite',
                    help='Path to the keras model checkpoint')
parser.add_argument('--quantized_input_lower', type=int, default=0,
                    help='Lower input stat')
parser.add_argument('--quantized_input_upper', type=int, default=255,
                    help='Upper input stat')
parser.add_argument('--default_ranges_lower', type=float, default=-25,
                    help='Lower boundary of default range')
parser.add_argument('--default_ranges_upper', type=float, default=25,
                    help='Upper boundary of default range')
args = parser.parse_args()

# initialize converter
converter = lite.TFLiteConverter.from_keras_model_file(args.keras_model)
converter.inference_type = lite.constants.QUANTIZED_UINT8
input_arrays = converter.get_input_arrays()
converter.quantized_input_stats = {
    input_arrays[0]: (
        args.quantized_input_lower,
        args.quantized_input_upper,
    )
}
converter.default_ranges_stats = (
    args.default_ranges_lower,
    args.default_ranges_upper,
)

# converte and save checkpoint
tflite_model = converter.convert()
print('Conversion finished! Storing checkpoint at %s' % (args.tflite_model))
open(args.tflite_model, 'wb').write(tflite_model)
"""Example code to run a Tensorflow Lite model for a classification task

This code demonstrates how you can use a Tensorflow Lite model to run image
classificaiton. We use the OpenMV camera to classify a face mask.
We have three classes (bad masks, good masks and no masks).
The code notifies the user using the LED and terminal output about the
classification result.
 > Good mask : green LED
 > Bad mask  : red LED
 > No mask   : blue LED


With the device attached to the computer you should get around 3.17 fps.

This code has been developed and tested using the OpenMV IDE 2.5.0
The firmware used for the OpenMV H7 board is 3.6.4
"""

import sensor, time, tf, pyb

# Set the maximum Frames Per Second for the main loop (fps)
MAX_FPS=2.0

# Reset and initialize the sensor.
sensor.reset()

# Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_pixformat(sensor.RGB565)

# Set frame size to 64x64 pixels
sensor.set_framesize(sensor.B64X64)

# Disable automatic exposure and gain
sensor.set_auto_exposure(False)
sensor.set_auto_gain(False)
sensor.set_contrast(0)
sensor.set_brightness(0)

# Wait two seconds for settings take effect.
sensor.skip_frames(time = 2000)

# Create a clock object to track the FPS.
clock = time.clock()


# Load the Tensorflow Lite model
model = tf.load('whattolabel_resnet9.tflite')

def setLED(color='none'):
    """Function to set LED to a color (red, green, blue, none)
    """

    red_LED = pyb.LED(1)
    green_LED = pyb.LED(2)
    blue_LED = pyb.LED(3)

    if color == 'red':
        red_LED.on()
        green_LED.off()
        blue_LED.off()
    elif color == 'green':
        red_LED.off()
        green_LED.on()
        blue_LED.off()
    elif color == 'blue':
        red_LED.off()
        green_LED.off()
        blue_LED.on()
    else:
        red_LED.off()
        green_LED.off()
        blue_LED.off()

while(True):
    # Update the FPS clock.
    clock.tick()

    # Take a picture and return the image.
    img = sensor.snapshot()

    # Run the image through our tensorflow model
    out = model.classify(img)[0]
    b, g, n = out.output()

    # Evaluate the output for the three classes (bad mask, good mask, no mask)
    if b > g and b > n:
        print('bad mask')
        setLED('red')
    elif g > b and g > n:
        print('good mask')
        setLED('green')
    elif n > g and n > b:
        print('no mask')
        setLED('blue')
    else:
        print('I can not decide')
        setLED('none')

    # sleep a bit to save energy if we're faster than MAX_FPS
    fps = clock.fps()
    sleep_time = int((1000 / MAX_FPS) - (1000 / fps))
    if sleep_time > 0:
        time.sleep(sleep_time)

    # Note: OpenMV Cam runs about half as fast when connected
    print(clock.fps())
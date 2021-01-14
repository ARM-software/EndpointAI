import image, network, rpc, sensor, struct
import time
import micropython

from pyb import Pin
from pyb import LED

# variables that can be changed
save_to_SD = False

# leds are used as an easy way to know if the remote camera has started fine
red_led   = LED(1)
green_led = LED(2)
blue_led  = LED(3)
ir_led    = LED(4)

def led_control(x):
    if   (x&1)==0: red_led.off()
    elif (x&1)==1: red_led.on()
    if   (x&2)==0: green_led.off()
    elif (x&2)==2: green_led.on()
    if   (x&4)==0: blue_led.off()
    elif (x&4)==4: blue_led.on()
    if   (x&8)==0: ir_led.off()
    elif (x&8)==8: ir_led.on()

processing = True

# pin to trigger the snapshot
pin4 = Pin('P4', Pin.IN, Pin.PULL_UP)

# communication with the controller cam
interface = rpc.rpc_spi_slave(cs_pin="P3", clk_polarity=1, clk_phase=0)

# here we always choose the QQVGA format (160x120) inside a QVGA image
#if this is changed, the camera have to calibrated again
# also, the logic of mask_height should be checked
img_width = 160
img_height = 120
sensor_format = sensor.RGB565

#additionnal data for the mask height
mask_height = 0
if sensor_format == sensor.RGB565:
    mask_height = int(img_height /8)
else:
    mask_height = int(img_height / 4)

if mask_height & 1:
    mask_height += 1

sensor.reset()
sensor_size = sensor.QVGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
# Note that there is a -2 compared to QVGA as there looks to be a little misalignement when moving from QVGA to QQQVGA
# As we use the same remap arrays for both definitions, it is important that both are aligned
sensor.set_windowing((int((sensor.width()-img_width)/2)-2,int((sensor.height()-img_height)/2),img_width,img_height))

#get the gains and exposure
gain_db = sensor.get_gain_db()
exposure_us = sensor.get_exposure_us()
rgb_gain_db = sensor.get_rgb_gain_db()

# Set the gain and exposure to fixed values (we dont care about the values)
sensor.set_auto_gain(False, gain_db)
sensor.set_auto_exposure(False, exposure_us)
sensor.set_auto_whitebal(False, rgb_gain_db)

# Setup contrast, brightness and saturation
sensor.set_contrast(0) # range -3 to +3
sensor.set_brightness(0) # range -3 to +3
sensor.set_saturation(0) # range -3 to +3

# Disable night mode (auto frame rate) and black level calibration (BLC)
sensor.__write_reg(0x0E, 0b00000000) # Disable night mode
sensor.__write_reg(0x3E, 0b00000000) # Disable BLC

sensor.__write_reg(0x13, 0b00000000) # disable automated gain


################################################################
# Call Backs
################################################################
def sensor_config(data):
    global processing
    sensor_regs= struct.unpack("<16I", data)
    reg_list = [0x00, 0x01, 0x02, 0x03, 0x08, 0x10, 0x2d, 0x2e, 0x2f, 0x33, 0x34, 0x35, 0x36, 0x37, 0x37, 0x38]
    i = 0
    for sr in sensor_regs:
        sensor.__write_reg(reg_list[i], sr)
        i += 1

    gain_db = sensor.get_gain_db()
    exposure_us = sensor.get_exposure_us()
    rgb_gain_db = sensor.get_rgb_gain_db()

    processing = False
    return struct.pack("<fIfff",gain_db, exposure_us, rgb_gain_db[0], rgb_gain_db[1], rgb_gain_db[2])

def raw_image_read_cb():
    global processing
    interface.put_bytes(sensor.get_fb().bytearray(), 5000) # timeout
    processing = False

def raw_image_read(data):
    interface.schedule_callback(raw_image_read_cb)
    return bytes()

def loop_callback():
    global processing
    if not processing:
        raise Exception

# Register call backs.
interface.register_callback(raw_image_read)
interface.register_callback(sensor_config)
interface.setup_loop_callback(loop_callback)

# a simple visual way to know the slave cam has started properly and is ready
# 2 blue blinks
led_control(4)
time.sleep(500)
led_control(0)
time.sleep(500)
led_control(4)
time.sleep(500)
led_control(0)

# configuration step
try:
    processing = True
    interface.loop()
except:
    pass

#stabilisation of the cam
sensor.skip_frames(time=2000)

# save the ref image used for the diff
data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
ref_img = sensor.alloc_extra_fb(img_width, img_height, sensor_format)
img = sensor.snapshot()
img.remap(data_fb, right=True, upside_down=True)
ref_img.replace(img)

# now add an additional part that will convey the mask info
sensor.set_windowing((int((sensor.width()-img_width)/2)-2,int((sensor.height()-img_height)/2),img_width,img_height+ mask_height))

# serve for ever
while True:
    try:
        processing = True
        while not pin4.value():
            pass
        # get the image and undistort it
        sent_image = sensor.snapshot()
        # remove the distortion and apply the stereo calibration
        sent_image.remap(data_fb, right=True, upside_down=True)
        # diff it with the ref image that has also been undistorted
        # (check image_difference_special.py to understand the meaning of the arguments)
        sent_image.difference_special(ref_img, data_fb, 25, 40, 100, 200)
        interface.loop()
    except:
        pass

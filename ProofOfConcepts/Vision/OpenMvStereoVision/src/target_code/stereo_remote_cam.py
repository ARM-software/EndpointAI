import image, network, rpc, sensor, struct
import time
import micropython

from pyb import Pin
from pyb import LED

# variables that can be changed
save_to_SD = False
sensor_format = sensor.RGB565
#sensor_format = sensor.GRAYSCALE


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

# here we always choose the QVGA format (320x240) inside a VGA image
#if this is changed, the camera have to calibrated again
# also, the logic of mask_height should be checked
img_width = 320
img_height = 240

#additionnal data for the mask height
if sensor_format == sensor.RGB565:
    mask_height = int(img_height /8)
else:
    mask_height = int(img_height / 4)

sensor.reset()

sensor_size = sensor.VGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height))

# the following is not really needed, this is to do the same as the controller cam
sensor.skip_frames(time = 2000)
sensor.snapshot()


################################################################
# Call Backs
################################################################
def sensor_config(data):
    global processing
    gain_db, exposure_us, r_gain_db, g_gain_db, b_gain_db = struct.unpack("<fIfff", data)
    sensor.set_auto_gain(False, gain_db)
    sensor.set_auto_exposure(False, exposure_us)
    sensor.set_auto_whitebal(False, (r_gain_db, g_gain_db, b_gain_db))
    processing = False
    return struct.pack("<fIfff",gain_db, exposure_us, r_gain_db, g_gain_db, b_gain_db)

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

# a simple visual way to know the slave cam has started properly
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
#print("About to save background image...")
data_fb = sensor.alloc_extra_fb(img_width, img_height, sensor.RGB565)
ref_img = sensor.alloc_extra_fb(img_width, img_height, sensor_format)
img = sensor.snapshot()
img.remap(data_fb, right=True)
ref_img.replace(img)
#print("Saved background image - Now frame differencing!")

# now add an additionnal part that will convey the mask info
sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height+ mask_height))

# serve for ever
while True:
    try:
        processing = True
        while not pin4.value():
            pass
        # get the image and undistort it
        sent_image = sensor.snapshot()
        sent_image.remap(data_fb, right=True)
        # diff it with the ref image that has also been undistorted
        sent_image.difference_special(ref_img, data_fb, 25, 40, 400, 2000)
        interface.loop()
    except:
        pass

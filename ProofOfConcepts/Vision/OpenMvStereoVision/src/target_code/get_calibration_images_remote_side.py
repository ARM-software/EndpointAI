import image, network, rpc, sensor, struct
import time
import micropython

from pyb import Pin
from pyb import LED

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

# pin used to sync the 2 cams
pin4 = Pin('P4', Pin.IN, Pin.PULL_UP)

# setting the SPI communication as a slave
interface = rpc.rpc_spi_slave(cs_pin="P3", clk_polarity=1, clk_phase=0)

# here we always choose the QVGA format (320x240) inside a VGA image
img_width = 320
img_height = 240
sensor.reset()
sensor_format = sensor.GRAYSCALE
sensor_size = sensor.VGA
sensor.set_pixformat(sensor_format)
sensor.set_framesize(sensor_size)
if img_width != sensor.width() or img_height != sensor.height():
    sensor.set_windowing((int((sensor.width()-img_width)/2),int((sensor.height()-img_height)/2),img_width,img_height))
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

# a simple visual way to know the slave cam has started properly and is ready
led_control(4)
time.sleep(500)
led_control(0)
time.sleep(500)
led_control(4)
time.sleep(500)
led_control(0)


# configuration step: getting the same image settings as the controller cam
try:
    processing = True
    interface.loop()
except:
    pass


# serve for ever
while True:
    try:
        processing = True
        # GPIO sync
        while not pin4.value():
            pass
        # Get a snapshot that will be sent back to the controller cam
        sensor.snapshot()
        interface.loop()
    except:
        pass

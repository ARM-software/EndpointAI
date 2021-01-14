# Lens Correction
import sensor, image, time

sensor.reset()
sensor.set_framesize(sensor.VGA)
sensor.set_pixformat(sensor.RGB565)
sensor.set_windowing((160,120,320,240))

sensor.skip_frames(time = 2000)

data_fb = sensor.alloc_extra_fb(320, 240, sensor.RGB565)

clock = time.clock()
count = 0
remap = False
while(True):
    clock.tick()

    # test without remap, with remap QVGA and with remap QQVGA
    img = sensor.snapshot()
    if remap:
        img.remap(data_fb, right=False, upside_down=False)

    count += 1
    if count == 100:
        print("remap QVGA GRAYSCALE")
        remap = True
        sensor.set_framesize(sensor.VGA)
        sensor.set_pixformat(sensor.GRAYSCALE)
        sensor.set_windowing((160,120,320,240))
    elif count == 200:
        print("remap QVGA")
        remap = True
        sensor.set_framesize(sensor.VGA)
        sensor.set_pixformat(sensor.RGB565)
        sensor.set_windowing((160,120,320,240))
    elif count == 300:
        print("remap QQVGA")
        remap = True
        sensor.set_framesize(sensor.QVGA)
        sensor.set_pixformat(sensor.RGB565)
        sensor.set_windowing((78,60,160,120))
    elif count == 400:
        print("no remap VGA")
        remap = False
        sensor.set_framesize(sensor.VGA)
        sensor.set_pixformat(sensor.RGB565)
        sensor.set_windowing((160,120,320,240))
        count = 0

# How to Use

image to C array converter with optional resize

## Usage :

img2c.py [***-h***] [***--format*** \<***FORMAT***\>] [***--name \<NAME\>***] [***--dim DIM DIM***] [***--rot ANGLE***] \<***-i \<Input File Path***\>\> \[***-o \<Output file Path***\>\]



| Arguments                 | Description                                                  | Memo     |
| ------------------------- | ------------------------------------------------------------ | -------- |
| ***-h, --help***          | show help information                                        |          |
| -i ***Input File Path***  | Input file (png, bmp, etc..)                                 |          |
| -o ***Output File Path*** | output C file containing RGB56/RGB888 and alpha values arrays | Optional |
| --name ***Name***         | A specified array name.                                      | Optional |
| --format ***Format***     | RGB Format, i.e. RGB565 (***default***), RGB32               | Optional |
| --dim ***Width Height***  | Resize the image with the given width and height             | Optional |
| --rot ***angle***         | Rotate the image with the given angle in degrees             | Optional |


## Example:

```
python img2c.py -i ..\examples\alpha_blending\asset\HeliumRes.jpg --name Helium
```



## Installation:

Needs ***python3***, ***pillow*** (https://python-pillow.org/) & ***numpy***

```
pip install Pillow
pip install numpy
```


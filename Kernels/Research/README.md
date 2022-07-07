# README

This folder is for studies, work in progress and more generally projects. Some of them might not fully ready for use, please check the README in corresponding folder for details.



## Libraries and Projects

### 1. FFT

The goal of this project is to provide a FFT framework  more flexible than
the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) one but with similar features and performances:

   * Several transforms : Complex FFT, Real FFT, Complex FFT 2D ;
   * More radixes : 2,3,4,5,6,8 (enabled depending on the core and FFT length) ;
   * Several datatypes : f64, f32, f16, q31, q15
   * More lengths
   * Configuration scripts

The new features compared to the [CMSIS-DSP](https://github.com/ARM-software/CMSIS_5/tree/develop/CMSIS/DSP) are the support of more radixes, the FFT 2D and the configuration scripts.



### 2. Arm-2D

[Deprecated]Arm-2D for acceleration of low-level 2D image processing.

- It has been moved to https://github.com/ARM-software/Arm-2D

### 3. EigenMVE 

This project is adding a MVE backend to the [Eigen library](https://eigen.tuxfamily.org/index.php?title=Main_Page) which is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.


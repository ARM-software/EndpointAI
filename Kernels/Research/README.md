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

1. This library is a research project used to explore and demonstrate the possibilities of delivering smart-phone modern graphic user interface using low-cost and resource constraint micro-controllers. It is not a committed product from Arm, and the quality of the service is not validated with sophisticated tests but some functional tests. 
2. The library name, i.e. **Arm-2D**, is a temporary term and might be subject to change in the future. The term "the Library" used in this documents refers to the **Arm-2D** library unless state otherwise. 

### 3. EigenMVE 

This project is adding a MVE backend to the [Eigen library](https://eigen.tuxfamily.org/index.php?title=Main_Page) which is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms.


# README

This proof of concept is showing how an extended Kalman filter could be implemented with the CMSIS-DSP to run on a Cortex-M or A.

One of the difficulty is the calibration of the sensors : this demo is using a dump of already calibrated sensor values. So the focus is on the Kalman loop and not on the sensors and their calibration.

Some sympy scripts are used to generate the code : this demo can be modified to support additional or different sensors.

There is a Processing script in Orientation folder which can be used to display the result of the simulation. The simulation result should be dumped into build/output.csv for this script to work and display something like that:

![orientation](Documentation/Assets/orientation.PNG?raw=true)

Processing can be downloaded from [here](https://processing.org/).

All the details about the algorithms and the mathematics are described in Documentation/Summary.pdf

The source code and python scripts are referring to the equations inside this documentation.

A cmake and some scripts are provided to be able to build this demo using the CMSIS-DSP test framework and run on a fast model.

There is also a MDK project.

Version 1.9.0 and above of the CMSIS-DSP must be used otherwise the function arm_mat_vec_mult_f32 won't be available.



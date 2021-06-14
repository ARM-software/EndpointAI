# MVE Backend for Eigen



This is an experimental project to add a MVE backend to the [Eigen library](https://eigen.tuxfamily.org/index.php?title=Main_Page).

This folder is containing two patches:

1. eigen_helium.patch is the MVE backend for Eigen. Just apply this patch to Eigen and start using it on Cortex-M55
2. eigen_tests.patch is a patch to the Eigen test suite to be able to run the tests on a Cortex-M

We hope that at some point, the helium patch will be officially integrated to the Eigen repository. In the meantime, it is provided as a patch here.

The patch for the tests will never be integrated to the Eigen repository. Cortex-M is a very different target for Eigen than the usual ones. It is also much more constrained. Most of the tests are very big and also the test environment is assuming an OS.

To be able to run the tests on a Fast Model or Cycle mode in bare metal mode, we had to customize the main.h, reduce the size of the matrices and use the CMSIS-DSP test framework to build.



## The tests

An example cmake command is available in the build folder.

Note that to pass the Eigen tests we need to use the options `-ffp-mode=full` and `-O3` to be fully compliant with IEEE float and manage NaN.

To have the best performance, you would probably use only `-Ofast` when building a project using Eigen. But with this setting, some tests will fail.

Once the cmake command has been run, make will build a lot of executables (more than 600 !). It will take a very long time.

To run those executables and log all the errors, there is a Python script `runall.py`

This Python script has to be run from the EigenMVE folder.

You need to change the `cmd` in function `runtest` in the script to describe how to run your specific fast model because internally we are calling another Python script from `runall.py` to run on our fast models / cycle models.

If you need to rebuild, `python rullall.py -c` will ask to delete all test executable from the build folder.

The test `lscg` is not passing but probably due to the changes we did to the tests (it is not testing directly the MVE backend).

Some tests are crashing the AC6.16 compiler and they are currently disabled (but are not testing the MVE backend directly). We are working on solving this issue.

There are some limitations to the current MVE backend:

* `real * complex` matrix multiply or `real.transpose() * complex` matrix multiply is not supported and will generate a compilation error explaining that it is not supported. You'll need to cast you matrix with `real.template cast<std::complex<float>>() * complex`
* `std::complex<float16_t>` is not supported. But it looks like a limitation of Eigen currently.



## Eigen

More documentation will come to explain how to best use Eigen on a Cortex-M55.

Eigen is a complex library (but easy to use). Here is some documentation from the Eigen project:

1. [Eigen quick reference](https://eigen.tuxfamily.org/dox/group__QuickRefPage.html)

2. [Longer tutorial](https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html)

3. [FAQ](https://eigen.tuxfamily.org/index.php?title=FAQ)

4. [Understanding Eigen](https://eigen.tuxfamily.org/dox-devel/UserManual_UnderstandingEigen.html)


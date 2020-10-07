# The FVP for the CMSIS-DSP test framework is a FVP from the IP
# selection sandbox.
# They are different from the FVP you could find in the MDK.
# So, if you try to run it on a normal FVP it won't work.
# The simplest is to use the MDK project.

/usr/local/bin/cmake \
 -DCMAKE_PREFIX_PATH="absolute path_to_compiler" \
 -DCMAKE_TOOLCHAIN_FILE="absolute path tp CMSIS/DSP/armac6.cmake" \
 -DARM_CPU="cortex-m7" \
 -DROOT="absolute path to CMSIS (folder containing CMSIS and Device)" \
 -DCMAKE_BUILD_TYPE=Release \
 -DOPTIMIZED=ON \
 -DFASTMATHCOMPUTATIONS=ON \
 -DPLATFORM=FVP \
 -DDISABLEFLOAT16=ON \
 -DBASICMATH=ON \
 -DCOMPLEXMATH=OFF \
 -DCONTROLLER=OFF \
 -DFASTMATH=OFF \
 -DFILTERING=OFF \
 -DMATRIX=ON \
 -DSTATISTICS=OFF \
 -DSUPPORT=OFF \
 -DTRANSFORM=OFF \
 -DSVM=OFF \
 -DBAYES=OFF \
 -DDISTANCE=OFF \
 -DINTERPOLATION=OFF \
 -G "Unix Makefiles" \
..

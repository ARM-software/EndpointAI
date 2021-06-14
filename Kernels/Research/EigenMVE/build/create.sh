/usr/local/bin/cmake \
 -DCMAKE_PREFIX_PATH=/benchresults/ARMCompiler6.16 \
 -DCMAKE_TOOLCHAIN_FILE=/benchresults/cmsis_docker/CMSIS/DSP/armac6.cmake \
 -DARM_CPU="cortex-m55" \
 -DROOT=/benchresults/cmsis_docker \
 -DCMSISDSP=OFF \
 -DCMAKE_BUILD_TYPE=Release \
 -DOPTIMIZED=OFF \
 -DFASTMATHCOMPUTATIONS=OFF \
 -DCMAKE_C_FLAGS_RELEASE="-ffp-mode=full -DFVP -O3 -DNDEBUG -Wall -Wextra" \
 -DCMAKE_CXX_FLAGS_RELEASE="-ferror-limit=1 -ffp-mode=full -O3  -DNDEBUG -Wall -Wextra" \
 -DPLATFORM="FVP" \
 -DEXTERNALBUFFER="OFF" \
 -G "Unix Makefiles" \
..


cmake_minimum_required (VERSION 3.6)


project(fft)


include(configLib)

set(DSP ${ROOT}/CMSIS/DSP)

if (BENCHMARK)
    message("BENCHMARK MODE")
    add_library(fft STATIC TestsBench/CFFTBenchmarksF32.cpp)
    target_sources(fft PRIVATE TestsBench/CFFT2DBenchmarksF32.cpp)
    target_sources(fft PRIVATE TestsBench/RFFTBenchmarksF32.cpp)

    target_sources(fft PRIVATE TestsBench/CFFTBenchmarksQ31.cpp)
    target_sources(fft PRIVATE TestsBench/CFFTBenchmarksQ15.cpp)

    target_sources(fft PRIVATE TestsBench/CMSISCFFTBenchmarksF32.cpp)
    target_sources(fft PRIVATE TestsBench/CMSISCFFTBenchmarksQ31.cpp)
    target_sources(fft PRIVATE TestsBench/CMSISCFFTBenchmarksQ15.cpp)

    target_sources(fft PRIVATE TestsBench/CMSISRFFTBenchmarksF32.cpp)

    if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
        target_sources(fft PRIVATE TestsBench/CFFTBenchmarksF16.cpp)
        target_sources(fft PRIVATE TestsBench/CFFT2DBenchmarksF16.cpp)
        target_sources(fft PRIVATE TestsBench/RFFTBenchmarksF16.cpp)
    endif()
else()
    message("TEST MODE")
    add_library(fft STATIC TestsBench/CFFTTestsF32.cpp)
    target_sources(fft PRIVATE TestsBench/CFFTTestsQ31.cpp)
    target_sources(fft PRIVATE TestsBench/CFFTTestsQ15.cpp)

    target_sources(fft PRIVATE TestsBench/CFFT2DTestsF32.cpp)
    target_sources(fft PRIVATE TestsBench/CFFT2DTestsQ31.cpp)
    target_sources(fft PRIVATE TestsBench/CFFT2DTestsQ15.cpp)

    target_sources(fft PRIVATE TestsBench/RFFTTestsF32.cpp)
    target_sources(fft PRIVATE TestsBench/RFFTTestsQ31.cpp)
    target_sources(fft PRIVATE TestsBench/RFFTTestsQ15.cpp)

    if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
        target_sources(fft PRIVATE TestsBench/CFFTTestsF16.cpp)
        target_sources(fft PRIVATE TestsBench/CFFT2DTestsF16.cpp)
        target_sources(fft PRIVATE TestsBench/RFFTTestsF16.cpp)
    endif()
endif()

target_sources(fft PRIVATE TestsBench/FixedPointScaling.cpp)

target_sources(fft PRIVATE Source/fft.cpp
    Generated/twiddle.cpp
    Generated/perm.cpp
    Generated/config.cpp
    )

configLib(fft ${ROOT})
configDsp(fft ${ROOT})


### Includes
target_include_directories(fft PUBLIC "${ROOT}/CMSIS/DSP/Include")
target_include_directories(fft PUBLIC "Include")
target_include_directories(fft PUBLIC "Source")
target_include_directories(fft PUBLIC "Generated")

### Selection of the kernel version according to the architecture
target_include_directories(fft PUBLIC "Source/Kernels")


### Sources and libs

target_link_libraries(fft PRIVATE CMSISDSP)


cmake_minimum_required (VERSION 3.14)
project (kalmanexe VERSION 0.1)


# Define the path to CMSIS-DSP (ROOT is defined on command line when using cmake)
set(DSP ${ROOT}/CMSIS/DSP)

# Add DSP folder to module path
list(APPEND CMAKE_MODULE_PATH ${DSP})

################################### 
#
# LIBRARIES
#
###################################

########### 
#
# CMSIS DSP
#

add_subdirectory(${DSP}/Source bin_dsp)


################################### 
#
# TEST APPLICATION
#
###################################


add_executable(kalmanexe)

# Configure the boot code

target_sources(kalmanexe PRIVATE main.cpp Kalman.cpp IO.cpp data.c)

include(config)
configApp(kalmanexe ${ROOT})

# Include directories.
# You can reuse the command to add more include directories
target_include_directories(kalmanexe PUBLIC ".")


target_link_libraries(kalmanexe PRIVATE CMSISDSP)



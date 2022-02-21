CFLAGS +=  -DFAST -DNODUMP  -D__ARM_FEATURE_DSP=1 -D__ARM_ARCH_8M_MAIN__=1  -D__DSP_PRESENT=1 -DARM_MATH_ARMV8MML -DNO_TBM -fno-vectorize -fno-slp-vectorize


OBJ_FILES   = main.o kiss_fft_helium_fxpt.o kiss_fft.o
LLVM_OBJECTS = main.o kiss_fft_helium_fxpt.o kiss_fft.o

LIBS        = common  arm_math_m55

TOOLS	:= $(shell readlink -f ../../tools/)
include $(TOOLS)/prog.mak

args:
	echo $(CFLAGS) $(LLVM_ARGS)


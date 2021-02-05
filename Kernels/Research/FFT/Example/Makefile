CORE= $(CMSISROOT)/CMSIS/Core
DSP= $(CMSISROOT)/CMSIS/DSP

TOOLCHAIN=/benchresults/ARMCompiler6.15
CC=$(TOOLCHAIN)/bin/armclang 
AR=$(TOOLCHAIN)/bin/armar

VPATH = ../Generated ../Source

INCLUDES = -I$(CORE)/Include -I$(DSP)/Include -I../Generated -I../Source -I../Source/Kernels

# If the core is changed, you need to edit the config.yaml and use the
# Python script to regenerate the configuration.
#CPPFLAGS = -mcpu=cortex-m55+nomve
CPPFLAGS = -mcpu=cortex-m55
CPPFLAGS += --target=arm-arm-none-eabi -fno-rtti -ffast-math
CPPFLAGS += -Ofast -DNDEBUG -Wall -Wextra  -Werror   -mfloat-abi=hard -mlittle-endian 
CPPFLAGS += -mthumb -fshort-enums -fshort-wchar

OBJDIR=obj

SRC = test.cpp fft.cpp config.cpp perm.cpp twiddle.cpp 
OBJ =$(addprefix $(OBJDIR)/, $(SRC:.cpp=.o))

.SUFFIXES: .cpp 


all: libtest.a

clean:
	rm -f $(OBJDIR)/*.o 
	rm -f libtest.a

$(OBJDIR)/%.o: %.cpp
	$(CC) -c $(CPPFLAGS) $(INCLUDES) -o $@ $<

libtest.a: $(OBJ)
	$(AR) -r -s --create libtest.a $(OBJ)


$(shell   mkdir -p $(OBJDIR))
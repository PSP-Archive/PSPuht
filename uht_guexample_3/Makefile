BINOUT = ./bin/
PATHSRC = ./src/
PATHOBJS = $(BINOUT)
TARGET = $(BINOUT)uht_gu

PATHFILES = $(shell ls $(PATHSRC)*.cpp)
OBJS = $(notdir $(patsubst %.cpp, %.o, $(PATHFILES)))
OBJS := $(sort $(OBJS:%.o=$(PATHOBJS)%.o))

CFLAGS = -fsingle-precision-constant -G0 -Wall -O2
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti

CFLAGS += -I$(PSPSDK)/../include \
	-I../utils_lib/include/ -I../uht_lib/include/ \
	-L../uht_lib/lib/ -L../utils_lib/lib/

LIBS += -lpspuht -lutils_mcl -lpspgum_vfpu -lpspgu -lpng -lz -lm \
	-lpspusb -lpsputility -lpspusbcam -lpspjpeg -lpspvfpu \
	-lpsprtc -lpsppower -lstdc++

PSP_EBOOT_SFO = $(BINOUT)PARAM.SFO
EXTRA_TARGETS = $(BINOUT)EBOOT.PBP
PSP_EBOOT = $(EXTRA_TARGETS)
PSP_EBOOT_TITLE = $(notdir $(TARGET))

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

$(PATHOBJS)%.o: $(PATHSRC)%.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS)


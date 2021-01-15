CC       = psp-gcc
AR       = psp-ar
RANLIB   = psp-ranlib

PATHLIBS = ./lib/
PATHSRC = ./src/
PATHOBJS = $(PATHLIBS)

PATHFILES = $(shell ls $(PATHSRC)*.cpp)
OBJS = $(notdir $(patsubst %.cpp, %.o, $(PATHFILES)))
OBJS := $(sort $(OBJS:%.o=$(PATHOBJS)%.o))

OUT = $(PATHLIBS)libutils_mcl.a

CXXFLAGS = -G0 -DPSP -fno-exceptions -fsingle-precision-constant \
		-fno-rtti -I$(PSPSDK)/../include -Wall -O2 -I./include/ \
		-I../uht_lib/include/

PSPSDK = $(shell psp-config --pspsdk-path)

INCDIR   := $(INCDIR) . $(PSPSDK)/include

CXXFLAGS := $(addprefix -I,$(INCDIR)) $(CXXFLAGS)

$(OUT): $(OBJS)
	$(AR) cru $@ $(OBJS)
	$(RANLIB) $@

$(PATHOBJS)%.o: $(PATHSRC)%.cpp
	$(CC) -o $@ -c $< $(CXXFLAGS) $(LIBS)

clean: 
	rm -rf $(PATHOBJS)*.o
	rm -rf $(PATHLIBS)libutils_mcl.a


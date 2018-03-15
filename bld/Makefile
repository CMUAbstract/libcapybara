LIB = libcapybara

include ../Makefile.config
include ../Makefile.options

OBJECTS += \
  capy_board_init.o \
	capybara.o \
	power.o \

ifeq ($(BOARD_MAJOR),2)
$(info major = 2)
else
$(info board major defined as ${BOARD_MAJOR})
endif

ifeq ($(TOOLCHAIN),$(filter $(TOOLCHAIN), gcc coatigcc))
$(info toolchain = gcc)
DEPS += libmspware \
        libfxl \
        libmsp \

CFLAGS += -I$(LIB_ROOT)/libmsp/src/include


else
$(info lib_root = $(LIB_ROOT))
CFLAGS += -I$(LIB_ROOT)/libmspware/src/include \
          -I$(LIB_ROOT)/libfxl/src/include

LFLAGS += -L$(LIB_ROOT)/libmspware/bld/gcc \
          -L$(LIB_ROOT)/libfxl/bld/gcc \

LIBS += -lmspware \
        -lfxl \

endif # TOOLCHAIN


ifneq ($(LIBCAPYBARA_SWITCH_DESIGN),)
OBJECTS += reconfig.o
endif # LIBCAPYBARA_SWITCH_DESIGN

DEPS += \
  libmsp \
  libio\
#  libmspware\
#  libfxl\


ifneq ($(LIBCAPYBARA_VARTH_ENABLED),)
DEPS += libmcppot
endif

override SRC_ROOT = ../../src

override CFLAGS += \
  -DCHAIN \
  -I$(SRC_ROOT)/ \
  -I$(SRC_ROOT)/include \
	-I$(SRC_ROOT)/include/$(LIB) \


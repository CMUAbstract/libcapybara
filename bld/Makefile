LIB = libcapybara

include ../Makefile.config
include ../Makefile.options
include ../Makefile.target

OBJECTS = \
	capybara.o \
	power.o \
  capy_board_init.o \

ifeq ($(TOOLCHAIN),gcc)
$(info toolchain = gcc)
DEPS += libmspware
else
$(info lib_root = $(LIB_ROOT))
CFLAGS += -I$(LIB_ROOT)/libmspware/src/include
LFLAGS += -L$(LIB_ROOT)/libmspware/bld/gcc
LIBS += -lmspware
endif # TOOLCHAIN


ifneq ($(LIBCAPYBARA_SWITCH_DESIGN),)
OBJECTS += reconfig.o
endif # LIBCAPYBARA_SWITCH_DESIGN

DEPS += \
  libmspware\
	libmsp \
  libio\
  libfxl\


ifneq ($(LIBCAPYBARA_VARTH_ENABLED),)
DEPS += libmcppot
endif

override SRC_ROOT = ../../src

override CFLAGS += \
  -DCHAIN \
  -I$(SRC_ROOT)/include \
	-I$(SRC_ROOT)/include/$(LIB) \

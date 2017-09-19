LIB = libcapybara

include ../Makefile.config
include ../Makefile.options

OBJECTS = \
	capybara.o \
	power.o \
  capy_board_init.o \

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

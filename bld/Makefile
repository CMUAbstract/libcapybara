LIB = libcapybara

include ../Makefile.config
include ../Makefile.options

OBJECTS = \
	capybara.o \
	reconfig.o \
	power.o \

DEPS += \
	libmsp \

ifneq ($(LIBCAPYBARA_VARTH_ENABLED),)
DEPS += libmcppot
endif

override SRC_ROOT = ../../src

override CFLAGS += \
	-I$(SRC_ROOT)/include \
	-I$(SRC_ROOT)/include/$(LIB) \

## Copyright (C) 2013-2015 ITRI.

# To build the example outside of the SDK directory please change SDK_DIR
# to the path of the SDK directory

SDK_DIR = /tools/autotalks/sdk-4.11.0-sc-i686-linux-gnu
CFLAGS := -std=c99 -Wall -Wextra -Os -I./include -I./internal_inc
LDFLAGS :=-Wl,--start-group lib/* -Wl,--end-group -lcraton -lm

BOARD := pcb201v1

include $(SDK_DIR)/build/rules.mk
include $(SDK_DIR)/build/craton.mk
include $(SDK_DIR)/build/u-boot.mk

.DEFAULT_GOAL := arm/img/wave-sdk.img

LIBS = $(shell ls lib)

LIB_FILES = $(LIBS:%=lib/%)

TEST_FILES = $(shell ls cmd_*.c)

TEST_OBJ = $(TEST_FILES:%.c=arm/obj/%.o)

arm/bin/wave-sdk: arm/obj/main.o $(TEST_OBJ) $(SDK_DIR)/board/$(BOARD)/arm/lib/ref-sys.o

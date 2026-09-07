SHELL = /usr/bin/env bash

ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

ifneq ($(realpath $(ROOT)/config.mk), $(ROOT)/config.mk)
$(error ERROR: Please run the configure script before building.)
endif

include $(ROOT)/config.mk

#
# User config
#

BUILD_DIR_NAME = build

SOURCE_DIR = src
CORE_DIR = core
HOST_DIR = host

OUTPUT = 2D_Engine
ASSET_OUTPUT = game.data

ifneq ($(DEBUG),)
CFLAGS += -O1 -g -DDEBUG=$(DEBUG) -Wextra -Wall --pedantic -Wno-missing-braces \
	-Wno-missing-field-initializers -Wformat=2 -Wswitch-default -Wswitch \
	-Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 \
	-Wredundant-decls -Wold-style-definition
endif

#
# End
#

# Relocate build dir
BUILD_DIR := $(ROOT)/$(BUILD_DIR_NAME)/$(HOST)

# Verbosity
Q := @
ifeq ($(VERBOSE), 1)
Q :=
endif

# Colors
GREEN  := \033[32m
YELLOW := \033[33m
RESET  := \033[m

# Default targets
all: setup $(BUILD_DIR)/$(OUTPUT)

include $(ROOT)/$(SOURCE_DIR)/Makefile
include $(ROOT)/$(CORE_DIR)/Makefile

setup:
	$(eval export TOTAL_BUILD_FILES := $(shell echo $$(($$(make -n $(OBJECTS) $(CORE_OBJECTS) 2>/dev/null | grep -c $(CC)) + 2))))
	$(Q)mkdir -p $(dir $(OBJECTS) $(CORE_OBJECTS))

.PHONY: clean run print-var-%
clean:
	$(Q)rm -rf $(dir $(BUILD_DIR))

run: all
	$(Q)cd $(BUILD_DIR) && ./$(OUTPUT)

print-var-%  :
	@echo $($*)

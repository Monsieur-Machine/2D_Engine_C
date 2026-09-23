export

SHELL = /usr/bin/env bash

ROOT := $(realpath $(dir $(lastword $(MAKEFILE_LIST))))

ifeq ($(filter clean-all,$(MAKECMDGOALS)),)
ifneq ($(realpath $(ROOT)/config.mk), $(ROOT)/config.mk)
$(error ERROR: Please run the configure script before building)
endif

include $(ROOT)/config.mk
endif

#
# User settings
#

BUILD_DIR = build

SOURCE_DIR = src
CORE_DIR = core
ASSETS_DIR = assets
ASSET_BUILDER_DIR = asset_builder

OUTPUT = 2D_Engine
ASSET_OUTPUT = game.data
ASSET_BUILDER_OUTPUT = asset_builder/asset_builder

ifneq ($(DEBUG),)
TARGET_HOST_CFLAGS += -O1 -g -DDEBUG=$(DEBUG) -Wextra -Wall --pedantic -Wno-missing-braces \
	-Wno-missing-field-initializers -Wformat=2 -Wswitch-default -Wswitch \
	-Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 \
	-Wredundant-decls -Wold-style-definition

ASSET_BUILDER_CFLAGS += -O1 -g -DDEBUG=$(DEBUG) -Wextra -Wall --pedantic -Wno-missing-braces \
	-Wno-missing-field-initializers -Wformat=2 -Wswitch-default -Wswitch \
	-Wcast-align -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs -Wcast-qual \
	-Wshadow -Wunreachable-code -Wlogical-op -Wfloat-equal -Wstrict-aliasing=2 \
	-Wredundant-decls -Wold-style-definition
endif

#
# End
#

# Relocate vars
BUILD_DIR := $(ROOT)/$(BUILD_DIR)/$(TARGET_HOST)

SOURCE_DIR := $(ROOT)/$(SOURCE_DIR)
CORE_DIR := $(ROOT)/$(CORE_DIR)
ASSETS_DIR := $(ROOT)/$(ASSETS_DIR)
ASSET_BUILDER_DIR := $(ROOT)/$(ASSET_BUILDER_DIR)

OUTPUT := $(BUILD_DIR)/$(OUTPUT)
LIBCORE := $(BUILD_DIR)/$(LIBCORE)
ASSET_OUTPUT := $(BUILD_DIR)/$(ASSET_OUTPUT)
ASSET_BUILDER_OUTPUT := $(BUILD_DIR)/$(ASSET_BUILDER_OUTPUT)

# Verbosity
export Q := @
ifeq ($(VERBOSE), 1)
Q :=
endif

# Colors
GREEN  := \033[32m
YELLOW := \033[33m
RESET  := \033[m

# Default targets
all: $(OUTPUT) $(ASSET_OUTPUT)

$(OUTPUT): FORCE
	$(Q)cd $(SOURCE_DIR) && $(MAKE) $@

$(ASSET_OUTPUT): FORCE
	$(Q)cd $(ASSETS_DIR) && $(MAKE) $@

FORCE:

.PHONY: clean clean-all run print-var-%
clean:
	$(Q)rm -rf $(BUILD_DIR)

clean-all:
	$(Q)rm -rf $(dir $(BUILD_DIR))

run: all
	$(Q)cd $(BUILD_DIR) && ./$(notdir $(OUTPUT))

print-var-% :
	@echo $($*)

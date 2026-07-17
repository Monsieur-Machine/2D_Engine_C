SHELL = /usr/bin/env bash

# Build folder
BUILD_DIR = build

# Source output target name
SOURCE_OUTPUT := 2D_Engine

# Assets output
ASSET_OUTPUT := game.data

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
all: build_core build_src compress_assets

# Sub Makefiles
SOURCE_DIR = src
CORE_DIR = core/src
ASSETS_DIR = assets

include $(CORE_DIR)/Makefile
include $(SOURCE_DIR)/Makefile
include $(ASSETS_DIR)/Makefile

# Clean
.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "Build dir cleaned"

# Run executable
.PHONY: run
run: all
	$(Q)./$(BUILD_DIR)/$(SOURCE_OUTPUT)

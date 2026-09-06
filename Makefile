SHELL = /usr/bin/env bash

# Build folder
BUILD_DIR = build

# Outputs target name and path
SOURCE_OUTPUT = $(BUILD_DIR)/2D_Engine
CORE_OUTPUT = $(BUILD_DIR)/libcore.a
ASSET_OUTPUT = $(BUILD_DIR)/game.data

# Verbosity
Q := @
ifeq ($(VERBOSE), 1)
Q :=
endif

# Colors
export GREEN  := \033[32m
export YELLOW := \033[33m
export RESET  := \033[m

# Default targets
all: $(SOURCE_OUTPUT)

# Sub Makefiles
SOURCE_DIR = src
CORE_DIR = core/src
ASSETS_DIR = assets

include $(SOURCE_DIR)/Makefile
include $(CORE_DIR)/Makefile
include $(ASSETS_DIR)/Makefile

# Clean
.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "Build dir cleaned"

# Run executable
.PHONY: run
run: all
	$(Q)cd $(BUILD_DIR) && ./$(notdir $(SOURCE_OUTPUT))

.PHONY: print-%
print-%  :
	@echo $($*)

SHELL = /bin/bash

# Build folder
BUILD_DIR = build

# Source output target name
SOURCE_OUTPUT := 2D_Engine

# Assets output
ASSETS_OUTPUT := game.data

# Verbosity
Q := @
ifeq ($(VERBOSE), 1)
Q :=
endif

# Colors
GREEN  := \033[0;32m
YELLOW := \033[1;33m
RESET  := \033[0m

# Default targets
#all: build_core build_src compress_assets end
all: build_src

# Current file nb to process
CURRENT_FILE := 0

# Sub-Makefiles
SOURCE_DIR = src
CORE_DIR = core
ASSETS_DIR = assets

include $(CORE_DIR)/Makefile
include $(SOURCE_DIR)/Makefile
include $(ASSETS_DIR)/Makefile

count_build:
	$(eval export TOTAL_FILES := $(shell echo $$(($$(make -n $(OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Print ending message
end:
	@echo "Built target $(OUTPUT)"

# Clean
.PHONY: clean
clean:
	$(Q)rm -rf $(BUILD_DIR)
	@echo "Build dir cleaned"

# Run executable
.PHONY: run
run:
	$(Q)./$(OUTPUT)


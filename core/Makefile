# Source files
CORE_SOURCES := \
  vector2d.c \
  linked_list.c \
  display.c

# Compiler and flags
CORE_CC = gcc
CORE_CFLAGS = $(sdl2-config --cflags --libs) -Wall -Wextra -std=c17 -fPIC
CORE_INCLUDE_DIRS = $(CORE_DIR)/headers
CORE_LDFLAGS =

# Deduce objects
CORE_OBJECTS = $(CORE_SOURCES:%.c=$(BUILD_DIR)/$(CORE_DIR)/%.o)

# Current file nb to process
CURRENT_CORE_FILE := 0

# Build core target
build_core: core_setup count_core_build $(BUILD_DIR)/$(CORE_OUTPUT)

# Core build directories
core_setup:
	@echo "Building Core (1/3)"
	$(Q)mkdir -p $(dir $(CORE_OBJECTS))

# Count
count_core_build:
	$(eval export TOTAL_CORE_FILES := $(shell echo $$(($$(make -n $(CORE_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link core target
$(BUILD_DIR)/$(CORE_OUTPUT): $(CORE_OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(CORE_OUTPUT)$(RESET)"
	$(Q)$(CORE_CC) -shared $(CORE_LDFLAGS) -o $@ $(CORE_OBJECTS)

# Build .o files
$(BUILD_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.c
	$(eval CURRENT_CORE_FILE := $(shell echo $$(($(CURRENT_CORE_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_CORE_FILE)*100/$(TOTAL_CORE_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(CORE_CC) $(CORE_CFLAGS) $(CORE_INCLUDE_DIRS:%=-I%) -MMD -MP -c $< -o $@

# Source files dependencies
-include $(SOURCE_OBJECTS:.o=.d)

# Source files
SOURCES := \
  main.c \
  game.c \
  texture_manager.c \
  ecs/ecs.c \
  ecs/components/transform_component.c \
  ecs/components/sprite_component.c \
  ecs/components/animation_system.c \
  ecs/components/player_system.c

# Compiler and flags
SRC_CC = gcc
SRC_CFLAGS = -Wall -Wextra -std=c17
SRC_INCLUDE_DIRS = $(SOURCE_DIR)/headers $(SOURCE_DIR)/ecs/headers $(SOURCE_DIR)/ecs/components/headers $(CORE_DIR)/headers
SRC_LDFLAGS =

# Deduce objects
SOURCE_OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/$(SOURCE_DIR)/%.o)

# Current file nb to process
CURRENT_SRC_FILE := 0

# Build src target
build_src: src_setup count_src_build $(BUILD_DIR)/$(SOURCE_OUTPUT)

# Source build directories and 
src_setup:
	@echo "Building Source (2/3)"
	$(Q)mkdir -p $(dir $(SOURCE_OBJECTS))

# Count
count_src_build:
	$(eval export TOTAL_SRC_FILES := $(shell echo $$(($$(make -n $(SOURCE_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link game target
$(BUILD_DIR)/$(SOURCE_OUTPUT): $(SOURCE_OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(SOURCE_OUTPUT)$(RESET)"
	$(Q)$(SRC_CC) $(SRC_LDFLAGS) -o $@ $(SOURCE_OBJECTS)

# Build .o files
$(BUILD_DIR)/$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(eval CURRENT_SRC_FILE := $(shell echo $$(($(CURRENT_SRC_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_SRC_FILE)*100/$(TOTAL_SRC_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(SRC_CC) $(SRC_CFLAGS) $(SRC_INCLUDE_DIRS:%=-I%) -MMD -MP -c $< -o $@

# Source files dependencies
-include $(SOURCE_OBJECTS:.o=.d)

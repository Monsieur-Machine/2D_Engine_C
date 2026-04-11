# Source files
SOURCES := \
  main.c \
  game.c \
  vector2d.c \
  linked_list.c \
  texture_manager.c \
  map_manager.c \
  ecs/ecs.c \
  ecs/components/transform_component.c \
  ecs/components/sprite_component.c \
  ecs/components/animation_system.c \
  ecs/components/player_system.c

# Compiler and flags
CC = ccache gcc
CFLAGS = -Wall -Wextra -std=c17
INCLUDE_DIRS = $(SOURCE_DIR)/headers $(SOURCE_DIR)/ecs/headers $(SOURCE_DIR)/ecs/components/headers $(CORE_DIR)/src/headers
LDFLAGS =

# Deduce objects
SOURCE_OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/$(SOURCE_DIR)/%.o)

# Build src target
build_src: src_build_dir count_src_build $(BUILD_DIR)/$(SOURCE_OUTPUT)

# Source build directories
src_build_dir:
	echo $(INCLUDE_DIRS)
	$(Q)mkdir -p $(dir $(SOURCE_OBJECTS))

# Count
count_src_build:
	$(eval export TOTAL_FILES := $(shell echo $$(($$(make -n $(SOURCE_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link game target
$(BUILD_DIR)/$(SOURCE_OUTPUT): $(SOURCE_OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(OUTPUT)$(RESET)"
	$(Q)$(CC) -Wl,-r $(LDFLAGS) -o $@ $(SOURCE_OBJECTS)

# Build .o files
$(BUILD_DIR)/$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(CC) $(CFLAGS) $(INCLUDE_DIRS:%=-I%) -MMD -MP -c $< -o $@

# Source files dependencies
-include $(SOURCE_OBJECTS:.o=.d)

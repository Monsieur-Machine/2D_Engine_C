# Source files
SOURCES := \
  game.c \
  components/transform_component.c \
  components/sprite_component.c \
  components/hitbox_component.c \
  components/physics_system.c \
  components/interact_component.c \
  components/animation_system.c \
  components/player_system.c \
  components/lever_component.c

# Compiler and flags
SRC_CC = gcc
ifeq ($(RELEASE), 1)
SRC_CFLAGS = --std=c17 --pedantic -O3 # RELEASE
else
SRC_CFLAGS = --std=c17 --pedantic -O0 -g -DDEBUG=2 \
	-Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers \
    -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
    -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs \
    -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
    -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
    -Wold-style-definition # DEVELOPEMENT
endif

SRC_INCLUDE_DIRS = $(SOURCE_DIR)/components/headers $(CORE_DIR)/headers
SRC_INCLUDE_DIRS := $(addprefix -I, $(SRC_INCLUDE_DIRS))

SRC_LDFLAGS =

# Deduce objects
SOURCE_OBJECTS = $(SOURCES:%.c=$(BUILD_DIR)/$(SOURCE_DIR)/%.o)

# Current file nb to process
CURRENT_SRC_FILE := 0

# Build src target
build_src: src_setup count_src_build src_end

# Source build directories
src_setup:
	@echo "Building $(notdir $(SOURCE_OUTPUT))"
	$(Q)mkdir -p $(dir $(SOURCE_OBJECTS))

# Count nb of files to build
count_src_build:
	$(eval export TOTAL_SRC_FILES := $(shell echo $$(($$(make -n $(SOURCE_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link game target
$(SOURCE_OUTPUT): $(SOURCE_OBJECTS) $(CORE_OUTPUT)
	@echo -e "[100%] $(YELLOW)Linking $(notdir $(SOURCE_OUTPUT))$(RESET)"
	$(Q)$(SRC_CC) $(SOURCE_OBJECTS) -L./$(BUILD_DIR) $(CORE_OUTPUT) -o $@ $(SRC_LDFLAGS) $(CORE_DEPS_FLAGS)

# Build .o files
$(BUILD_DIR)/$(SOURCE_DIR)/%.o: $(SOURCE_DIR)/%.c
	$(eval CURRENT_SRC_FILE := $(shell echo $$(($(CURRENT_SRC_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_SRC_FILE)*100/$(TOTAL_SRC_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(SRC_CC) $(SRC_CFLAGS) $(SRC_INCLUDE_DIRS) -MMD -MP -c $< -o $@

# Print ending message
src_end: $(SOURCE_OUTPUT)
	@echo "Built target $(notdir $(SOURCE_OUTPUT))"

# Source files dependencies
-include $(SOURCE_OBJECTS:.o=.d)

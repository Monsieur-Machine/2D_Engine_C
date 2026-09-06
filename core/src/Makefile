# Source files
CORE_SOURCES := \
  linked_list.c \
  vector2d.c \
  inputs.c \
  asset_manager.c \
  texture.c \
  map.c \
  camera.c \
  event_bus.c \
  ecs.c \
  display.c \
  main.c

# Compiler and flags
CORE_CC = gcc
ifeq ($(RELEASE), 1)
CORE_CFLAGS = --std=c17 --pedantic -O3 -DASSET_FILE_NAME=\"$(notdir $(ASSET_OUTPUT))\" -D_GNU_SOURCE=1 $(shell pkg-config --cflags sdl3) # RELEASE
else
CORE_CFLAGS = --std=c17 --pedantic -O1 -g -DDEBUG=2 -DASSET_FILE_NAME=\"$(notdir $(ASSET_OUTPUT))\" -D_GNU_SOURCE=1 $(shell pkg-config --cflags sdl3) \
	-Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers \
    -Wformat=2 -Wswitch-default -Wswitch -Wcast-align \
    -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs \
    -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
    -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
    -Wold-style-definition # DEVELOPEMENT
endif

CORE_INCLUDE_DIRS = $(SOURCE_DIR)/components $(CORE_DIR)
CORE_INCLUDE_DIRS := $(addprefix -I, $(CORE_INCLUDE_DIRS))

CORE_AR = ar
CORE_ARFLAGS =

# Used in final linking.
CORE_DEPS_FLAGS = $(shell pkg-config --libs sdl3) -lm

# Deduce objects
CORE_OBJECTS = $(CORE_SOURCES:%.c=$(BUILD_DIR)/$(CORE_DIR)/%.o)

# Current file nb to process
CURRENT_CORE_FILE := 0

# Build core target
build_core: core_setup count_core_build core_end

# Core build directories
core_setup:
	@echo "Building $(notdir $(CORE_OUTPUT))"
	$(Q)mkdir -p $(dir $(CORE_OBJECTS))

# Count nb of files to build
count_core_build:
	$(eval export TOTAL_CORE_FILES := $(shell echo $$(($$(make -n $(CORE_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link core target
$(CORE_OUTPUT): $(CORE_OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(notdir $(CORE_OUTPUT))$(RESET)"
	$(Q)$(CORE_AR) rcs $(CORE_ARFLAGS) $@ $^

# Build .o files
$(BUILD_DIR)/$(CORE_DIR)/%.o: $(CORE_DIR)/%.c
	$(eval CURRENT_CORE_FILE := $(shell echo $$(($(CURRENT_CORE_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_CORE_FILE)*100/$(TOTAL_CORE_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(CORE_CC) $(CORE_CFLAGS) $(CORE_INCLUDE_DIRS) -MMD -MP -c $< -o $@

# Print ending message
core_end: $(CORE_OUTPUT)
	@echo "Built target $(notdir $(CORE_OUTPUT))"

# Source files dependencies
-include $(CORE_OBJECTS:.o=.d)

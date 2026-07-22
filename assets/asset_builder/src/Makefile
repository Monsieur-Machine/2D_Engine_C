# Source files
ASSET_BUILDER_SOURCES := \
  main.c \
  asset_file.c \
  errors.c

# Compiler and flags
ASSET_BUILDER_CC = gcc
ifeq ($(RELEASE), 1)
ASSET_BUILDER_CFLAGS = --std=c17 --pedantic -O3 $(shell pkg-config --cflags libpng) # RELEASE
else
ASSET_BUILDER_CFLAGS = --std=c17 --pedantic -O1 -g $(shell pkg-config --cflags libpng) \
	-Wall -Wno-missing-braces -Wextra -Wno-missing-field-initializers \
    -Wformat=2 -Wswitch-default -Wswitch-enum -Wcast-align \
    -Wpointer-arith -Wbad-function-cast -Wstrict-overflow=5 \
    -Wstrict-prototypes -Winline -Wundef -Wnested-externs \
    -Wcast-qual -Wshadow -Wunreachable-code -Wlogical-op \
    -Wfloat-equal -Wstrict-aliasing=2 -Wredundant-decls \
    -Wold-style-definition # DEVELOPEMENT
endif

ASSET_BUILDER_INCLUDE_DIRS = $(ASSET_BUILDER_DIR)/headers
ASSET_BUILDER_INCLUDE_DIRS := $(addprefix -I, $(ASSET_BUILDER_INCLUDE_DIRS))

ASSET_BUILDER_LDFLAGS = $(shell pkg-config --libs libpng)

# Deduce objects
ASSET_BUILDER_OBJECTS = $(ASSET_BUILDER_SOURCES:%.c=$(BUILD_DIR)/$(ASSET_BUILDER_DIR)/%.o)

# Current file nb to process
CURRENT_ASSET_BUILDER_FILE := 0

# Build asset builder target
build_asset_builder: asset_builder_setup count_asset_builder_build asset_builder_end

# Source build directories
asset_builder_setup:
	@echo "Building $(notdir $(ASSET_BUILDER_OUTPUT))"
	$(Q)mkdir -p $(dir $(ASSET_BUILDER_OBJECTS))

# Count nb of files to build
count_asset_builder_build:
	$(eval export TOTAL_ASSET_BUILDER_FILES := $(shell echo $$(($$(make -n $(ASSET_BUILDER_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

# Link asset builder target
$(ASSET_BUILDER_OUTPUT): $(ASSET_BUILDER_OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(notdir $(ASSET_BUILDER_OUTPUT))$(RESET)"
	$(Q)$(SRC_CC) $(ASSET_BUILDER_OBJECTS) -o $@ $(ASSET_BUILDER_LDFLAGS)

# Build .o files
$(BUILD_DIR)/$(ASSET_BUILDER_DIR)/%.o: $(ASSET_BUILDER_DIR)/%.c
	$(eval CURRENT_ASSET_BUILDER_FILE := $(shell echo $$(($(CURRENT_ASSET_BUILDER_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_ASSET_BUILDER_FILE)*100/$(TOTAL_ASSET_BUILDER_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(SRC_CC) $(ASSET_BUILDER_CFLAGS) $(ASSET_BUILDER_INCLUDE_DIRS) -MMD -MP -c $< -o $@

# Print ending message
asset_builder_end: $(ASSET_BUILDER_OUTPUT)
	@echo "Built target $(notdir $(ASSET_BUILDER_OUTPUT))"

# Source files dependencies
-include $(ASSET_BUILDER_OBJECTS:.o=.d)

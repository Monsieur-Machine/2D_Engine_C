ASSETS := \
  player_sheets/player_idle_sheet.png \
  player_sheets/player_walk_sheet.png \
  player_sheets/player_run_sheet.png \
  props_sheets/lever_sheet.png \
  props_sheets/barred_door_sheet.png \
  props_sheets/wooden_door_sheet.png \
  tileset/tileset.png \
  maps/dungeon_entry.map \
  maps/dungeon_room_1.map

ASSETS := $(addprefix $(ASSETS_DIR)/,$(ASSETS))

ASSETS_OBJECTS = $(ASSETS:%=$(BUILD_DIR)/%.o)

ASSET_BUILDER_DIR = $(ASSETS_DIR)/asset_builder/src
ASSET_BUILDER_OUTPUT = $(BUILD_DIR)/$(ASSETS_DIR)/asset_builder/asset_builder
include $(ASSET_BUILDER_DIR)/Makefile

build_assets: setup_asset build_asset_builder count_assets_build asset_end

# Current file nb to process
CURRENT_ASSET_FILE := 0

# Count nb of files to build
count_assets_build:
	$(eval export TOTAL_ASSETS_FILES := $(shell echo $$(($$(make -n $(ASSETS_OBJECTS) 2>/dev/null | grep -c "Building") + 1))))

setup_asset:
	@echo "Building $(notdir $(ASSET_OUTPUT))"
	$(Q)mkdir -p $(dir $(ASSETS_OBJECTS))

$(ASSET_OUTPUT): $(ASSETS_OBJECTS) $(ASSET_BUILDER_OUTPUT)
	@echo -e "[100%] $(YELLOW)Building $(notdir $(ASSET_OUTPUT))$(RESET)"
	$(Q)$(ASSET_BUILDER_OUTPUT) $@ $(ASSETS_OBJECTS)

# Build .o files
$(BUILD_DIR)/$(ASSETS_DIR)/%.o: $(ASSETS_DIR)/% $(ASSET_BUILDER_OUTPUT)
	$(eval CURRENT_ASSET_FILE := $(shell echo $$(($(CURRENT_ASSET_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_ASSET_FILE)*100/$(TOTAL_ASSETS_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building ASSET object $@$(RESET)"
	$(Q)$(ASSET_BUILDER_OUTPUT) $@ -c $<

asset_end: $(ASSET_OUTPUT)
	@echo "Built target $(notdir $(ASSET_OUTPUT))"

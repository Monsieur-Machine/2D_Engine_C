ASSETS := \
  player_sheets/player_idle_sheet.png \
  player_sheets/player_walk_sheet.png \
  player_sheets/player_run_sheet.png \
  props_sheets/lever_sheet.png \
  props_sheets/barred_door_sheet.png \
  props_sheets/wooden_door_sheet.png \
  tileset/tileset.png \
  maps/dungeon_room_1.map

ASSETS := $(addprefix $(ASSETS_DIR)/,$(ASSETS))

ASSET_BUILDER_DIR = $(ASSETS_DIR)/asset_builder/src

ASSET_BUILDER_OUTPUT = $(BUILD_DIR)/$(ASSETS_DIR)/asset_builder/asset_builder

build_assets: setup_asset build_asset_builder asset_end

include $(ASSET_BUILDER_DIR)/Makefile

setup_asset:
	@echo "Building $(notdir $(ASSET_OUTPUT))"

$(ASSET_OUTPUT): $(ASSET_BUILDER_OUTPUT) $(ASSETS)
	@echo -e "[100%] $(YELLOW)Building $(notdir $(ASSET_OUTPUT))$(RESET)"
	$(Q)$(ASSET_BUILDER_OUTPUT) $@ $(ASSETS)

asset_end: $(ASSET_OUTPUT)
	@echo "Built target $(notdir $(ASSET_OUTPUT))"

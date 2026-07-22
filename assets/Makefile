ASSETS := \
  player_sheets/player_idle_sheet.png \
  player_sheets/player_walk_sheet.png \
  tileset/tileset.png

ASSETS := $(addprefix $(ASSETS_DIR)/,$(ASSETS))

ASSET_BUILDER_DIR = $(ASSETS_DIR)/asset_builder/src

ASSET_BUILDER_OUTPUT = $(BUILD_DIR)/$(ASSETS_DIR)/asset_builder/asset_builder

build_assets: setup_asset build_asset_builder asset_end

include $(ASSET_BUILDER_DIR)/Makefile

setup_asset:
	@echo "Building $(notdir $(ASSET_OUTPUT))"

$(ASSET_OUTPUT): $(ASSET_BUILDER_OUTPUT)
	$(Q)$^ $@ $(ASSETS)

asset_end: $(ASSET_OUTPUT)
	@echo "Built target $(notdir $(ASSET_OUTPUT))"

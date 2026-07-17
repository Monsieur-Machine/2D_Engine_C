ASSETS := \
  player_sheets/player_idle_sheet.png \
  player_sheets/player_walk_sheet.png

ASSETS := $(ASSETS:%=$(ASSETS_DIR)/%)

ASSET_BUILDER_DIR = $(ASSETS_DIR)/asset_builder/src

ASSET_BUILDER_OUTPUT = asset_builder

build_assets: setup_asset asset_end

include $(ASSET_BUILDER_DIR)/Makefile

setup_asset:
	@echo "Building $(ASSET_OUTPUT)"
	$(Q)mkdir -p $(dir $(ASSETS))

$(BUILD_DIR)/$(ASSET_OUTPUT): build_asset_builder
	$(Q)$(BUILD_DIR)/$(ASSET_BUILDER_DIR)/$(ASSET_BUILDER_OUTPUT) $@ $(ASSETS)

asset_end: $(BUILD_DIR)/$(ASSET_OUTPUT)
	@echo "Built target $(ASSET_OUTPUT)"

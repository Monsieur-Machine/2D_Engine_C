# Assets files
ASSETS := \
  assets/tileset/tileset.png \
  assets/player_sheets/player_idle_sheet.png \
  assets/player_sheets/player_run_sheet.png \
  assets/player_sheets/player_death_sheet.png \
  assets/enemy_sheets/enemy_idle_sheet.png \
  assets/enemy_sheets/enemy_run_sheet.png \
  assets/enemy_sheets/enemy_death_sheet.png \
  assets/maps/map_test.json

# Deduce assets lz4
ASSETS_COMPRESSED = $(ASSETS:%=$(BUILD_DIR)/%.lz4)

# Current file nb to process
CURRENT_ASSET_FILE := 0

# Build asset target
compress_assets: asset_setup count_assets_compress $(BUILD_DIR)/$(ASSET_OUTPUT) assets_end

# Asset compressing message
asset_setup:
	@echo "Compressing Assets (3/3)"
	$(Q)mkdir -p $(dir $(ASSETS_COMPRESSED))

# Count nb of assets to compress
count_assets_compress:
	$(eval export TOTAL_ASSETS_FILES := $(shell echo $$(($$(make -n $(ASSETS_COMPRESSED) 2>/dev/null | grep -c "Compressing") + 1))))

# Compress assets into tar file
$(BUILD_DIR)/$(ASSET_OUTPUT): $(ASSETS_COMPRESSED)
	@echo -e "[100%] $(YELLOW)Compressing $(ASSET_OUTPUT)$(RESET)"
	$(Q)tar -cf $@ $(ASSETS_COMPRESSED)

# Compress individual assets
$(BUILD_DIR)/%.lz4: %
	$(eval CURRENT_ASSET_FILE := $(shell echo $$(($(CURRENT_ASSET_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_ASSET_FILE) * 100 / $(TOTAL_ASSETS_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Compressing asset $@$(RESET)"
	$(Q)lz4 --favor-decSpeed -fqtz $< $@

# Print ending message
assets_end: $(BUILD_DIR)/$(SOURCE_OUTPUT)
	@echo "Compressed target $(ASSET_OUTPUT)"

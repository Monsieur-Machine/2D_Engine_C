# Assets files
ASSETS := \
  tileset/tileset.png \
  player_sheets/player_idle_sheet.png \
  player_sheets/player_run_sheet.png \
  player_sheets/player_death_sheet.png \
  enemy_sheets/enemy_idle_sheet.png \
  enemy_sheets/enemy_run_sheet.png \
  enemy_sheets/enemy_death_sheet.png \
  maps/map_test.json

# Build asset target
compress_assets: asset_setup $(BUILD_DIR)/$(ASSET_OUTPUT) assets_end

# Asset compressing message
asset_setup:
	@echo "Compressing Assets (3/3)"
	$(Q)mkdir -p $(BUILD_DIR)/$(ASSETS_DIR)

# Compress assets into tar file
$(BUILD_DIR)/$(ASSET_OUTPUT): $(ASSETS:%=$(ASSETS_DIR)/%)
	@echo -e "[100%] $(YELLOW)Compressing $(ASSET_OUTPUT)$(RESET)"
	$(Q)tar -cf $@ $^

# Print ending message
assets_end: $(BUILD_DIR)/$(SOURCE_OUTPUT)
	@echo "Compressed target $(ASSET_OUTPUT)"

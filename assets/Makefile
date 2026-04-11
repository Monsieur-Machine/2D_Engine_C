# Assets files
ASSETS := \
  assets/tileset/tileset.png \
  assets/player-sheets/player_idle_sheet.png \
  assets/player-sheets/player_walk_sheet.png \
  assets/player-sheets/player_run_sheet.png \
  assets/props-sheets/barred_door_sheet.png \
  assets/props-sheets/lever_sheet.png \
  assets/props-sheets/small_chest_sheet.png \
  assets/props-sheets/big_chest_sheet.png \
  assets/maps/map_test.json

# Deduce assets lz4
ASSETS_COMPRESSED = $(ASSETS:%=$(BUILD_DIR)/%.lz4)

# Compress assets into tar file
$(BUILD_DIR)/$(ASSETS_OUTPUT): $(ASSETS_COMPRESSED)
	@echo "[100%] $(YELLOW)Compressing $(ASSETS_OUTPUT)$(RESET)"
	$(Q)tar -cvf $@ $()

# Compress individual assets
$(BUILD_DIR)/%.lz4: %
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_FILES)))))
	@echo "[$(PERCENTAGE)%] $(GREEN)Compressing asset $@$(RESET)"
	$(Q)lz4 $< $@

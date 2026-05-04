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

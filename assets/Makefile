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

LUA_SOURCES := \
  enemy_system.lua

OBJECTS := $(ASSETS:%=$(BUILD_DIR)/$(notdir $(ASSETS_DIR))/%.o)

ifeq ($(TOTAL_BUILD_FILES),)
TOTAL_BUILD_FILES := $(shell echo $$(($$($(MAKE) -n $(OBJECTS) TOTAL_BUILD_FILES=1 2>/dev/null | grep -c Building) + 1)))
endif

$(ASSET_OUTPUT): $(OBJECTS) $(ASSET_BUILDER_OUTPUT)
	@echo -e "[100%] $(YELLOW)Building $(notdir $(ASSET_OUTPUT))$(RESET)"
	$(Q)$(ASSET_BUILDER_OUTPUT) $@ $(OBJECTS)

$(BUILD_DIR)/$(notdir $(ASSETS_DIR))/%.o: $(ASSETS_DIR)/% $(ASSET_BUILDER_OUTPUT)
	$(Q)mkdir -p $(dir $@)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building ASSET object $@$(RESET)"
	$(Q)$(ASSET_BUILDER_OUTPUT) $@ -c $<

$(ASSET_BUILDER_OUTPUT): FORCE
	$(Q)cd $(ASSET_BUILDER_DIR) && $(MAKE) $@

FORCE:

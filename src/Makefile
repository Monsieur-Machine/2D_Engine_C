SOURCES += \
  game.c \
  components/transform_component.c \
  components/sprite_component.c \
  components/hitbox_component.c \
  components/physics_system.c \
  components/interact_component.c \
  components/animation_system.c \
  components/player_system.c \
  components/lever_component.c \
  components/door_component.c# \
  components/lua_component.c

INCLUDES += \
  $(SOURCE_DIR)/components \
  $(CORE_DIR) \
  $(CORE_DIR)/host/$(TARGET_HOST)

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/$(notdir $(SOURCE_DIR))/%.o)

ifeq ($(TOTAL_BUILD_FILES),)
TOTAL_BUILD_FILES := $(shell echo $$(($$($(MAKE) -n $(OBJECTS) TOTAL_BUILD_FILES=1 2>/dev/null | grep -c Building) + 1)))
endif

$(OUTPUT): $(OBJECTS) $(LIBCORE)
	@echo "[100%] $(YELLOW)Linking $(notdir $(OUTPUT))$(RESET)"
	$(Q)$(TARGET_HOST_CC) $(TARGET_HOST_CFLAGS) $(OBJECTS) -o $@ -L$(BUILD_DIR) -lcore $(TARGET_HOST_LDFLAGS)

$(BUILD_DIR)/$(notdir $(SOURCE_DIR))/%.o: $(SOURCE_DIR)/%
	$(Q)mkdir -p $(dir $@)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(TARGET_HOST_CC) $(TARGET_HOST_CFLAGS) $(INCLUDES:%=-I%) -MMD -MP -c $< -o $@

$(LIBCORE): FORCE
	$(Q)cd $(CORE_DIR) && $(MAKE) $@

FORCE:

-include $(OBJECTS:.o=.d)

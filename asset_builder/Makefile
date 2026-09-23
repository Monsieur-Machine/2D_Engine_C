SOURCES := \
  object.c \
  image.c \
  map.c \
  utils.c \
  main.c

INCLUDES += \
  $(ASSET_BUILDER_DIR) \
  $(SOURCE_DIR)/components

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/$(notdir $(ASSET_BUILDER_DIR))/%.o)

ifeq ($(TOTAL_BUILD_FILES),)
TOTAL_BUILD_FILES := $(shell echo $$(($$($(MAKE) -n $(OBJECTS) TOTAL_BUILD_FILES=1 2>/dev/null | grep -c Building) + 1)))
endif

$(ASSET_BUILDER_OUTPUT): $(OBJECTS)
	$(Q)mkdir -p $(dir $@)
	@echo "[100%] $(YELLOW)Linking $(notdir $(ASSET_BUILDER_OUTPUT))$(RESET)"
	$(Q)$(ASSET_BUILDER_CC) $(ASSET_BUILDER_CFLAGS) $(OBJECTS) -o $@  $(ASSET_BUILDER_LDFLAGS)

$(BUILD_DIR)/$(notdir $(ASSET_BUILDER_DIR))/%.o: $(ASSET_BUILDER_DIR)/%
	$(Q)mkdir -p $(dir $@)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE)+1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE)*100/$(TOTAL_BUILD_FILES)))))
	@echo "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(ASSET_BUILDER_CC) $(ASSET_BUILDER_CFLAGS) $(INCLUDES:%=-I%) -MMD -MP -c $< -o $@

-include $(ASSET_BUILDER_OBJECTS:.o=.d)

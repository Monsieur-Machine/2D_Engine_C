SOURCES += \
  main.c \
  camera.c \
  linked_list.c \
  event_bus.c \
  ecs.c \
  asset_manager.c \
  inputs.c \
  map.c

INCLUDES += \
  $(CORE_DIR) \
  $(CORE_DIR)/host/$(TARGET_HOST) \
  $(SOURCE_DIR)/components

include $(CORE_DIR)/host/$(TARGET_HOST)/Makefile

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/$(notdir $(CORE_DIR))/%.o)

ifeq ($(TOTAL_BUILD_FILES),)
TOTAL_BUILD_FILES := $(shell echo $$(($$($(MAKE) -n $(OBJECTS) TOTAL_BUILD_FILES=1 2>/dev/null | grep -c Building) + 1)))
endif

$(LIBCORE): $(OBJECTS)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo "[$(PERCENTAGE)%] $(YELLOW)Creating Static Lib $@$(RESET)"
	$(Q)$(TARGET_HOST_AR) rcs $@ $(OBJECTS)

$(BUILD_DIR)/$(notdir $(CORE_DIR))/%.o: $(CORE_DIR)/%
	$(Q)mkdir -p $(dir $@)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(TARGET_HOST_CC) $(TARGET_HOST_CFLAGS) $(INCLUDES:%=-I%) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

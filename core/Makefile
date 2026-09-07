CORE_SOURCES += \
  main.c \
  camera.c \
  linked_list.c \
  event_bus.c \
  ecs.c \
  asset_manager.c \
  inputs.c \
  map.c

CORE_INCLUDES += \
  $(CORE_DIR) \
  $(CORE_DIR)/$(HOST_DIR)/$(HOST) \
  $(SOURCE_DIR)/components

include $(ROOT)/$(CORE_DIR)/$(HOST_DIR)/$(HOST)/Makefile
CORE_OBJECTS := $(CORE_SOURCES:%=$(BUILD_DIR)/$(CORE_DIR)/%.o)

$(BUILD_DIR)/$(LIBCORE): $(CORE_OBJECTS)
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(YELLOW)Creating Static Lib $@$(RESET)"
	$(Q)$(AR) rcs $@ $(CORE_OBJECTS)

$(BUILD_DIR)/$(CORE_DIR)/%.c.o: $(ROOT)/$(CORE_DIR)/%.c
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(CC) $(CFLAGS) $(CORE_INCLUDES:%=-I%) -MMD -MP -c $< -o $@

-include $(CORE_OBJECTS:.o=.d)

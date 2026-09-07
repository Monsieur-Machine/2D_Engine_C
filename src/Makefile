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
  components/door_component.c \
  components/lua_component.c

INCLUDES += \
  $(SOURCE_DIR)/components \
  $(CORE_DIR) \
  $(CORE_DIR)/$(HOST_DIR)/$(HOST)

OBJECTS := $(SOURCES:%=$(BUILD_DIR)/$(SOURCE_DIR)/%.o)

$(BUILD_DIR)/$(OUTPUT): $(BUILD_DIR)/$(LIBCORE) $(OBJECTS)
	@echo -e "[100%] $(YELLOW)Linking $(OUTPUT)$(RESET)"
	$(Q)$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) $(BUILD_DIR)/$(LIBCORE) -o $@

$(BUILD_DIR)/$(SOURCE_DIR)/%.c.o: $(ROOT)/$(SOURCE_DIR)/%.c
	$(eval CURRENT_FILE := $(shell echo $$(($(CURRENT_FILE) + 1))))
	$(eval PERCENTAGE := $(shell echo $$(($(CURRENT_FILE) * 100 / $(TOTAL_BUILD_FILES)))))
	@echo -e "[$(PERCENTAGE)%] $(GREEN)Building C object $@$(RESET)"
	$(Q)$(CC) $(CFLAGS) $(INCLUDES:%=-I%) -MMD -MP -c $< -o $@

-include $(OBJECTS:.o=.d)

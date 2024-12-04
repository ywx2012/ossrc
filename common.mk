$(DIR)_SSRCS := $(wildcard $(DIR)/src/*.S)
$(DIR)_CSRCS := $(wildcard $(DIR)/src/*.c)
$(DIR)_OBJS := $($(DIR)_SSRCS:$(DIR)/src/%.S=$(BUILDDIR)/$(DIR)/%.o) $($(DIR)_CSRCS:$(DIR)/src/%.c=$(BUILDDIR)/$(DIR)/%.o)
OBJS := $(OBJS) $($(DIR)_OBJS)

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.S | makedirs
	$(V)gcc -MMD $(CFLAGS) $(INCLUDE_DIRS) -D__ASSEMBLY__ -c -o $@ $<

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc -MMD $(CFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

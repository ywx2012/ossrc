$(DIR)_SSRCS := $(wildcard $(DIR)/src/*.S)
$(DIR)_CSRCS := $(wildcard $(DIR)/src/*.c)
$(DIR)_OBJS := $($(DIR)_SSRCS:$(DIR)/src/%.S=$(BUILDDIR)/$(DIR)/%.o) $($(DIR)_CSRCS:$(DIR)/src/%.c=$(BUILDDIR)/$(DIR)/%.o)
OBJS := $(OBJS) $($(DIR)_OBJS)

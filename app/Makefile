DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))

-include common.mk
$(DIR)_TARGETS := $($(DIR)_OBJS:%.o=%.elf)
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

$(BUILDDIR)/$(DIR)/%.elf: $(BUILDDIR)/$(DIR)/%.o | makedirs libs
	$(V)ld $(LDFLAGS) -o $@ $^ $(LIBS)

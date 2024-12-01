DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))

-include common.mk
$(DIR)_TARGETS := $($(DIR)_OBJS:%.o=%.bin)
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

$(BUILDDIR)/$(DIR)/%.bin: $(BUILDDIR)/$(DIR)/%.o | makedirs libs
	$(V)ld $(LDFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc $(CFLAGS) -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.c | makedirs
	$(V)gcc -MM $(CFLAGS) -c -o $@ $^

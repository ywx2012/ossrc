DIR := $(lastword $(MAKEFILE_LIST:/Makefile=))

-include common.mk
$(DIR)_TARGETS := $($(DIR)_OBJS:%.o=%.bin)
TARGETS := $(TARGETS) $($(DIR)_TARGETS)

$(BUILDDIR)/$(DIR)/%.bin: $(BUILDDIR)/$(DIR)/%.o | makedirs libs
	$(V)ld $(USER_LDFLAGS) -o $@ $^ $(LIBS)

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc $(USER_CFLAGS) -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.c | makedirs
	$(V)gcc -MM $(USER_CFLAGS) -c -o $@ $^

-include common.mk

$(DIR)_LIBS := $(BUILDDIR)/$(DIR)/$(DIR).a
LIBS := $(LIBS) $($(DIR)_LIBS)
CFLAGS := $(CFLAGS) -I$(DIR)/include

$(BUILDDIR)/$(DIR)/$(DIR).a: $($(DIR)_OBJS) | makedirs
	$(V)ar crs $@ $^

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.S | makedirs
	$(V)gcc -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.o: $(DIR)/src/%.c | makedirs
	$(V)gcc -c $(CFLAGS) -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.S | makedirs
	$(V)gcc -MM -c -o $@ $^

$(BUILDDIR)/$(DIR)/%.d: $(DIR)/src/%.c | makedirs
	$(V)gcc -MM -c $(CFLAGS) -o $@ $^

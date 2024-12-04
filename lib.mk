-include common.mk

$(DIR)_LIBS := $(BUILDDIR)/$(DIR)/$(DIR).a
LIBS := $(LIBS) $($(DIR)_LIBS)
INCLUDE_DIRS := $(INCLUDE_DIRS) $(shell [ -d $(DIR)/include ] && echo -I$(DIR)/include)

$(BUILDDIR)/$(DIR)/$(DIR).a: $($(DIR)_OBJS) | makedirs
	$(V)ar crs $@ $^

SUBDIRS := ClientQt Server

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	for dir in $(SUBDIRS); do \
    $(MAKE) -C $$dir -f Makefile $@; \
  	done
	
.PHONY: all $(SUBDIRS)
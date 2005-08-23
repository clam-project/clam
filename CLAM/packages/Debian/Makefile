SUBDIRS = libclamcore0-dev libclamprocessing0-dev libclamio0-dev libclamvmqt0-dev libclamvmfl0-dev

.PHONY: init all clean $(SUBDIRS)

init: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir init; \
	done

all: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done

clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
 

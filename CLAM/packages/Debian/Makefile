SUBDIRS = libclam0-dev libclamcore0-dev libclamprocessing0-dev libclamio0-dev libclamvmqt0-dev libclamvmfl0-dev

.PHONY: init all clean $(SUBDIRS)

init: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir init; \
	done

all: $(SUBDIRS)
	@for f in `find . -type f -name .sconsign`; do \
		rm -rf $f; \
	done
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir; \
	done
	@mkdir -p ./debs
#	@for f in `find . -type f -name *.deb`; do \
#		echo $f; \
#		cp $f ./debs; \
#	done

clean: $(SUBDIRS)
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
 

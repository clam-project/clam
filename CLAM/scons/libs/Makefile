OPTIONS=prefix=/usr xmlbackend=xercesc release=yes

.PHONY: clean build all build-core build-processing build-audioio build-vmfl build-vmqt install install-core install-processing install-audioio install-vmfl install-vmqt

build: build-core build-processing build-audioio build-vmfl build-vmqt

clean:
	scons -c

build-core: 
	scons $(OPTIONS) core

build-processing:
	scons $(OPTIONS) processing

build-audioio:
	scons $(OPTIONS) audioio

build-vmfl:
	scons $(OPTIONS) vmfl

build-vmqt:
	scons $(OPTIONS) vmqt

install: install-core install-processing install-audioio install-vmfl install-vmqt

install-core:
	scons install_prefix=$(DESTDIR)/usr install_core

install-processing:
	scons install_prefix=$(DESTDIR)/usr install_processing

install-audioio:
	scons install_prefix=$(DESTDIR)/usr install_audioio

install-vmfl:
	scons install_prefix=$(DESTDIR)/usr install_vmfl

install-vmqt:
	scons install_prefix=$(DESTDIR)/usr install_vmqt

	

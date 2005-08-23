BUILD_PKG = dpkg-deb
TOP = ../..
PKG_NAME = libclamvmqt0-dev
VERSION = 0.8-1
ARCH = i386
PREFIX = /usr
PKG_BASE = ./debian
BIN_PREFIX= $(PREFIX)/bin
DOC_PREFIX= $(PREFIX)/share/doc/libclamvmqt0-dev
MAN_PREFIX= $(PREFIX)/share/man

all: init build

clean:
	@echo -n "Cleaning up... "
	@rm -rf $(PKG_BASE)
	@rm -rf *.deb
	@echo "Done!"

init:
	@echo -n "Initializing package directories... "
	@mkdir -p $(PKG_BASE)/DEBIAN
	@mkdir -p $(PKG_BASE)$(DOC_PREFIX)
	@echo "Done!"

setup-control:
	@echo -n "Setting up $(PKG_NAME) package control files... "
	@cp control $(PKG_BASE)/DEBIAN
	@cp shlibs $(PKG_BASE)/DEBIAN
	@cp copyright $(PKG_BASE)$(DOC_PREFIX)
	@echo "Done!"

setup-data:
	@echo -n "Moving $(PKG_NAME) manpages and assorted documentation... "
	@cp changelog $(PKG_BASE)/$(DOC_PREFIX)
	@gzip --best $(PKG_BASE)/$(DOC_PREFIX)/changelog
	@cp changelog.Debian $(PKG_BASE)/$(DOC_PREFIX)
	@gzip --best $(PKG_BASE)/$(DOC_PREFIX)/changelog.Debian
	@echo "Done!"

build: setup-control setup-data
	@echo -n "Setting up debian package... "
	@find $(PKG_BASE) -type d | xargs chmod 755
	@fakeroot $(BUILD_PKG) --build $(PKG_BASE)
	@mv $(PKG_BASE).deb $(PKG_NAME)_$(VERSION)_$(ARCH).deb
	@echo "Done!"
	@echo "Running lintian on $(PKG_NAME)_$(VERSION)_$(ARCH).deb..."
	@lintian $(PKG_NAME)_$(VERSION)_$(ARCH).deb

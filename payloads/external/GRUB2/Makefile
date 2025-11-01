ifeq ($(CONFIG_GRUB2_SECONDARY_PAYLOAD),y)
CONFIG_GRUB2_STABLE=$(CONFIG_GRUB2_SECONDARY_STABLE)
CONFIG_GRUB2_MASTER=$(CONFIG_GRUB2_SECONDARY_MASTER)
CONFIG_GRUB2_REVISION=$(CONFIG_GRUB2_SECONDARY_REVISION)
endif

TAG-$(CONFIG_GRUB2_MASTER)=origin/HEAD
TAG-$(CONFIG_GRUB2_REVISION)=$(CONFIG_GRUB2_REVISION_ID)

TAG-$(CONFIG_GRUB2_STABLE)=2.02_branch
NAME-$(CONFIG_GRUB2_MASTER)=HEAD
NAME-$(CONFIG_GRUB2_REVISION)=$(CONFIG_GRUB2_REVISION_ID)
NAME-$(CONFIG_GRUB2_STABLE)=2.02

project_git_repo=https://git.15h.org/mrothfuss/grub.git
project_dir=grub2
project_install=$(shell realpath ./$(project_dir))/install

unexport HOSTCC CC LD OBJCOPY STRIP
MAKEOVERRIDES :=

all: grub2-coreboot grub2-floppy

checkout:
	echo "    GIT        GRUB2 $(NAME-y)"
	test -d $(project_dir) || git clone $(project_git_repo) $(project_dir)
	git -C $(project_dir) fetch
	git -C $(project_dir) checkout -f $(TAG-y)

grub2/build/config.h: $(CONFIG_DEP) | checkout
	echo "    CONFIG     GRUB2 $(NAME-y)"
	rm -rf grub2/build
	mkdir grub2/build
	cd grub2 && ./autogen.sh
	cd grub2/build && ../configure CC="$(HOSTCC)" LD="$(LD)" \
	FREETYPE="pkg-config freetype2" BUILD_FREETYPE="pkg-config freetype2" \
	TARGET_CC="$(CC)" TARGET_OBJCOPY="$(OBJCOPY)" TARGET_STRIP="$(STRIP)" \
	MAKE="$(MAKE)" CFLAGS=-O2 TARGET_CFLAGS=-Os \
	--target=i386 --with-platform=$(PLATFORM) --enable-boot-time --disable-werror \
	--prefix="$(project_install)"

config: grub2/build/config.h checkout

grub2-coreboot: config
	echo "    MAKE       GRUB2 $(NAME-y)"
	$(MAKE) -C grub2/build
	$(MAKE) -C grub2/build default_payload.elf \
	        EXTRA_PAYLOAD_MODULES="$(CONFIG_GRUB2_EXTRA_MODULES)"

grub2-floppy: config
	echo "    MAKE       GRUB2 $(NAME-y)"
	test -d $(project_install) || $(MAKE) -C grub2/build install
	mkdir -p $(project_install)/share/locale
	$(project_install)/bin/grub-mkrescue --output=grub2/build/grub.img \
	--fonts=ascii.pf2 --compress=gz --install-modules="$(CONFIG_GRUB2_MODULES)" \
	"$(CONFIG_GRUB2_FLOPPY_FDROOT)"
	dd bs=1024 count=2880 if=/dev/zero of=grub2/build/floppy.img
	dd if=grub2/build/grub.img of=grub2/build/floppy.img conv=notrunc

clean:
	test -f grub2/build/Makefile && $(MAKE) -C grub2/build clean || exit 0

distclean:
	rm -rf grub2

print-repo-info:
	echo "$(project_git_repo) $(project_dir)"

.PHONY: checkout config grub2 clean distclean print-repo-info

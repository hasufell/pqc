include common.mk


all:
	$(MAKE) -C src

main:
	$(MAKE) -C src main

install:
	$(MAKE) -C src install
	$(MAKE) -C include install

uninstall:
	$(MAKE) -C src uninstall
	$(MAKE) -C include uninstall

check:
	$(MAKE) -C tests check

test:
	$(MAKE) -C tests check

clean:
	$(MAKE) -C include clean
	$(MAKE) -C src clean
	$(MAKE) -C tests clean

doc:
	$(MAKE) -C include doc
	$(MAKE) -C src doc

doc-pdf: doc
	$(MAKE) -C include doc-pdf
	$(MAKE) -C src doc-pdf


.PHONY: clean doc doc-pdf install

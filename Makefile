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

clean:
	$(MAKE) -C src clean
	$(MAKE) -C doxygen clean

doc:
	$(MAKE) -C doxygen doc

doc-pdf: doc
	$(MAKE) -C doxygen doc-pdf


.PHONY: clean doc doc-pdf install

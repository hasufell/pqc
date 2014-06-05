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
	$(MAKE) -C include clean

doc:
	$(MAKE)	 -C src doc
	$(MAKE)	 -C include doc

doc-pdf: doc
	$(MAKE) -C src doc-pdf
	$(MAKE) -C include doc-pdf


.PHONY: clean doc doc-pdf install

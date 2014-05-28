include common.mk


all:
	$(MAKE) -C src

main:
	$(MAKE) -C src main

clean:
	$(MAKE) -C src clean

doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf


.PHONY: clean doc doc-pdf

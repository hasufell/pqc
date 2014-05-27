include common.mk


all:
	$(MAKE) -C src

main:
	$(MAKE) -C src main

clean:
	$(MAKE) -C src clean

clean-all: clean
	$(MAKE) -C external/flint-2.4.3 clean

doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf


.PHONY: clean clean-all doc doc-pdf

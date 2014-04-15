all:
	$(MAKE) -C src

main:
	$(MAKE) -C src main

clean:
	$(MAKE) -C src clean

clean-all: clean
	$(MAKE) -C external/libtommath-0.42.0 clean
	$(MAKE) -C external/libtompoly-0.04 clean

doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf


.PHONY: clean clean-all doc doc-pdf

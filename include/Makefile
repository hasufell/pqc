include ../common.mk


install:
	$(INSTALL_DIR) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"
	$(INSTALL) ntru.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru.h
	$(INSTALL) decrypt.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/decrypt.h
	$(INSTALL) encrypt.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/encrypt.h
	$(INSTALL) keypair.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/keypair.h
	$(INSTALL) rnd.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/rnd.h

uninstall:
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/decrypt.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/encrypt.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/keypair.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/rnd.h

doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf

clean:
	rm -rf html/ latex/


.PHONY: clean doc doc-pdf install uninstall

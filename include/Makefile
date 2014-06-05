include ../common.mk


doc:
	doxygen

doc-pdf: doc
	$(MAKE) -C latex pdf

install:
	$(INSTALL_DIR) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"
	$(INSTALL) ntru.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru.h
	$(INSTALL) ntru_decrypt.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_decrypt.h
	$(INSTALL) ntru_encrypt.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_encrypt.h
	$(INSTALL) ntru_keypair.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_keypair.h
	$(INSTALL) ntru_rnd.h "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_rnd.h

uninstall:
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_decrypt.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_encrypt.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_keypair.h
	$(RM) "$(DESTDIR)$(INSTALL_INCLUDEDIR)"/ntru_rnd.h

clean:
	rm -rf html/ latex/


.PHONY: clean doc doc-pdf install uninstall


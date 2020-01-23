EXTRA_DIST = FaunaOne-Regular.ttf

install-data-local:
	$(mkinstalldirs) $(DESTDOR)$(datdir)/fonts
	@for file in $(EXTRA_DIST); do\
		$(INSTALL_DATA) $$file $(DESTDIR)$(datdir)/fonts/$$file;\
	done

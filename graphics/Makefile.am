EXTRA_DIST = button.png button2line.png InterfaceButtons.tga \
keyboard_top_separator.png longbutton.png star.png thumb_down.png \
title_Logo.png trackbox.png

install-data-local:
	$(mkinstalldirs) $(DESTDOR)$(datdir)/graphics
	@for file in $(EXTRA_DIST); do\
		$(INSTALL_DATA) $$file $(DESTDIR)$(datdir)/graphics/$$file;\
	done

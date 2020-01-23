EXTRA_DIST = Aria-Notebook_AM_Bach.mid Fur_Elise-Beethoven.mid \
Minuet_in_F-Leopold_Mozart.mid Rondino-Rameau.mid Sonatina_in_C-Latour.mid \
Clementi_Opus_36_no_1_First_Movement.mid Minuet_in_G-JS_Bach.mid \
Russian_Folksong-Beethoven.mid


install-data-local:
	$(mkinstalldirs) $(DESTDOR)$(datdir)/songs
	@for file in $(EXTRA_DIST); do\
		$(INSTALL_DATA) $$file $(DESTDIR)$(datdir)/songs/$$file;\
	done

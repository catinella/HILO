
cleanall.target   = cleanall
cleanall.commands = $$escape_expand(@rm -fv $$DESTDIR/$$TARGET Makefile)
cleanall.depends  = clean

QMAKE_EXTRA_TARGETS += cleanall

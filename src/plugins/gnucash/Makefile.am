# Gnucash plugin

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src $(GRISBI_CFLAGS) $(XML2_CFLAGS)

if HAVE_XML2

noinst_LTLIBRARIES = gnucash.la
gnucash_la_SOURCES = gnucash.c gnucash.h
gnucash_la_LDFLAGS = -module -avoid-version
gnucash_la_LIBADD = $(XML2_LIBS)

endif

CLEANFILES = \
	*~

MAINTAINERCLEANFILES = \
	Makefile.in

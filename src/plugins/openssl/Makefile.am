# OpenSSL plugin

AM_CPPFLAGS = -I$(top_srcdir) -I$(top_srcdir)/src $(GRISBI_CFLAGS) $(OPENSSL_CFLAGS)

if HAVE_SSL

noinst_LTLIBRARIES = openssl.la
openssl_la_SOURCES = openssl.c openssl.h
openssl_la_LDFLAGS = -module -avoid-version
openssl_la_LIBADD = $(OPENSSL_LIBS)

endif

CLEANFILES = \
	*~

MAINTAINERCLEANFILES = \
	Makefile.in

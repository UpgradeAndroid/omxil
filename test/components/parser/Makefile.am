check_PROGRAMS = omxparsertest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(srcdir)/../common -I$(includedir)

omxparsertest_SOURCES = omxparsertest.c omxparsertest.h
omxparsertest_LDADD = $(bellagio_LDADD) -lpthread
omxparsertest_CFLAGS = $(common_CFLAGS)

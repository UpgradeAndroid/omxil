check_PROGRAMS = omxmuxtest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir)

omxmuxtest_SOURCES = omxmuxtest.c omxmuxtest.h
omxmuxtest_LDADD = $(bellagio_LDADD) -lpthread
omxmuxtest_CFLAGS = $(common_CFLAGS)

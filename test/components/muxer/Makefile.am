check_PROGRAMS = omxmuxtest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(srcdir)/../common -I$(includedir)

omxmuxtest_SOURCES = omxmuxtest.c omxmuxtest.h
omxmuxtest_LDADD = $(bellagio_LDADD) -lpthread
omxmuxtest_CFLAGS = $(common_CFLAGS)

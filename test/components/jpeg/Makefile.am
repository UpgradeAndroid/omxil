check_PROGRAMS = omxjpegdectest omxjpegenctest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(srcdir)/../common

omxjpegdectest_SOURCES = omxjpegdectest.c omxjpegdectest.h
omxjpegdectest_LDADD = $(bellagio_LDADD) -lpthread
omxjpegdectest_CFLAGS = $(common_CFLAGS)

omxjpegenctest_SOURCES = omxjpegenctest.c omxjpegenctest.h
omxjpegenctest_LDADD = $(bellagio_LDADD) -lpthread
omxjpegenctest_CFLAGS = $(common_CFLAGS)

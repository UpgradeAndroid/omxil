check_PROGRAMS = omxcameratest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(srcdir)/../common

omxcameratest_SOURCES = omxcameratest.c omxcameratest.h
omxcameratest_LDADD = $(bellagio_LDADD) -lpthread
omxcameratest_CFLAGS = $(common_CFLAGS)

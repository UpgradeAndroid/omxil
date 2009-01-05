check_PROGRAMS = omxvideodectest omxvideocapturetest omxvideocapnplay omxvideoenctest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir)

omxvideodectest_SOURCES = omxvideodectest.c omxvideodectest.h
omxvideodectest_LDADD   = $(bellagio_LDADD) -lpthread
omxvideodectest_CFLAGS  = $(common_CFLAGS)

omxvideocapnplay_SOURCES = omxvideocapnplay.c omxvideocapnplay.h
omxvideocapnplay_LDADD   = $(bellagio_LDADD) -lpthread
omxvideocapnplay_CFLAGS  = $(common_CFLAGS)

omxvideoenctest_SOURCES = omxvideoenctest.c omxvideoenctest.h
omxvideoenctest_LDADD   = $(bellagio_LDADD) -lpthread
omxvideoenctest_CFLAGS  = $(common_CFLAGS)

omxvideocapturetest_SOURCES = omxvideocapturetest.c omxvideocapturetest.h
omxvideocapturetest_LDADD   = $(bellagio_LDADD) -lpthread 
omxvideocapturetest_CFLAGS  = $(common_CFLAGS)


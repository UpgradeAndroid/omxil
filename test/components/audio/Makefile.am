check_PROGRAMS = omxaudiodectest audio_use_case audio_use_case_tunnel omxaudiocapnplay omxvoicexchange omxaudioenctest

bellagio_LDADD = -lomxil-bellagio
common_CFLAGS = -I$(top_srcdir)/test/components/common -I$(includedir)

omxaudiodectest_SOURCES = omxaudiodectest.c omxaudiodectest.h
omxaudiodectest_LDADD = $(bellagio_LDADD) -lpthread
omxaudiodectest_CFLAGS = $(common_CFLAGS)

omxaudiocapnplay_SOURCES = omxaudiocapnplay.c omxaudiocapnplay.h
omxaudiocapnplay_LDADD = $(bellagio_LDADD) -lpthread
omxaudiocapnplay_CFLAGS = $(common_CFLAGS)

audio_use_case_SOURCES = audio_use_case.c omxaudiodectest.h
audio_use_case_LDADD = $(bellagio_LDADD) -lpthread
audio_use_case_CFLAGS = $(common_CFLAGS)

audio_use_case_tunnel_SOURCES = audio_use_case_tunnel.c omxaudiodectest.h
audio_use_case_tunnel_LDADD = $(bellagio_LDADD) -lpthread
audio_use_case_tunnel_CFLAGS = $(common_CFLAGS)

omxvoicexchange_SOURCES = omxvoicexchange.c omxvoicexchange.h 
omxvoicexchange_LDADD =  $(bellagio_LDADD) -lpthread 
omxvoicexchange_CFLAGS= $(common_CFLAGS)

omxaudioenctest_SOURCES = omxaudioenctest.c omxaudioenctest.h
omxaudioenctest_LDADD = $(bellagio_LDADD) -lpthread
omxaudioenctest_CFLAGS = $(common_CFLAGS)


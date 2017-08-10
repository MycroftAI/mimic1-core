ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -Wall -Werror -I$(top_srcdir)/include $(PCRE2_CFLAGS) -DMIMIC_PLUGIN_DIR='"${libdir}/ttsmimic/plugins"'

SUBDIRS = .

# instead of pkgincludedir we will use $(includedir)/ttsmimic
headersdir = $(includedir)/ttsmimic/

EXTRA_DIST = ACKNOWLEDGEMENTS COPYING dependencies.sh thirdparty/pcre2.patch run_testsuite.sh meson.build meson_options.txt
noinst_HEADERS = 
headers_HEADERS =
lib_LTLIBRARIES =

lib_LTLIBRARIES += libttsmimiccore.la
libttsmimiccore_la_SOURCES = 
libttsmimiccore_la_CFLAGS =
libttsmimiccore_la_LDFLAGS = -no-undefined
libttsmimiccore_la_LIBADD = $(PCRE2_LIBS)


TESTS = 

pkgconfiginstalldir = $(libdir)/pkgconfig
pkgconfiginstall_DATA = ttsmimiccore.pc

voicesinstalldir = $(pkgdatadir)/voices
voicesinstall_DATA = 

###### src/audio ###############

src_audio_SOURCES = \
  src/audio/au_alsa.c \
  src/audio/auclient.c \
  src/audio/au_command.c \
  src/audio/audio.c \
  src/audio/au_none.c \
  src/audio/au_oss.c \
  src/audio/au_portaudio.c \
  src/audio/au_pulseaudio.c \
  src/audio/auserver.c \
  src/audio/au_streaming.c \
  src/audio/au_sun.c \
  src/audio/au_wince.c \
  src/audio/native_audio.h

libttsmimiccore_la_CFLAGS += $(AUDIODEFS) -DPKGDATADIR='"$(pkgdatadir)"'
libttsmimiccore_la_SOURCES += $(src_audio_SOURCES)

libttsmimiccore_la_LIBADD += $(AUDIOLIBS) $(HTS_LIBS)

###### src/cg ##################
libttsmimiccore_la_SOURCES += \
  src/cg/cst_mlsa.h \
  src/cg/cst_mlpg.h \
  src/cg/cst_vc.h \
  src/cg/cst_cg_map.h \
  src/cg/cst_spamf0.h \
  src/cg/cst_cg.c \
  src/cg/cst_mlsa.c \
  src/cg/cst_mlpg.c \
  src/cg/cst_vc.c \
  src/cg/cst_cg_load_voice.c \
  src/cg/cst_cg_dump_voice.c \
  src/cg/cst_cg_map.c \
  src/cg/cst_spamf0.c

####### src/hrg ###############
libttsmimiccore_la_SOURCES += \
  src/hrg/cst_utterance.c \
  src/hrg/cst_relation.c \
  src/hrg/cst_item.c \
  src/hrg/cst_ffeature.c \
  src/hrg/cst_rel_io.c

####### src/hts ###############
libttsmimiccore_la_SOURCES += \
  src/hts/flite_hts_engine.c

if HTSEngineBundled
  AM_CPPFLAGS += -I$(top_srcdir)/src/hts/hts_engine_API/include
  headers_HEADERS +=src/hts/hts_engine_API/include/HTS_engine.h
  libttsmimiccore_la_SOURCES += \
    src/hts/hts_engine_API/lib/HTS_audio.c \
    src/hts/hts_engine_API/lib/HTS_engine.c \
    src/hts/hts_engine_API/lib/HTS_hidden.h \
    src/hts/hts_engine_API/lib/HTS_misc.c  \
    src/hts/hts_engine_API/lib/HTS_pstream.c \
    src/hts/hts_engine_API/lib/HTS_sstream.c \
    src/hts/hts_engine_API/lib/HTS_model.c \
    src/hts/hts_engine_API/lib/HTS_vocoder.c \
    src/hts/hts_engine_API/lib/HTS_gstream.c \
    src/hts/hts_engine_API/lib/HTS_label.c
else
  libttsmimiccore_la_LIBADD += $(HTS_LIBS)
endif

###### src/lexicon #########

libttsmimiccore_la_SOURCES += \
  src/lexicon/cst_lexicon.c \
  src/lexicon/cst_lts.c \
  src/lexicon/cst_lts_rewrites.c


###### src/regex ########
libttsmimiccore_la_SOURCES += \
  src/regex/cst_regex.c \
  src/regex/cst_regex_defs.h \
  src/regex/regexp.c \
  src/regex/regsub.c

#regexes:
#	./make_cst_regexes $(BINDIR) >cst_regex_defs.h

EXTRA_DIST += src/regex/compile_mimic_regex.c


###### src/speech #########
libttsmimiccore_la_SOURCES += \
  src/speech/cst_lpcres.c \
  src/speech/cst_track.c \
  src/speech/cst_track_io.c \
  src/speech/cst_wave.c \
  src/speech/cst_wave_io.c \
  src/speech/cst_wave_utils.c \
  src/speech/g721.c \
  src/speech/g723_24.c \
  src/speech/g723_40.c \
  src/speech/g72x.c \
  src/speech/g72x.h \
  src/speech/rateconv.c

###### src/stats #########
libttsmimiccore_la_SOURCES += \
  src/stats/cst_cart.c \
  src/stats/cst_ss.c \
  src/stats/cst_viterbi.c

###### src/synth #########
libttsmimiccore_la_SOURCES += \
  src/synth/cst_ffeatures.c \
  src/synth/cst_phoneset.c \
  src/synth/cst_ssml.c \
  src/synth/cst_synth.c \
  src/synth/cst_utt_utils.c \
  src/synth/cst_voice.c \
  src/synth/mimic.c

###### src/utils #########
libttsmimiccore_la_SOURCES += \
  src/utils/cst_alloc.c \
  src/utils/cst_args.c \
  src/utils/cst_endian.c \
  src/utils/cst_error.c \
  src/utils/cst_features.c \
  src/utils/cst_file_stdio.c \
  src/utils/cst_mmap_none.c \
  src/utils/cst_mmap_posix.c \
  src/utils/cst_mmap_win32.c \
  src/utils/cst_plugins.c \
  src/utils/cst_socket.c \
  src/utils/cst_string.c \
  src/utils/cst_tokenstream.c \
  src/utils/cst_uregex.c \
  src/utils/cst_url.c \
  src/utils/cst_val.c \
  src/utils/cst_val_const.c \
  src/utils/cst_val_user.c \
  src/utils/cst_wchar.c

###### src/wavesynth #########
libttsmimiccore_la_SOURCES += \
  src/wavesynth/cst_clunits.c \
  src/wavesynth/cst_diphone.c \
  src/wavesynth/cst_reflpc.c \
  src/wavesynth/cst_sigpr.c \
  src/wavesynth/cst_sts.c \
  src/wavesynth/cst_units.c

###############################

libttsmimiccore_la_LIBADD += $(AUDIOLIBS) -lm


################# main ########################
bin_PROGRAMS = mimic compile_regexes mimicvox_info

mimic_SOURCES = main/mimic_main.c
mimic_LDADD = libttsmimiccore.la

compile_regexes_SOURCES = main/compile_regexes.c
compile_regexes_LDADD = libttsmimiccore.la

mimicvox_info_SOURCES = \
  main/mimicvox_info_main.c
mimicvox_info_LDADD = libttsmimiccore.la


################# END: main ###################
dist_noinst_SCRIPTS = autogen.sh

########## Unit tests #########################
noinst_HEADERS += unittests/cutest.h

myunittests = unittests/hrg_test \
              unittests/regex_test \
              unittests/string_test \
              unittests/token_test \
              unittests/wave_test

unittests_hrg_test_SOURCES = unittests/hrg_test_main.c
unittests_hrg_test_LDADD = libttsmimiccore.la

check_PROGRAMS = $(myunittests)

unittests_regex_test_SOURCES = unittests/regex_test_main.c
unittests_regex_test_LDADD = libttsmimiccore.la

unittests_string_test_SOURCES = unittests/string_test_main.c
unittests_string_test_LDADD = libttsmimiccore.la $(PCRE2_LIBS)


unittests_token_test_SOURCES = unittests/token_test_main.c
unittests_token_test_CFLAGS = -DTEST_FILE=\"$(top_srcdir)/unittests/data.one\" \
                              -DTEST_FILE_UTF8=\"$(top_srcdir)/unittests/data_utf8.txt\"
unittests_token_test_LDADD = libttsmimiccore.la

unittests_wave_test_SOURCES = unittests/wave_test_main.c
unittests_wave_test_LDADD = libttsmimiccore.la
unittests_wave_test_CFLAGS = -DA_WAV1=\"$(top_srcdir)/unittests/hello_world.wav\" \
                             -DA_WAV2=\"$(top_srcdir)/unittests/hi_again.wav\" 


EXTRA_DIST += unittests/data.one unittests/data_utf8.txt \
    unittests/hello_world.wav unittests/hi_again.wav

########## END Unit tests #########################

########## START testsuite #########################
check_PROGRAMS += \
  testsuite/asciiS2U \
  testsuite/asciiU2S \
  testsuite/bin2ascii \
  testsuite/combine_waves \
  testsuite/compare_wave \
  testsuite/lpc_resynth \
  testsuite/lpc_test2 \
  testsuite/lpc_test \
  testsuite/play_client \
  testsuite/play_server \
  testsuite/play_sync \
  testsuite/play_wave \
  testsuite/rfc

testsuite_asciiS2U_SOURCES = testsuite/asciiS2U_main.c
testsuite_asciiS2U_LDADD = libttsmimiccore.la

testsuite_asciiU2S_SOURCES = testsuite/asciiU2S_main.c
testsuite_asciiU2S_LDADD = libttsmimiccore.la

testsuite_bin2ascii_SOURCES = testsuite/bin2ascii_main.c
testsuite_bin2ascii_LDADD = libttsmimiccore.la


testsuite_combine_waves_SOURCES = testsuite/combine_waves_main.c
testsuite_combine_waves_LDADD = libttsmimiccore.la

testsuite_compare_wave_SOURCES = testsuite/compare_wave_main.c
testsuite_compare_wave_LDADD = libttsmimiccore.la

testsuite_lpc_resynth_SOURCES = testsuite/lpc_resynth_main.c
testsuite_lpc_resynth_LDADD = libttsmimiccore.la

testsuite_lpc_test2_SOURCES = testsuite/lpc_test2_main.c
testsuite_lpc_test2_LDADD = libttsmimiccore.la

testsuite_lpc_test_SOURCES = testsuite/lpc_test_main.c
testsuite_lpc_test_LDADD = libttsmimiccore.la

testsuite_play_client_SOURCES = testsuite/play_client_main.c
testsuite_play_client_LDADD = libttsmimiccore.la

testsuite_play_server_SOURCES = testsuite/play_server_main.c
testsuite_play_server_LDADD = libttsmimiccore.la

testsuite_play_sync_SOURCES = testsuite/play_sync_main.c
testsuite_play_sync_LDADD = libttsmimiccore.la

testsuite_play_wave_SOURCES = testsuite/play_wave_main.c
testsuite_play_wave_LDADD = libttsmimiccore.la

testsuite_rfc_SOURCES = testsuite/rfc_main.c
testsuite_rfc_LDADD = libttsmimiccore.la

TESTS += $(myunittests)

headers_HEADERS += \
  include/cst_alloc.h \
  include/cst_args.h \
  include/cst_audio.h \
  include/cst_cart.h \
  include/cst_cg.h \
  include/cst_clunits.h \
  include/mimic_core_config.h \
  include/cst_diphone.h \
  include/cst_endian.h \
  include/cst_error.h \
  include/cst_features.h \
  include/cst_ffeatures.h \
  include/cst_file.h \
  include/cst_hrg.h \
  include/cst_item.h \
  include/cst_lexicon.h \
  include/cst_lts.h \
  include/cst_lts_rewrites.h \
  include/cst_plugins.h \
  include/cst_phoneset.h \
  include/cst_regex.h \
  include/cst_relation.h \
  include/cst_sigpr.h \
  include/cst_socket.h \
  include/cst_ss.h \
  include/cst_string.h \
  include/cst_sts.h \
  include/cst_synth.h \
  include/cst_tokenstream.h \
  include/cst_track.h \
  include/cst_units.h \
  include/cst_uregex.h \
  include/cst_utterance.h \
  include/cst_utt_utils.h \
  include/cst_val_const.h \
  include/cst_val_defs.h \
  include/cst_val.h \
  include/cst_viterbi.h \
  include/cst_voice.h \
  include/cst_wave.h \
  include/cst_wchar.h \
  include/flite_hts_engine.h \
  include/mimic.h

# Documentation
dist_man1_MANS = man/man1/mimic.1
EXTRA_DIST += doc/alice doc/intro.txt doc/Makefile doc/mimic.texi doc/stuff.ed

clean-local:
	find . -name '*.gcno' -exec rm {} \;
	find . -name '*.gcda' -exec rm {} \;
	find . -name '*.gcov' -exec rm {} \;


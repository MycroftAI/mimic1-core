dnl Use autogen.sh to regenerate the configure script
AC_PREREQ([2.62])
AC_INIT([mimic],
        [1.1.0.9000],
        [https://github.com/MycroftAI/mimic/issues])

AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])


AC_CANONICAL_HOST
AC_CANONICAL_BUILD

AM_INIT_AUTOMAKE([-Wall foreign subdir-objects])
AC_PROG_CC
AM_PROG_AR
LT_INIT([dlopen win32-dll disable-static])
AM_SILENT_RULES([yes])
AC_PROG_INSTALL
AC_CONFIG_SRCDIR([include/mimic.h])
AC_CONFIG_HEADERS([include/config.h])
LT_PREREQ([2.2.6])
AC_C_BIGENDIAN

PKG_PROG_PKG_CONFIG

dnl By default enable shared libraries.
dnl use ./configure --enable-static (and/or --disable-shared)
dnl to build them or not.


dnl openmp support for multi-threaded test:
AC_OPENMP

dnl
dnl Does this platform support mmap(2)
dnl Note we don't use AC_FUNC_MMAP because we really only care whether
dnl read-only mappings work.
dnl
MMAPTYPE=
dnl These numbers must match the assignments below
AC_DEFINE([MMAP_TYPE_NONE], [0], [enum for mmap_types]) 
AC_DEFINE([MMAP_TYPE_POSIX], [1], [enum for mmap_types]) 
AC_DEFINE([MMAP_TYPE_WIN32], [2], [enum for mmap_types]) 
AC_CHECK_FUNC([mmap], [MMAPTYPE=posix])

dnl Force no mmap for chrome
AS_IF([test "$host_os" = "chrome"], [MMAPTYPE=none])

dnl AC_CHECK_FUNC doesn't seem to work for Win32 API calls...
AS_IF([ test "x$MMAPTYPE" = "x" ],
      [ AS_CASE([ "${host_os}" ],
                [ cygwin*|mingw* ], [ MMAPTYPE=win32],
                [ android* ], [ MMAPTYPE=posix],
                [ MMAPTYPE=none] )])

dnl These numbers must match the definitions above
AS_CASE( [ ${MMAPTYPE} ],
         [ win32 ], [ AC_DEFINE([MMAP_TYPE], [2], [setting an mmap type])],
         [ posix ], [ AC_DEFINE([MMAP_TYPE], [1], [setting an mmap type])],
         [ none ],  [ AC_DEFINE([MMAP_TYPE], [0], [setting an mmap type])])

AC_CHECK_FUNCS([pow])
AC_CHECK_LIB([m], [pow])

dnl Whether or not we have sockets (we don't in Windows)
AC_CHECK_HEADERS([sys/socket.h])


dnl: Look for htsengine_API headers and library
AC_ARG_WITH([hts],
  [AS_HELP_STRING([--without-hts],
    [disable hts speech synthesis capabilities (for hts voices)])],
  [],
  [with_hts=yes])
AM_CONDITIONAL([HTSEngine], [test "x$with_hts" != xno])
  
HTS_LIBS=
with_hts_bundled="no"
AS_IF([test "x$with_hts" != "xno"],
  [AC_DEFINE([HAVE_HTSENGINE], [1], [Define if you have hts_engine])
      AC_CHECK_LIB([HTSEngine],
                [HTS_Engine_initialize],
                [AC_SUBST([HTS_LIBS], ["-lHTSEngine"])
                ],
                [with_hts_bundled="yes"
                                 AC_MSG_NOTICE([HTSEngine was not detected, using bundled HTSEngine])
                ],
                [])
  ])
AM_CONDITIONAL([HTSEngineBundled], [test "x$with_hts_bundled" != "xno"])

dnl: pkg-config mimic.pc file:
PKGCONFIG_MIMIC_CORE_CFLAGS="-I${includedir}/ttsmimic -I${includedir}/ttsmimic/lang -DMIMIC_PLUGIN_DIR=@libdir@/ttsmimic/plugins"
PKGCONFIG_MIMIC_CORE_LIBS="-L${libdir}"
PKGCONFIG_MIMIC_CORE_DEPS=


PKGCONFIG_MIMIC_CFLAGS="-I${includedir}/ttsmimic -I${includedir}/ttsmimic/lang"
PKGCONFIG_MIMIC_LIBS="-L${libdir}"
PKGCONFIG_MIMIC_DEPS="libttsmimiccore"

dnl Check libpcre2-8
PKG_CHECK_MODULES([PCRE2], [libpcre2-8])
PKGCONFIG_MIMIC_CORE_DEPS="${PKGCONFIG_MIMIC_CORE_DEPS} libpcre2-8"

AS_IF([test "x$enable_shared" = "xyes"],
  [enable_plugins=yes
   AC_SEARCH_LIBS([dlopen], [dl dld], [], [
     AC_MSG_NOTICE([dlopen not available, disabling plugin support])
     enable_plugins="no"])],
  [enable_plugins=no
   AC_MSG_NOTICE([plugins not available without shared libraries])
  ])

AS_IF([test "x$enable_plugins" != "xno"],
  [AC_DEFINE([MIMIC_ENABLE_PLUGINS], [1], [Define if you want plugin support])
  PKGCONFIG_MIMIC_CORE_LIBS="${PKGCONFIG_MIMIC_LIBS} -ldl"
])
AM_CONDITIONAL([MIMIC_ENABLE_PLUGINS], [test "x$enable_plugins" != "xno"])


dnl
dnl determine audio type or use none if none supported on this platform
dnl
AUDIODRIVER=none

dnl check for OSS in FreeBSD and Sun:
AUDIODETECTED_OSS="false"
AC_CHECK_HEADER(machine/soundcard.h,
              [AUDIODRIVER="oss"
               AUDIODETECTED_OSS="true"])
AUDIODEFS_OSS=-DCST_AUDIO_FREEBSD
AUDIOLIBS_OSS=

AUDIODETECTED_SUN="false"
AC_CHECK_HEADER(sys/audioio.h,
              [AUDIODRIVER="sun"
               AUDIODETECTED_SUN="true"
              ])
AUDIODEFS_SUN=-DCST_AUDIO_SUNOS
AUDIOLIBS_SUN=

dnl Check PulseAudio
AUDIODETECTED_PULSEAUDIO="false"
PKG_CHECK_MODULES([PULSEAUDIO], [libpulse],
                  [AUDIODRIVER="pulseaudio"
                   AUDIODETECTED_PULSEAUDIO="true"],
                  [m4_ignore])
AUDIODEFS_PULSEAUDIO="-DCST_AUDIO_PULSEAUDIO ${PULSEAUDIO_CFLAGS}"
AUDIOLIBS_PULSEAUDIO="${PULSEAUDIO_LIBS}"

dnl Check PortAudio is at least (v19-dev)
AUDIODETECTED_PORTAUDIO="false"
PKG_CHECK_MODULES([PORTAUDIO], [portaudio-2.0 >= 19],
                  [AUDIODRIVER="portaudio"
                   AUDIODETECTED_PORTAUDIO="true"],
                  [m4_ignore])
AUDIODEFS_PORTAUDIO="-DCST_AUDIO_PORTAUDIO ${PORTAUDIO_CFLAGS}"
AUDIOLIBS_PORTAUDIO="${PORTAUDIO_LIBS}"

dnl Check for ALSA sound library
AUDIODETECTED_ALSA="false"
PKG_CHECK_MODULES([ALSA], [alsa >= 1.0.11],
                  [AUDIODRIVER="alsa"
                   AUDIODETECTED_ALSA="true"],
                  [m4_ignore])
AUDIODEFS_ALSA="-DCST_AUDIO_ALSA ${ALSA_CFLAGS}"
AUDIOLIBS_ALSA="${ALSA_LIBS}"

dnl check for wince audio
AUDIODETECTED_WINCE="false"
AC_CHECK_HEADERS([mmsystem.h],
                 [AUDIODRIVER="wince"
                  AUDIODETECTED_WINCE="true"],
                 [],
                 AC_INCLUDES_DEFAULT)
AUDIODEFS_WINCE=-DCST_AUDIO_WINCE
AUDIOLIBS_WINCE=-lwinmm

dnl For wince you get wince, for android and chrome you get none:
AS_CASE([ ${host_os} ],
        [ wince* ], [ AUDIODRIVER="wince"],
        [ android* ], [ AUDIODRIVER="none"],
        [ chrome* ], [AUDIODRIVER="none"])

dnl
dnl allow the user to override the one detected above
dnl
AC_ARG_WITH([audio],
   [AS_HELP_STRING([--with-audio],
      [with specific audio support (none alsa portaudio etc) ])],
   [AUDIODRIVER=$with_audio])

AS_CASE( [ ${AUDIODRIVER} ],
         [ alsa ],
           [ AUDIODEFS="${AUDIODEFS_ALSA}"
             AUDIOLIBS="${AUDIOLIBS_ALSA}"
             AUDIODETECTED="${AUDIODETECTED_ALSA}"
             PKGCONFIG_MIMIC_CORE_DEPS="${PKGCONFIG_MIMIC_CORE_DEPS} alsa"
           ],
         [ pulseaudio ],
           [ AUDIODEFS="${AUDIODEFS_PULSEAUDIO}"
             AUDIOLIBS="${AUDIOLIBS_PULSEAUDIO}"
             AUDIODETECTED="${AUDIODETECTED_PULSEAUDIO}"
             PKGCONFIG_MIMIC_CORE_DEPS="${PKGCONFIG_MIMIC_CORE_DEPS} libpulse"
           ],
         [ portaudio ],
           [ AUDIODEFS="${AUDIODEFS_PORTAUDIO}"
             AUDIOLIBS="${AUDIOLIBS_PORTAUDIO}"
             AUDIODETECTED="${AUDIODETECTED_PORTAUDIO}"
             PKGCONFIG_MIMIC_CORE_DEPS="${PKGCONFIG_MIMIC_CORE_DEPS} portaudio-2.0"
	   ],
         [ sun ],
           [ AUDIODEFS="${AUDIODEFS_SUN}"
             AUDIOLIBS="${AUDIOLIBS_SUN}"
             AUDIODETECTED="${AUDIODETECTED_SUN}"
           ],
         [ oss|*bsd ],
           [ AUDIODRIVER=oss
             AUDIODEFS="${AUDIODEFS_OSS}"
             AUDIOLIBS="${AUDIOLIBS_OSS}"
             AUDIODETECTED="${AUDIODETECTED_OSS}"
           ],
         [ wince|win32 ],
           [ AUDIODEFS="${AUDIODEFS_WINCE}"
             AUDIOLIBS="${AUDIOLIBS_WINCE}"
             AUDIODETECTED="${AUDIODETECTED_WINCE}"
           ],
         [ none ],
           [ AUDIODEFS=-DCST_AUDIO_NONE
             AUDIOLIBS=
             AUDIODETECTED="true"
           ])

AS_IF([ test "${AUDIODETECTED}" = "false" ],
      [AC_MSG_WARN([Using $AUDIODRIVER as audio module, it was not autodetected])])

AC_SUBST(AUDIODRIVER)
AC_SUBST(AUDIODEFS)
AC_SUBST(AUDIOLIBS)


dnl
dnl language/lexicon/voice parameters 
dnl
dnl By default all languages, lexicons and voices are compiled
dnl --------------------- Global language switch ---------------------
AC_ARG_ENABLE([lang-all],
  [AS_HELP_STRING([--disable-lang-all],
      [disable all languages support])
   AS_HELP_STRING([--enable-lang-all=plugin],
      [build languages as plugins])
  ],
  [],
  [enable_lang_all=yes]) dnl by default all languages are included

AC_ARG_ENABLE([voices-all],
  [AS_HELP_STRING([--disable-voices-all],
      [disable embedding all voices])
   AS_HELP_STRING([--enable-voices-all=plugin],
      [build voices as plugins])
  ],
  [],
  [enable_voices_all=yes])

AC_ARG_ENABLE([lang-english],
  [AS_HELP_STRING([--disable-lang-english],
      [disable English language support])
   AS_HELP_STRING([--enable-lang-english=plugin],
      [build English language support as plugin])
  ],
  [],
  [enable_lang_english=${enable_lang_all}])

AC_ARG_ENABLE([lang-grapheme],
  [AS_HELP_STRING([--disable-lang-grapheme],
      [disable grapheme language support])],
  [],
  [enable_lang_grapheme=${enable_lang_all}])


AC_ARG_ENABLE([lang-indic],
  [AS_HELP_STRING([--disable-lang-indic],
      [disable Indic language support])],
  [],
  [enable_lang_indic=${enable_lang_all}])

AC_ARG_ENABLE([lang-es],
  [AS_HELP_STRING([--enable-lang-es],
      [(NOT WORKING YET) enable Spanish language support])],
  [],
  [enable_lang_es=no])


dnl ----------------- Voices -------------------------------

AC_ARG_ENABLE([voices-english],
  [AS_HELP_STRING([--disable-voices-english],
      [disable embedding all English voices])],
  [],
  [enable_voices_english=${enable_voices_all}])

dnl cmu_us_kal
AC_ARG_ENABLE([cmu_us_kal],
  [AS_HELP_STRING([--disable-cmu_us_kal],
       [disable cmu_us_kal 8kHz diphone voice])],
  [],
  [enable_cmu_us_kal=${enable_voices_english}])

dnl: Dependencies for cmu_us_kal
AS_IF([test "x$enable_cmu_us_kal" != xno -a "x${enable_lang_english}" = "xno" ],
  [enable_lang_english="yes"])

dnl cmu_time_awb
AC_ARG_ENABLE([cmu_time_awb],
  [AS_HELP_STRING([--disable-cmu_time_awb],
       [disable cmu_time_awb clunits voice])],
  [],
  [enable_cmu_time_awb=${enable_voices_english}])

dnl: Dependencies for cmu_time_awb
AS_IF([test "x$enable_cmu_time_awb" != xno -a "x${enable_lang_english}" = "xno" ],
  [enable_lang_english="yes"])

dnl cmu_us_kal16
AC_ARG_ENABLE([cmu_us_kal16],
  [AS_HELP_STRING([--disable-cmu_us_kal16],
       [disable cmu_us_kal16 16kHz diphone voice])],
  [],
  [enable_cmu_us_kal16=${enable_voices_english}])

dnl: Dependencies for cmu_us_kal16
AS_IF([test "x$enable_cmu_us_kal16" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])

dnl cmu_us_awb
AC_ARG_ENABLE([cmu_us_awb],
  [AS_HELP_STRING([--disable-cmu_us_awb],
       [disable cmu_us_awb clustergen voice])],
  [],
  [enable_cmu_us_awb=${enable_voices_english}])

dnl: Dependencies for cmu_us_awb
AS_IF([test "x$enable_cmu_us_awb" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])

dnl cmu_us_rms
AC_ARG_ENABLE([cmu_us_rms],
  [AS_HELP_STRING([--disable-cmu_us_rms],
       [disable cmu_us_rms clustergen voice])],
  [],
  [enable_cmu_us_rms=${enable_voices_english}])

dnl: Dependencies for cmu_us_rms
AS_IF([test "x$enable_cmu_us_rms" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])

dnl cmu_us_slt
AC_ARG_ENABLE([cmu_us_slt],
  [AS_HELP_STRING([--disable-cmu_us_slt],
       [disable cmu_us_slt clustergen voice])],
  [],
  [enable_cmu_us_slt=${enable_voices_english}])

dnl: Dependencies for cmu_us_slt
AS_IF([test "x$enable_cmu_us_slt" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])


dnl cmu_us_slt_hts
AC_ARG_ENABLE([cmu_us_slt_hts],
  [AS_HELP_STRING([--disable-cmu_us_slt_hts],
       [disable cmu_us_slt_hts hts voice])],
  [],
  [enable_cmu_us_slt_hts=${enable_voices_english}])

AS_IF([test "x$enable_cmu_us_slt_hts" != "xno" -a "x$with_hts" = "xno"],
    [enable_cmu_us_slt_hts="no"
          AC_MSG_WARN([cmu_us_slt_hts disabled because hts support is not available])],
    [])

dnl: Dependencies for cmu_us_slt_hts
AS_IF([test "x$enable_cmu_us_slt_hts" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])


dnl vid_gb_ap
AC_ARG_ENABLE([vid_gb_ap],
  [AS_HELP_STRING([--disable-vid_gb_ap],
       [disable vid_gb_ap clustergen voice])],
  [],
  [enable_vid_gb_ap=${enable_voices_english}])

dnl: Dependencies for vid_gb_ap
AS_IF([test "x$enable_vid_gb_ap" != xno -a "x${enable_lang_english}" = "xno"],
  [enable_lang_english="yes"])

dnl To generate the pkg-config file .pc we want the libraries in the order
dnl the linker will need them. For that reason, we first list voices, then
dnl languages and we append the lib required on each case.

AM_CONDITIONAL([VOICE_CMU_US_KAL], [test "x$enable_cmu_us_kal" != xno])
AS_IF([test "x$enable_cmu_us_kal" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_KAL], [1], [Enable cmu_us_kal voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_kal"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_KAL], [0], [Enable cmu_us_kal voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_KAL_BUILTIN], [test "x$enable_cmu_us_kal" = "xyes"])
AS_IF([test "x${enable_cmu_us_kal}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_KAL_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_KAL_BUILTIN],[0],[Do we link the voice at compilation time?])])

AM_CONDITIONAL([VOICE_CMU_TIME_AWB], [test "x$enable_cmu_time_awb" != xno])
AS_IF([test "x$enable_cmu_time_awb" != xno ],
  [ AC_DEFINE([ENABLE_CMU_TIME_AWB], [1], [Enable cmu_time_awb voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_time_awb"
  ],
  [ AC_DEFINE([ENABLE_CMU_TIME_AWB], [0], [Enable cmu_time_awb voice]) ])

AM_CONDITIONAL([ENABLE_CMU_TIME_AWB_BUILTIN], [test "x$enable_cmu_time_awb" = "xyes"])
AS_IF([test "x${enable_cmu_time_awb}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_TIME_AWB_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_TIME_AWB_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([VOICE_CMU_US_KAL16], [test "x$enable_cmu_us_kal16" != xno])
AS_IF([test "x$enable_cmu_us_kal16" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_KAL16], [1], [Enable cmu_us_kal16 voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_kal16"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_KAL16], [0], [Enable cmu_us_kal16 voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_KAL16_BUILTIN], [test "x$enable_cmu_us_kal16" = "xyes"])
AS_IF([test "x${enable_cmu_us_kal16}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_KAL16_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_KAL16_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([VOICE_CMU_US_AWB], [test "x$enable_cmu_us_awb" != xno])
AS_IF([test "x$enable_cmu_us_awb" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_AWB], [1], [Enable cmu_us_awb voice]) 
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_awb"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_AWB], [0], [Enable cmu_us_awb voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_AWB_BUILTIN], [test "x$enable_cmu_us_awb" = "xyes"])
AS_IF([test "x${enable_cmu_us_awb}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_AWB_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_AWB_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([VOICE_CMU_US_RMS], [test "x$enable_cmu_us_rms" != xno])
AS_IF([test "x$enable_cmu_us_rms" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_RMS], [1], [Enable cmu_us_rms voice]) 
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_rms"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_RMS], [0], [Enable cmu_us_rms voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_RMS_BUILTIN], [test "x$enable_cmu_us_rms" = "xyes"])
AS_IF([test "x${enable_cmu_us_rms}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_RMS_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_RMS_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([VOICE_CMU_US_SLT], [test "x$enable_cmu_us_slt" != xno])
AS_IF([test "x$enable_cmu_us_slt" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_SLT], [1], [Enable cmu_us_slt voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_slt"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_SLT], [0], [Enable cmu_us_slt voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_SLT_BUILTIN], [test "x$enable_cmu_us_slt" = "xyes"])
AS_IF([test "x${enable_cmu_us_slt}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_SLT_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_SLT_BUILTIN],[0],[Do we link the voice at compilation time?])])

AM_CONDITIONAL([VOICE_CMU_US_SLT_HTS], [test "x$enable_cmu_us_slt_hts" != xno])
AS_IF([test "x$enable_cmu_us_slt_hts" != xno ],
  [ AC_DEFINE([ENABLE_CMU_US_SLT_HTS], [1], [Enable cmu_us_slt_hts voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_cmu_us_slt_hts"
  ],
  [ AC_DEFINE([ENABLE_CMU_US_SLT_HTS], [0], [Enable cmu_us_slt_hts voice]) ])

AM_CONDITIONAL([ENABLE_CMU_US_SLT_HTS_BUILTIN], [test "x$enable_cmu_us_slt_hts" = "xyes"])
AS_IF([test "x${enable_cmu_us_slt_hts}" = "xyes" ],
  [AC_DEFINE([ENABLE_CMU_US_SLT_HTS_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_CMU_US_SLT_HTS_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([VOICE_VID_GB_AP], [test "x$enable_vid_gb_ap" != xno])
AS_IF([test "x$enable_vid_gb_ap" != xno ],
  [ AC_DEFINE([ENABLE_VID_GB_AP], [1], [Enable vid_gb_ap voice])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_vid_gb_ap"
  ],
  [ AC_DEFINE([ENABLE_VID_GB_AP], [0], [Enable vid_gb_ap voice]) ])

AM_CONDITIONAL([ENABLE_VID_GB_AP_BUILTIN], [test "x$enable_vid_gb_ap" = "xyes"])
AS_IF([test "x${enable_vid_gb_ap}" = "xyes" ],
  [AC_DEFINE([ENABLE_VID_GB_AP_BUILTIN],[1],[Do we link the voice at compilation time?])],
  [AC_DEFINE([ENABLE_VID_GB_AP_BUILTIN],[0],[Do we link the voice at compilation time?])])


AM_CONDITIONAL([LANG_INDIC], [test "x$enable_lang_indic" != xno])
AS_IF([test "x$enable_lang_indic" != xno ],
  [ AC_DEFINE([ENABLE_LANG_INDIC], [1], [Enable Indic language])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_indic"
  ],
  [ AC_DEFINE([ENABLE_LANG_INDIC], [0], [Enable Indic language]) ])

AM_CONDITIONAL([ENABLE_LANG_INDIC_BUILTIN], [test "x$enable_lang_indic" = "xyes"])
AS_IF([test "x${enable_lang_indic}" = "xyes" ],
  [AC_DEFINE([ENABLE_LANG_INDIC_BUILTIN],[1],[Do we link the language at compilation time?])],
  [AC_DEFINE([ENABLE_LANG_INDIC_BUILTIN],[0],[Do we link the language at compilation time?])])

AM_CONDITIONAL([LANG_GRAPHEME], [test "x$enable_lang_grapheme" != xno])
AS_IF([test "x$enable_lang_grapheme" != xno ],
  [ AC_DEFINE([ENABLE_LANG_GRAPHEME], [1], [Enable Grapheme language support])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_grapheme"
  ],
  [ AC_DEFINE([ENABLE_LANG_GRAPHEME], [0], [Enable Grapheme language support]) ])

AM_CONDITIONAL([ENABLE_LANG_GRAPHEME_BUILTIN], [test "x$enable_lang_grapheme" = "xyes"])
AS_IF([test "x${enable_lang_grapheme}" = "xyes" ],
  [AC_DEFINE([ENABLE_LANG_GRAPHEME_BUILTIN],[1],[Do we link the language at compilation time?])],
  [AC_DEFINE([ENABLE_LANG_GRAPHEME_BUILTIN],[0],[Do we link the language at compilation time?])])


AM_CONDITIONAL([LANG_ENGLISH], [test "x$enable_lang_english" != "xno"])
AS_IF([test "x$enable_lang_english" != "xno" ],
  [ AC_DEFINE([ENABLE_LANG_ENGLISH], [1], [Enable US English language])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_usenglish" ],
  [ AC_DEFINE([ENABLE_LANG_ENGLISH], [0], [Enable US English language]) ])

AM_CONDITIONAL([ENABLE_LANG_ENGLISH_BUILTIN], [test "x$enable_lang_english" = "xyes"])
AS_IF([test "x${enable_lang_english}" = "xyes" ],
  [AC_DEFINE([ENABLE_LANG_ENGLISH_BUILTIN],[1],[Do we link the language at compilation time?])],
  [AC_DEFINE([ENABLE_LANG_ENGLISH_BUILTIN],[0],[Do we link the language at compilation time?])])


AM_CONDITIONAL([LANG_ES], [test "x$enable_lang_es" != xno])
AS_IF([test "x$enable_lang_es" != xno ],
  [ AC_DEFINE([ENABLE_LANG_ES], [1], [Enable Spanish language])
    PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimic_es" ],
  [ AC_DEFINE([ENABLE_LANG_ES], [0], [Enable Spanish language]) ])

AM_CONDITIONAL([ENABLE_LANG_ES_BUILTIN], [test "x$enable_lang_es" = "xyes"])
AS_IF([test "x${enable_lang_es}" = "xyes" ],
  [AC_DEFINE([ENABLE_LANG_ES_BUILTIN],[1],[Do we link the language at compilation time?])],
  [AC_DEFINE([ENABLE_LANG_ES_BUILTIN],[0],[Do we link the language at compilation time?])])



PKGCONFIG_MIMIC_LIBS="${PKGCONFIG_MIMIC_LIBS} -lttsmimiccore"

AC_SUBST(PKGCONFIG_MIMIC_CORE_CFLAGS)
AC_SUBST(PKGCONFIG_MIMIC_CORE_LIBS)
AC_SUBST(PKGCONFIG_MIMIC_CORE_DEPS)

AC_SUBST(PKGCONFIG_MIMIC_CFLAGS)
AC_SUBST(PKGCONFIG_MIMIC_LIBS)
AC_SUBST(PKGCONFIG_MIMIC_DEPS)

AC_SUBST(EXEEXT)

LIBPREFIX="lib"
LIBEXTENSION="so"
AC_DEFINE([MIMIC_PLUGIN_PREFIX], ["libttsmimic_"], [Prefix used in plugins])
AC_DEFINE([MIMIC_PLUGIN_EXT], ["so"], [Extension used in plugins])

AC_CONFIG_FILES([Makefile libttsmimiccore.pc libttsmimic.pc])
AC_OUTPUT

AS_IF( [ test "${AUDIODRIVER}" = "none" && test "$with_audio" != "none" ],
  [ AC_MSG_WARN([No audio driver could be determined, and no sound output will be possible. Mimic will work but only be able to write output to disk.])
  ])


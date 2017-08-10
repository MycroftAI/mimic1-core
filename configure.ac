dnl Use autogen.sh to regenerate the configure script
AC_PREREQ([2.62])
AC_INIT([mimic-core],
        [1.1.0.9000],
        [https://github.com/MycroftAI/mimic-core/issues])

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
AC_CONFIG_HEADERS([include/mimic_core_config.h])
LT_PREREQ([2.2.6])
AC_C_BIGENDIAN

PKG_PROG_PKG_CONFIG

dnl By default enable shared libraries.
dnl use ./configure --enable-static (and/or --disable-shared)
dnl to build them or not.


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
PKGCONFIG_MIMIC_CORE_CFLAGS="-I${includedir}/ttsmimic "'-DMIMIC_PLUGIN_DIR=${libdir}/ttsmimic/plugins'
PKGCONFIG_MIMIC_CORE_LIBS="-L${libdir}"
PKGCONFIG_MIMIC_CORE_DEPS=

dnl Check libpcre2-8
PKG_CHECK_MODULES([PCRE2], [libpcre2-8])
PKGCONFIG_MIMIC_CORE_DEPS="${PKGCONFIG_MIMIC_CORE_DEPS} libpcre2-8"

AC_SEARCH_LIBS([dlopen], [dl dld],
  [ enable_plugins="yes"
    AC_DEFINE([MIMIC_ENABLE_PLUGINS], [1], [Define if you want plugin support])
    PKGCONFIG_MIMIC_CORE_LIBS="${PKGCONFIG_MIMIC_LIBS} -ldl"
  ],
  [  AC_MSG_NOTICE([dlopen not available, disabling plugin support])
     enable_plugins="no"
  ])
AM_CONDITIONAL([MIMIC_ENABLE_PLUGINS], [test "x$enable_plugins" = "xyes"])


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

AC_SUBST(PKGCONFIG_MIMIC_CORE_CFLAGS)
AC_SUBST(PKGCONFIG_MIMIC_CORE_LIBS)
AC_SUBST(PKGCONFIG_MIMIC_CORE_DEPS)

dnl estoy aqui
AC_SUBST(EXEEXT)

LIBPREFIX="lib"
LIBEXTENSION="so"
AC_DEFINE([MIMIC_PLUGIN_PREFIX], ["libttsmimic_"], [Prefix used in plugins])
AC_DEFINE([MIMIC_PLUGIN_EXT], ["so"], [Extension used in plugins])

AC_CONFIG_FILES([Makefile libttsmimiccore.pc])
AC_OUTPUT

AS_IF( [ test "${AUDIODRIVER}" = "none" && test "$with_audio" != "none" ],
  [ AC_MSG_WARN([No audio driver could be determined, and no sound output will be possible. Mimic will work but only be able to write output to disk.])
  ])


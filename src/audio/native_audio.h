/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2001                             */
/*                        All Rights Reserved.                           */
/*                                                                       */
/*  Permission is hereby granted, free of charge, to use and distribute  */
/*  this software and its documentation without restriction, including   */
/*  without limitation the rights to use, copy, modify, merge, publish,  */
/*  distribute, sublicense, and/or sell copies of this work, and to      */
/*  permit persons to whom this work is furnished to do so, subject to   */
/*  the following conditions:                                            */
/*   1. The code must retain the above copyright notice, this list of    */
/*      conditions and the following disclaimer.                         */
/*   2. Any modifications must be clearly marked as such.                */
/*   3. Original authors' names are not deleted.                         */
/*   4. The authors' names are not used to endorse or promote products   */
/*      derived from this software without specific prior written        */
/*      permission.                                                      */
/*                                                                       */
/*  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         */
/*  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      */
/*  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   */
/*  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      */
/*  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    */
/*  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   */
/*  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          */
/*  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       */
/*  THIS SOFTWARE.                                                       */
/*                                                                       */
/*************************************************************************/
/*             Author:  Alan W Black (awb@cs.cmu.edu)                    */
/*               Date:  January 2001                                     */
/*************************************************************************/
/*                                                                       */
/*  Selects between audio devices                                        */
/*                                                                       */
/*************************************************************************/

#ifndef _NATIVE_AUDIO_H__
#define _NATIVE_AUDIO_H__

#include "mimic_core_config.h"

#ifdef CST_AUDIO_COMMAND

#define AUDIO_INIT_NATIVE audio_init_command
#define AUDIO_OPEN_NATIVE audio_open_command
#define AUDIO_CLOSE_NATIVE audio_close_command
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_command
#define AUDIO_WRITE_NATIVE audio_write_command
#define AUDIO_DRAIN_NATIVE audio_drain_command
#define AUDIO_FLUSH_NATIVE audio_flush_command
#define AUDIO_EXIT_NATIVE audio_exit_command

#endif

#ifdef CST_AUDIO_SUNOS

#define AUDIO_INIT_NATIVE audio_init_sun
#define AUDIO_OPEN_NATIVE audio_open_sun
#define AUDIO_CLOSE_NATIVE audio_close_sun
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_sun
#define AUDIO_WRITE_NATIVE audio_write_sun
#define AUDIO_DRAIN_NATIVE audio_drain_sun
#define AUDIO_FLUSH_NATIVE audio_flush_sun
#define AUDIO_EXIT_NATIVE audio_exit_sun
#endif

#ifdef CST_AUDIO_ALSA

#define AUDIO_INIT_NATIVE audio_init_alsa
#define AUDIO_OPEN_NATIVE audio_open_alsa
#define AUDIO_CLOSE_NATIVE audio_close_alsa
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_alsa
#define AUDIO_WRITE_NATIVE audio_write_alsa
#define AUDIO_DRAIN_NATIVE audio_drain_alsa
#define AUDIO_FLUSH_NATIVE audio_flush_alsa
#define AUDIO_EXIT_NATIVE audio_exit_alsa

#endif

#ifdef CST_AUDIO_PULSEAUDIO

#define AUDIO_INIT_NATIVE audio_init_pulseaudio
#define AUDIO_OPEN_NATIVE audio_open_pulseaudio
#define AUDIO_CLOSE_NATIVE audio_close_pulseaudio
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_pulseaudio
#define AUDIO_WRITE_NATIVE audio_write_pulseaudio
#define AUDIO_DRAIN_NATIVE audio_drain_pulseaudio
#define AUDIO_FLUSH_NATIVE audio_flush_pulseaudio
#define AUDIO_EXIT_NATIVE audio_exit_pulseaudio

#endif

#ifdef CST_AUDIO_FREEBSD

#define AUDIO_INIT_NATIVE audio_init_oss
#define AUDIO_OPEN_NATIVE audio_open_oss
#define AUDIO_CLOSE_NATIVE audio_close_oss
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_oss
#define AUDIO_WRITE_NATIVE audio_write_oss
#define AUDIO_DRAIN_NATIVE audio_drain_oss
#define AUDIO_FLUSH_NATIVE audio_flush_oss
#define AUDIO_EXIT_NATIVE audio_exit_oss

#endif

#ifdef CST_AUDIO_WINCE

#define AUDIO_INIT_NATIVE audio_init_wince
#define AUDIO_OPEN_NATIVE audio_open_wince
#define AUDIO_CLOSE_NATIVE audio_close_wince
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_wince
#define AUDIO_WRITE_NATIVE audio_write_wince
#define AUDIO_DRAIN_NATIVE audio_drain_wince
#define AUDIO_FLUSH_NATIVE audio_flush_wince
#define AUDIO_EXIT_NATIVE audio_exit_wince

#endif

#ifdef CST_AUDIO_WIN32

#define AUDIO_INIT_NATIVE audio_init_wince
#define AUDIO_OPEN_NATIVE audio_open_wince
#define AUDIO_CLOSE_NATIVE audio_close_wince
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_wince
#define AUDIO_WRITE_NATIVE audio_write_wince
#define AUDIO_DRAIN_NATIVE audio_drain_wince
#define AUDIO_FLUSH_NATIVE audio_flush_wince
#define AUDIO_EXIT_NATIVE audio_exit_wince

#endif

#ifdef CST_AUDIO_NONE

#define AUDIO_INIT_NATIVE audio_init_none
#define AUDIO_OPEN_NATIVE audio_open_none
#define AUDIO_CLOSE_NATIVE audio_close_none
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_none
#define AUDIO_WRITE_NATIVE audio_write_none
#define AUDIO_DRAIN_NATIVE audio_drain_none
#define AUDIO_FLUSH_NATIVE audio_flush_none
#define AUDIO_EXIT_NATIVE audio_exit_none

#endif

#ifdef CST_AUDIO_PORTAUDIO

#define AUDIO_INIT_NATIVE audio_init_portaudio
#define AUDIO_OPEN_NATIVE audio_open_portaudio
#define AUDIO_CLOSE_NATIVE audio_close_portaudio
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_portaudio
#define AUDIO_WRITE_NATIVE audio_write_portaudio
#define AUDIO_DRAIN_NATIVE audio_drain_portaudio
#define AUDIO_FLUSH_NATIVE audio_flush_portaudio
#define AUDIO_EXIT_NATIVE audio_exit_portaudio

#endif

#ifndef AUDIO_OPEN_NATIVE

#define AUDIO_INIT_NATIVE audio_init_none
#define AUDIO_OPEN_NATIVE audio_open_none
#define AUDIO_CLOSE_NATIVE audio_close_none
#define AUDIO_SET_SAMPLE_RATE_NATIVE audio_set_sample_rate_none
#define AUDIO_WRITE_NATIVE audio_write_none
#define AUDIO_DRAIN_NATIVE audio_drain_none
#define AUDIO_FLUSH_NATIVE audio_flush_none
#define AUDIO_EXIT_NATIVE audio_exit_none
#define CST_AUDIO_NONE

#endif

int AUDIO_INIT_NATIVE();
cst_audiodev *AUDIO_OPEN_NATIVE(int sps, int channels, cst_audiofmt fmt);
int AUDIO_CLOSE_NATIVE(cst_audiodev *ad);
int AUDIO_WRITE_NATIVE(cst_audiodev *ad, void *buff, int num_bytes);
int AUDIO_DRAIN_NATIVE(cst_audiodev *ad);
int AUDIO_FLUSH_NATIVE(cst_audiodev *ad);
int AUDIO_EXIT_NATIVE();

extern volatile int shutdown_request;
#endif

/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 1999                             */
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
/*               Date:  December 1999                                    */
/*************************************************************************/
/*                                                                       */
/*  Light weight run-time speech synthesis system, public API            */
/*                                                                       */
/*************************************************************************/
#ifndef _MIMIC_H__
#define _MIMIC_H__

#ifdef __cplusplus
extern "C" {
#endif                          /* __cplusplus */

#include "mimic_core_config.h"
#include "cst_lib_visibility.h"
#include "cst_string.h"
#include "cst_regex.h"
#include "cst_val.h"
#include "cst_features.h"
#include "cst_item.h"
#include "cst_relation.h"
#include "cst_utterance.h"
#include "cst_wave.h"
#include "cst_track.h"

#include "cst_cart.h"
#include "cst_phoneset.h"
#include "cst_voice.h"
#include "cst_audio.h"

#include "cst_utt_utils.h"
#include "cst_lexicon.h"
#include "cst_synth.h"
#include "cst_units.h"
#include "cst_tokenstream.h"
#include "cst_plugins.h"

typedef struct mimic_context_s {
    uint16_t version;
    cst_voice **voices;
    cst_lang **languages;
    mimic_plugin_handler_t **plugins;
    uint32_t num_voices;
    uint32_t voices_size;
    uint32_t num_languages;
    uint32_t languages_size;
    cst_voice *voice_selected;
} mimic_context;

/* Public functions */
MIMIC_CORE_PUBLIC mimic_context *mimic_core_init();
MIMIC_CORE_PUBLIC int mimic_core_exit(mimic_context *ctx);
MIMIC_CORE_PUBLIC mimic_context *mimic_core_init_no_plugins();
MIMIC_CORE_PUBLIC int mimic_core_exit_no_plugins(mimic_context *ctx);

/* General top level functions */
MIMIC_CORE_PUBLIC const cst_lang *mimic_lang_select(mimic_context *ctx, const char *lang);
MIMIC_CORE_PUBLIC cst_voice *mimic_voice_select(mimic_context *ctx, const char *name);
MIMIC_CORE_PUBLIC cst_voice *mimic_voice_load(mimic_context *ctx, const char *filename);
MIMIC_CORE_PUBLIC int mimic_voice_dump(cst_voice *voice, const char *voice_filename);
MIMIC_CORE_PUBLIC int mimic_file_to_speech(const char *filename, cst_voice *voice,
                             const char *outtype, float *dur);
MIMIC_CORE_PUBLIC int mimic_text_to_speech(const char *text, cst_voice *voice,
                             const char *outtype, float *dur);
MIMIC_CORE_PUBLIC int mimic_phones_to_speech(const char *text, cst_voice *voice,
                               const char *outtype, float *dur);
MIMIC_CORE_PUBLIC int mimic_ssml_file_to_speech(
    mimic_context *ctx,const char *filename, cst_voice *voice,
    const char *outtype, float *dur);
MIMIC_CORE_PUBLIC int mimic_ssml_text_to_speech(
    mimic_context *ctx, const char *text, cst_voice *voice,
    const char *outtype, float *dur);
MIMIC_CORE_PUBLIC int mimic_voice_add_lex_addenda(cst_voice *v, const cst_string *lexfile);

/* Lower lever user functions */
    cst_wave *mimic_text_to_wave(const char *text, cst_voice *voice);
    MIMIC_CORE_PUBLIC cst_utterance *mimic_synth_text(const char *text, cst_voice *voice);
    cst_utterance *mimic_synth_phones(const char *phones, cst_voice *voice);

    int mimic_ts_to_speech(cst_tokenstream *ts, cst_voice *voice,
                           const char *outtype, float *dur);
    cst_utterance *mimic_do_synth(cst_utterance *u,
                                  cst_voice *voice, cst_uttfunc synth);
    int mimic_process_output(cst_utterance *u,
                             const char *outtype, int append, float *dur);

/* for voices with external voxdata */
    int mimic_mmap_clunit_voxdata(const char *voxdir, cst_voice *voice);
    int mimic_munmap_clunit_voxdata(cst_voice *voice);

/* mimic public export wrappers for features access */
    MIMIC_CORE_PUBLIC int mimic_get_param_int(const cst_features *f, const char *name, int def);
    MIMIC_CORE_PUBLIC float mimic_get_param_float(const cst_features *f, const char *name,
                                float def);
    MIMIC_CORE_PUBLIC const char *mimic_get_param_string(const cst_features *f,
                                       const char *name, const char *def);
    MIMIC_CORE_PUBLIC const cst_val *mimic_get_param_val(const cst_features *f,
                                       const char *name, cst_val *def);
    MIMIC_CORE_PUBLIC void mimic_feat_set_int(cst_features *f, const char *name, int v);
    MIMIC_CORE_PUBLIC void mimic_feat_set_float(cst_features *f, const char *name, float v);
    MIMIC_CORE_PUBLIC void mimic_feat_set_string(cst_features *f, const char *name,
                               const char *v);
    MIMIC_CORE_PUBLIC void mimic_feat_set(cst_features *f, const char *name, const cst_val *v);
    MIMIC_CORE_PUBLIC int mimic_feat_remove(cst_features *f, const char *name);

    MIMIC_CORE_PUBLIC const char *mimic_ffeature_string(const cst_item *item,
                                                        const char *featpath);
    int mimic_ffeature_int(const cst_item *item, const char *featpath);
    float mimic_ffeature_float(const cst_item *item, const char *featpath);
    const cst_val *mimic_ffeature(const cst_item *item, const char *featpath);
    cst_item *mimic_path_to_item(const cst_item *item, const char *featpath);

/* These functions are *not* thread-safe, they are designed to be called */
/* before the initial synthesis occurs */
MIMIC_CORE_PUBLIC int mimic_voice_add(mimic_context *ctx, cst_voice *voice);
    MIMIC_CORE_PUBLIC int mimic_lang_add(mimic_context *ctx, const char *langname,
                       void (*lang_init) (cst_voice *vox),
                       cst_lexicon *(*lex_init) ());
/* These are init functions for generic grapheme based voices */
    void utf8_grapheme_lang_init(cst_voice *v);
    cst_lexicon *utf8_grapheme_lex_init(void);

#ifdef __cplusplus
}                               /* extern "C" */
#endif                          /* __cplusplus */
#endif

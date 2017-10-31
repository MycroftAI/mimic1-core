/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                         Copyright (c) 2001                            */
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
/*  For text to phones                                                   */
/*                                                                       */
/*************************************************************************/

#include <stdio.h>
#include <string.h>

#include "cst_args.h"
#include "cst_error.h"
#include "mimic.h"

static cst_utterance *no_wave_synth(cst_utterance *u)
{
    return u;
}

static cst_voice *voice_no_wave(mimic_context *ctx, const char *lang_code, const char *dialect,
                                int use_addenda, int use_lexicon)
{
    const cst_lang *lang = mimic_lang_select(ctx, lang_code);
    cst_voice *v = new_voice();

    v->name = "no_wave_voice";

    /* Set up basic values for synthesizing with this voice */
    if (lang == NULL)
    {
       cst_errmsg("Language %s not found\n", lang_code);
       return NULL;
    }

    if (lang->lang_init != NULL)
    {
        lang->lang_init(v);
    }
    mimic_feat_set_string(v->features, "name", "voice_no_wave");
    if (dialect != NULL)
    {
        mimic_feat_set_string(v->features, "dialect", dialect);
    }

    /* Lexicon */
    if (lang->lex_init != NULL)
    {
        cst_lexicon *lex;
        lex = lang->lex_init();
        feat_set(v->features, "lexicon", lexicon_val(lex));
        if (lex->postlex != NULL)
        {
            feat_set(v->features, "postlex_func", uttfunc_val(lex->postlex));
        }
    }
    mimic_feat_set_int(v->features, "use_addenda", use_addenda);
    mimic_feat_set_int(v->features, "use_lexicon", use_lexicon);

    /* Intonation */
    mimic_feat_set_string(v->features, "no_f0_target_model", "1");
    mimic_feat_set_string(v->features, "no_segment_duration_model", "1");

    /* Waveform synthesis: diphone_synth */
    feat_set(v->features, "wave_synth_func", uttfunc_val(&no_wave_synth));

    return v;
}

int main(int argc, char **argv)
{
    cst_val *files;
    cst_features *args = new_features();
    cst_voice *v;
    cst_utterance *u;
    cst_item *s;
    const char *text, *lang, *dialect;
    int use_addenda = 1;
    int use_lexicon = 1;

    files =
        cst_args(argv, argc,
                 "usage: t2p [OPTIONS]\n"
                 "-lang <string> Language code to transcribe\n"
                 "-dialect <string>  Dialect\n"
                 "-disable_addenda <binary>  Disable addenda lookup\n"
                 "-disable_lexicon <binary>  Disable lexicon lookup\n"
                 "-t <string>  Text\n"
                 "usage: t2p -lang en -t \"word word word\"\n"
                 "Convert text to phonemes.", args);

    text = get_param_string(args, "-t", NULL);
    if (text == NULL)
    {
        fprintf(stderr, "no text specified\n");
        exit(-1);
    }
    lang = get_param_string(args, "-lang", "en");
    dialect = get_param_string(args, "-dialect", NULL);
    if (cst_streq(get_param_string(args, "-disable_addenda", "false"), "true"))
    {
        use_addenda = 0;
    }
    if (cst_streq(get_param_string(args, "-disable_lexicon", "false"), "true"))
    {
        use_lexicon = 0;
    }
    mimic_context *ctx = mimic_core_init();
    v = voice_no_wave(ctx, lang, dialect, use_addenda, use_lexicon);
    if (v == NULL)
    {
       cst_errmsg("Could not load voice with language '%s' and dialect '%s'\n", lang, dialect == NULL ? "" : dialect);
       exit(-1);
    }

    u = mimic_synth_text(text, v);

    for (s = relation_head(utt_relation(u, "Segment")); s; s = item_next(s))
    {
        const char *name = item_feat_string(s, "name");
        printf("%s", name);
        /* If its a vowel and is stessed output stress value */
        if ((cst_streq("+", ffeature_string(s, "ph_vc"))) &&
            (cst_streq("1", ffeature_string(s, "R:SylStructure.parent.stress"))))
            printf("1");
        printf(" ");
    }

    printf("\n");

    delete_utterance(u);
    delete_features(args);
    delete_val(files);

    mimic_core_exit(ctx);
    return 0;
}

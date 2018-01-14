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
/*  Letter to sound rule support                                         */
/*                                                                       */
/*************************************************************************/

#include "cst_lts.h"
#include "cst_features.h"
#include <stdlib.h>

#define MIMIC_WORD_LIMIT 1
#define MIMIC_OUT_BOUNDS 2

static cst_lts_phone apply_model(cst_lts_letter * x_vals, cst_lts_addr start,
                                 const cst_lts_rule * model);

cst_lts_rules *new_lts_rules()
{
    cst_lts_rules *lt = cst_alloc(cst_lts_rules, 1);
    lt->name = NULL;
    lt->model = NULL;
    lt->phone_table = NULL;
    lt->context_window_size = 0;
    lt->context_extra_feats = 0;
    lt->letter_index = NULL;
    return lt;
}

static int word_to_buff(const char *word, int ctx_size,
                        cst_lts_letter ** full_buff)
{
    /* Given an UTF-8 encoded word "potato",
     * create full_buff, as an integer vector like
     * 2 2 2 1 p o t a t o 1 2 2 2
     * where the characters are unicode code points.
     * Returns the number of code points in word 
     */
    const cst_val *v;
    size_t i, i2, num_cp_in_word;
    cst_val *utflets = NULL;
    *full_buff = cst_alloc(cst_lts_letter, 2 * ctx_size + cst_strlen(word));
    for (i = 0; i < ctx_size - 1; i++)
    {
        (*full_buff)[i] = MIMIC_OUT_BOUNDS;
    }
    (*full_buff)[i] = MIMIC_WORD_LIMIT;
    ++i;
    /* the word */
    utflets = cst_utf8_explode(word);
    /* For each UTF-8 character */
    num_cp_in_word = 0;
    for (v = utflets; v; v = val_cdr(v), ++i)
    {
        (*full_buff)[i] = utf8char_to_cp(val_string(val_car(v)));
        ++num_cp_in_word;
    }
    delete_val(utflets);
    (*full_buff)[i] = MIMIC_WORD_LIMIT;
    ++i;
    for (i2 = i; i2 < i + ctx_size - 1; i2++)
    {
        (*full_buff)[i2] = MIMIC_OUT_BOUNDS;
    }
    /*
       printf("Full buff: ");
       for (size_t jjj = 0; jjj <  2*ctx_size + num_cp_in_word; ++jjj)
       {
       printf("%d ", full_buff[jjj]);
       }
       printf("\n");
     */
    return num_cp_in_word;
}

cst_val *lts_apply(const char *word, const char *feats,
                   const cst_lts_rules *r)
{
    int pos;
    size_t i, i2, i3, num_cp_in_word;
    cst_val *phones = NULL;
    cst_lts_letter *fval_buff;
    cst_lts_letter *full_buff = NULL;
    cst_lts_phone phone;
    char *left, *right, *p;
    unsigned char utf8char[5];
    if (r->letter_index == NULL)
    {
        cst_errmsg("The letter_index that gives the initial rule for a given "
                   "unicode code point is missing. Malformed LTS rules\n");
        return NULL;
    }
    /* Buffer with added contexts */
    num_cp_in_word = word_to_buff(word, r->context_window_size, &full_buff);

    /* For feature vals for each letter */
    fval_buff = cst_alloc(cst_lts_letter, (r->context_window_size * 2) +
                          r->context_extra_feats);

    /* Do the prediction backwards so we don't need to reverse the answer */
    for (pos = r->context_window_size + num_cp_in_word - 1;
         full_buff[pos] != MIMIC_WORD_LIMIT; pos--)
    {
        int index;
        /* Fill the features buffer for the predictor */
        /* This is the context before the letter: */
        for (i = 0; i < r->context_window_size; ++i)
        {
            fval_buff[i] = full_buff[pos - r->context_window_size + i];
        }
        /* This is the context after the letter: */
        for (i2 = 0; i2 < r->context_window_size; ++i2)
        {
            fval_buff[i + i2] = full_buff[pos + 1 + i2];
        }
        /* Any extra additional feature:
           TODO: The extra feats are not used anywhere in mimic -nor Flite that
           I (Sergio Oller)
           know of-. This loop is always skipped. Here we assume ASCII or single
           bytes, but
           feats could be UTF-8 parsed as well. If you need this, please open an
           issue.
         */
        for (i3 = 0; i3 < cst_strlen(feats); ++i3)
        {
            fval_buff[i + i2 + i3] = feats[i3];
        }
        /* full_buff[pos] contains the letter. We need to get the initial state
           in the LTS rules for that letter. In ASCII it is trivial as we can
           map 256 cases but in Unicode is not that simple as we may have up to
           2^21 cases (although usually less than 256) r->letter_index maps a
           code point that represents a letter to the initial rule to check
         */
        cp_to_utf8char(full_buff[pos], utf8char);
        index = cst_unicode_int_map(r->letter_index, utf8char, 0, 0);
        if (index == r->letter_index->not_found)
        {
            // cst_errmsg("lts:skipping unknown char \"%s\"\n", utf8char);
            continue;
        }
        phone = apply_model(fval_buff, index, r->model);
        /* delete epsilons and split dual-phones */
        if (cst_streq("epsilon", r->phone_table[phone]))
            continue;
        else if ((p = strchr(r->phone_table[phone], '-')) != NULL)
        {
            left =
                cst_substr(r->phone_table[phone], 0,
                           cst_strlen(r->phone_table[phone]) - cst_strlen(p));
            right = cst_substr(r->phone_table[phone],
                               (cst_strlen(r->phone_table[phone]) -
                                cst_strlen(p)) + 1, (cst_strlen(p) - 1));
            phones =
                cons_val(string_val(left),
                         cons_val(string_val(right), phones));
            cst_free(left);
            cst_free(right);
        }
        else
            phones = cons_val(string_val(r->phone_table[phone]), phones);
    }

    cst_free(full_buff);
    cst_free(fval_buff);

    return phones;
}

static inline cst_lts_feat get_feat(const cst_lts_feat_val feat_val)
{
    return (cst_lts_feat) ((feat_val & 0xFF000000) >> 24);
}

static inline cst_lts_letter get_val(const cst_lts_feat_val feat_val)
{
    return (cst_lts_letter) (feat_val & 0x001FFFFF);
}

static cst_lts_phone apply_model(cst_lts_letter * x_vals, cst_lts_addr start,
                                 const cst_lts_rule * model)
{
    cst_lts_addr nstate = start;
    cst_lts_feat feat = get_feat(model[nstate].feat_val);
    cst_lts_letter val = get_val(model[nstate].feat_val);
    /*printf("nstate: %d\n", nstate); */
    //printf("x_vals[0..8]: %d %d %d %d %d %d %d %d\n",
    //x_vals[0], x_vals[1], x_vals[2], x_vals[3], x_vals[4], x_vals[5], x_vals[6], x_vals[7]);
    for (; feat != CST_LTS_EOR;)
    {
        if (x_vals[feat] == val)
        {
            //printf("True: %d ->", nstate);
            nstate = model[nstate].qtrue;
            //printf(" %d \n", nstate);
        }
        else
        {
            //printf("False: %d ->", nstate);
            nstate = model[nstate].qfalse;
            //printf(" %d \n", nstate);
        }
        feat = get_feat(model[nstate].feat_val);
        val = get_val(model[nstate].feat_val);
    }
    return val;
}

#undef MIMIC_WORD_LIMIT
#undef MIMIC_OUT_BOUNDS

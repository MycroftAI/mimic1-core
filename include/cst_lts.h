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
/*  Letter to sound rules                                                */
/*                                                                       */
/*************************************************************************/
#ifndef CST_LTS_H
#define CST_LTS_H

#include "cst_lib_visibility.h"
#include "cst_string.h"
#include "cst_val.h"
#include <stdint.h>

typedef uint16_t cst_lts_addr;
/* The feat typically holds which context character is the rule considering or
 * CST_LTS_EOR if it is a terminal rule. As there are typically <=8 features, we
 * can use a uint8_t type, that can hold up to 255 features (+ terminal rule) */
typedef uint8_t cst_lts_feat;
/* cst_lts_phone and cst_lts_letter must be of the same type to save space.
 * They hold a unicode code point and therefore require 21 bits.
 */
typedef uint32_t cst_lts_letter;
typedef cst_lts_letter cst_lts_phone;
/* The feat_val type can hold both the cst_lts_feat and the cst_lts_letter type.
 * Because cst_lts_letter requires 21 bits and cst_lts_feat requires 8 bits,
 * there is a possible bit encoding to put both of them on a single uint32_t,
 * saving space.*/
typedef uint32_t cst_lts_feat_val;

typedef struct cst_lts_rule_struct {
    cst_lts_feat_val feat_val;
    cst_lts_addr qtrue;
    cst_lts_addr qfalse;
} cst_lts_rule;

/* end of rule value: If this feature is found, we are on a terminal node */
#define CST_LTS_EOR 255

typedef struct cst_lts_rules_struct {
    char *name;
    cst_lts_rule *model;
    const char *const *phone_table;
    int context_window_size;
    int context_extra_feats; /* Size of the extra features, measured in
                                cst_lts_letter */
    map_unicode_to_int *letter_index;
} cst_lts_rules;

cst_lts_rules *new_lts_rules();

MIMIC_CORE_PUBLIC cst_val *lts_apply(const char *word, const char *feats, const cst_lts_rules *r);

#endif

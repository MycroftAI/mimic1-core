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
/*  Item                                                                 */
/*                                                                       */
/*************************************************************************/
#ifndef CST_ITEM_H
#define CST_ITEM_H

#include "cst_lib_visibility.h"
#include "cst_file.h"
#include "cst_features.h"

/* Everyone needs these so forward define these */
typedef struct cst_relation_struct cst_relation;
typedef struct cst_utterance_struct cst_utterance;
typedef struct cst_item_struct cst_item;

/* So items, relations and utterances can be used as vals */
CST_VAL_USER_TYPE_DCLS_VISIB(relation, cst_relation, MIMIC_CORE_PUBLIC);
CST_VAL_USER_TYPE_DCLS_VISIB(item, cst_item, MIMIC_CORE_PUBLIC);
CST_VAL_USER_TYPE_DCLS_VISIB(utterance, cst_utterance, MIMIC_CORE_PUBLIC);
typedef struct cst_item_contents_struct {
    cst_features *features;
    cst_features *relations;
} cst_item_contents;

struct cst_item_struct {
    cst_item_contents *contents;   /* the shared part of an item */
    cst_relation *relation;
    cst_item *n;
    cst_item *p;
    cst_item *u;
    cst_item *d;
};

/* Constructor functions */
cst_item *new_item_relation(cst_relation *r, cst_item *i);
cst_item_contents *new_item_contents(cst_item *i);

/* Remove this item from this references */
MIMIC_CORE_PUBLIC void delete_item(cst_item *item);

void item_contents_set(cst_item *current, cst_item *i);
void item_unref_contents(cst_item *i);

MIMIC_CORE_PUBLIC cst_item *item_as(const cst_item *i, const char *rname);

MIMIC_CORE_PUBLIC cst_utterance *item_utt(const cst_item *i);

/* List accessor/manipulator function */
MIMIC_CORE_PUBLIC cst_item *item_next(const cst_item *i);
MIMIC_CORE_PUBLIC cst_item *item_prev(const cst_item *i);

MIMIC_CORE_PUBLIC cst_item *item_append(cst_item *i, cst_item *new_item);
MIMIC_CORE_PUBLIC cst_item *item_prepend(cst_item *i, cst_item *new_item);

/* Tree accessor/manipulator function */
MIMIC_CORE_PUBLIC cst_item *item_parent(const cst_item *i);
MIMIC_CORE_PUBLIC cst_item *item_nth_daughter(const cst_item *i, int n);
MIMIC_CORE_PUBLIC cst_item *item_daughter(const cst_item *i);
MIMIC_CORE_PUBLIC cst_item *item_last_daughter(const cst_item *i);

MIMIC_CORE_PUBLIC cst_item *item_add_daughter(cst_item *i, cst_item *new_item);
MIMIC_CORE_PUBLIC cst_item *item_append_sibling(cst_item *i, cst_item *new_item);
MIMIC_CORE_PUBLIC cst_item *item_prepend_sibling(cst_item *i, cst_item *new_item);

/* Feature accessor/manipulator functions */
MIMIC_CORE_PUBLIC int item_feat_present(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC int item_feat_remove(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC cst_features *item_feats(const cst_item *i);
MIMIC_CORE_PUBLIC const cst_val *item_feat(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC int item_feat_int(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC float item_feat_float(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC const char *item_feat_string(const cst_item *i, const char *name);
MIMIC_CORE_PUBLIC void item_set(const cst_item *i, const char *name, const cst_val *val);
MIMIC_CORE_PUBLIC void item_set_int(const cst_item *i, const char *name, int val);
MIMIC_CORE_PUBLIC void item_set_float(const cst_item *i, const char *name, float val);
MIMIC_CORE_PUBLIC void item_set_string(const cst_item *i, const char *name,
                     const char *val);

#define item_name(I) item_feat_string(I,"name")

     int item_equal(const cst_item *a, const cst_item *b);

MIMIC_CORE_PUBLIC const char *ffeature_string(const cst_item *item, const char *featpath);
MIMIC_CORE_PUBLIC int ffeature_int(const cst_item *item, const char *featpath);
MIMIC_CORE_PUBLIC float ffeature_float(const cst_item *item, const char *featpath);
MIMIC_CORE_PUBLIC const cst_val *ffeature(const cst_item *item, const char *featpath);
cst_item *path_to_item(const cst_item *item, const char *featpath);

/* Feature function, for features that are derived algorithmically from others. */
typedef const cst_val *(*cst_ffunction) (const cst_item *i);
CST_VAL_USER_FUNCPTR_DCLS(ffunc, cst_ffunction);
MIMIC_CORE_PUBLIC void ff_register(cst_features *ffeatures, const char *name,
                 cst_ffunction f);
MIMIC_CORE_PUBLIC void ff_unregister(cst_features *ffeatures, const char *name);

/* Generalized item hook function, like cst_uttfunc. */
typedef cst_val *(*cst_itemfunc) (cst_item *i);
CST_VAL_USER_FUNCPTR_DCLS_VISIB(itemfunc, cst_itemfunc, MIMIC_CORE_PUBLIC);
#endif

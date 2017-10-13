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
/*  Vals, typed objects                                                  */
/*                                                                       */
/*************************************************************************/
#ifndef CST_VAL_H
#define CST_VAL_H

#include "mimic_core_config.h"
#include "cst_endian.h"
#include "cst_lib_visibility.h"
#include "cst_file.h"
#include "cst_string.h"
#include "cst_error.h"
#include "cst_alloc.h"
#include "cst_val_defs.h"
#include <inttypes.h>

/* Only CONS can be an even number */
#define CST_VAL_TYPE_CONS    0
#define CST_VAL_TYPE_INT     1
#define CST_VAL_TYPE_FLOAT   3
#define CST_VAL_TYPE_STRING  5
#define CST_VAL_TYPE_FIRST_FREE 7
#define CST_VAL_TYPE_MAX     54

typedef struct cst_val_cons_struct {
    struct cst_val_struct *car;
    struct cst_val_struct *cdr;
} cst_val_cons;

/*
 The cst_val_atom structure has the same size than the cst_val_cons in all
 architectures.
 The least significant bit of the `car` element in the cst_val_cons structure
 should be in the same position than the least significant bit of the
 type integer in the cst_val_atom structure. That's why the
 "order here is important" comments exist.
 */
typedef struct cst_val_atom_struct {
#if defined(WORDS_BIGENDIAN)
 #if MIMIC_CPU_BITS == 64
    int32_t ref_count;            /* order is here important */
    int32_t type;
 #elif MIMIC_CPU_BITS == 32
    int16_t ref_count;
    int16_t type;                 /* order is here important */
 #else
   #error "Unknown CPU bit size"
 #endif
#elif defined(WORDS_LITTLEENDIAN)
 #if MIMIC_CPU_BITS == 64
    int32_t type;                   /* order is here important */
    int32_t ref_count;
 #elif MIMIC_CPU_BITS == 32
    int16_t type;                 /* order is here important */
    int16_t ref_count;
 #else
  #error "Unknown CPU bit size"
 #endif
#else
 #error "Unknown CPU endianness"
#endif
    union {
#if MIMIC_CPU_BITS == 64
        double fval;
        int64_t ival;
        void *vval;
#elif MIMIC_CPU_BITS == 32
        float fval;
        int32_t ival;
        void *vval;
#else
 #error "Unknown CPU bit size"
#endif
    } v;
} cst_val_atom;

typedef struct cst_val_struct {
    union {
        cst_val_cons cc;
        cst_val_atom a;
    } c;
} cst_val;

typedef struct cst_val_def_struct {
    const char *name;
    void (*delete_function) (void *);
} cst_val_def;

/* Constructor functions */
MIMIC_CORE_PUBLIC cst_val *int_val(int i);
MIMIC_CORE_PUBLIC cst_val *float_val(float f);
MIMIC_CORE_PUBLIC cst_val *string_val(const char *s);
cst_val *val_new_typed(int type, void *vv);
MIMIC_CORE_PUBLIC cst_val *cons_val(const cst_val *a, const cst_val *b);

/* Derefence and delete val if no other references */
MIMIC_CORE_PUBLIC void delete_val(cst_val *val);
MIMIC_CORE_PUBLIC void delete_val_list(cst_val *val);

/* Accessor functions */
MIMIC_CORE_PUBLIC int val_int(const cst_val *v);
MIMIC_CORE_PUBLIC float val_float(const cst_val *v);
MIMIC_CORE_PUBLIC const char *val_string(const cst_val *v);
MIMIC_CORE_PUBLIC void *val_void(const cst_val *v);
MIMIC_CORE_PUBLIC void *val_generic(const cst_val *v, int type, const char *stype);
MIMIC_CORE_PUBLIC const cst_val *val_car(const cst_val *v);
MIMIC_CORE_PUBLIC const cst_val *val_cdr(const cst_val *v);

MIMIC_CORE_PUBLIC const cst_val *set_cdr(cst_val *v1, const cst_val *v2);
MIMIC_CORE_PUBLIC const cst_val *set_car(cst_val *v1, const cst_val *v2);

int cst_val_consp(const cst_val *v);

/* Unsafe accessor function -- for the brave and foolish */
#define CST_VAL_STRING_LVAL(X) ((X)->c.a.v.vval)
#define CST_VAL_TYPE(X) ((X)->c.a.type)
#define CST_VAL_INT(X) ((X)->c.a.v.ival)
#define CST_VAL_FLOAT(X) ((X)->c.a.v.fval)
#define CST_VAL_STRING(X) ((const char *)(CST_VAL_STRING_LVAL(X)))
#define CST_VAL_VOID(X) ((X)->c.a.v.vval)
#define CST_VAL_CAR(X) ((X)->c.cc.car)
#define CST_VAL_CDR(X) ((X)->c.cc.cdr)

#define CST_VAL_REFCOUNT(X) ((X)->c.a.ref_count)

/* Some standard function */
int val_equal(const cst_val *a, const cst_val *b);
int val_less(const cst_val *a, const cst_val *b);
int val_greater(const cst_val *a, const cst_val *b);
int val_member(const cst_val *a, const cst_val *b);
int val_member_string(const char *a, const cst_val *b);
int val_stringp(const cst_val *a);
const cst_val *val_assoc_string(const char *v1, const cst_val *al);

MIMIC_CORE_PUBLIC void val_print(cst_file fd, const cst_val *v);
MIMIC_CORE_PUBLIC cst_val *val_readlist_string(const char *str);

MIMIC_CORE_PUBLIC cst_val *val_reverse(cst_val *v);
MIMIC_CORE_PUBLIC cst_val *val_append(cst_val *a, cst_val *b);
MIMIC_CORE_PUBLIC int val_length(const cst_val *l);
MIMIC_CORE_PUBLIC cst_val *cst_utf8_explode(const cst_string *utf8string);
cst_string *cst_implode(const cst_val *string_list);

MIMIC_CORE_PUBLIC cst_val *cst_utf8_ord(const cst_val *utf8_char);
cst_val *cst_utf8_chr(const cst_val *ord);

MIMIC_CORE_PUBLIC int cst_utf8_ord_string(const char *utf8_char);

/* make sure you know what you are doing before you call these */
int val_dec_refcount(const cst_val *b);
cst_val *val_inc_refcount(const cst_val *b);

#include "cst_val_const.h"
extern const cst_val_def cst_val_defs[];

/* Generic pointer vals */
typedef void cst_userdata;
CST_VAL_USER_TYPE_DCLS_VISIB(userdata, cst_userdata, MIMIC_CORE_PUBLIC);
#endif

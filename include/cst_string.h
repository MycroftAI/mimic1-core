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
/*    String manipulation functions                                      */
/*                                                                       */
/*************************************************************************/
#ifndef CST_STRING_H
#define CST_STRING_H

#include "cst_lib_visibility.h"
#include <string.h>
#include <stdint.h>

/* typedef unsigned char cst_string; */
typedef char cst_string;

double cst_atof(const char *str);

MIMIC_CORE_PUBLIC cst_string *cst_strdup(const cst_string *s);
MIMIC_CORE_PUBLIC cst_string *cst_strchr(const cst_string *s, int c);
MIMIC_CORE_PUBLIC cst_string *cst_strrchr(const cst_string *str, int c);
#define cst_strlen(s) (strlen((const char *)s))
#define cst_streq(A,B) (strcmp(A,B) == 0)
#define cst_streqn(A,B,N) (strncmp(A,B,N) == 0)
MIMIC_CORE_PUBLIC int cst_member_string(const char *str, const char *const *slist);
char *cst_substr(const char *str, int start, int length);

char *cst_string_before(const char *s, const char *c);
MIMIC_CORE_PUBLIC char *cst_strcat(const char *a, const char *b);
char *cst_strcat3(const char *a, const char *b, const char *c);

MIMIC_CORE_PUBLIC cst_string *cst_downcase(const cst_string *str);
cst_string *cst_upcase(const cst_string *str);

uint32_t utf8char_to_cp(const cst_string *const utf8char);
void cp_to_utf8char(const uint32_t cp, unsigned char *utf8char);

/* With this struct you can map unicode code points to int32_t in a reasonable way.
   With ASCII you can simply create "int32_t vector[256];" and that is it, with
   Unicode code points the full space would be 2^21, but we don't want to use all
   that memory.
*/
typedef struct struct_map_unicode_to_int {
    int32_t *v1;
    int32_t **v2;
    int32_t ***v3;
    int32_t ****v4;
    int32_t not_found; /* not found return value */
    int freeable;
} map_unicode_to_int;

map_unicode_to_int* cst_unicode_int_map_create();
void cst_unicode_int_map_delete(map_unicode_to_int *m);
int32_t cst_unicode_int_map(map_unicode_to_int *m, const unsigned char *utf8char, int set, int32_t value);


#endif

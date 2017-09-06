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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cst_alloc.h"
#include "cst_string.h"
#include "cst_file.h"
#include "cst_error.h"

cst_string *cst_strrchr(const cst_string *str, int c)
{
    return (cst_string *) strrchr((const char *) str, c);
}

double cst_atof(const char *str)
{
    return atof(str);
}

cst_string *cst_strdup(const cst_string *str)
{
    cst_string *nstr = NULL;

    if (str)
    {
        nstr = cst_alloc(cst_string, cst_strlen((const char *) str) + 1);
        memmove(nstr, str, cst_strlen((const char *) str) + 1);
    }
    return nstr;
}

cst_string *cst_strchr(const cst_string *s, int c)
{
    return (cst_string *) strchr((const char *) s, c);
}

char *cst_substr(const char *str, int start, int length)
{
    char *nstr = NULL;

    if (str)
    {
        nstr = cst_alloc(char, length + 1);
        strncpy(nstr, str + start, length);
        nstr[length] = '\0';
    }
    return nstr;
}

char *cst_string_before(const char *s, const char *c)
{
    char *p;
    char *q;

    p = strstr(s, c);
    if (p == NULL)
        return NULL;
    q = (char *) cst_strdup((cst_string *) s);
    q[cst_strlen(s) - cst_strlen(p)] = '\0';
    return q;
}

cst_string *cst_downcase(const cst_string *str)
{
    cst_string *dc;
    int i;

    dc = cst_strdup(str);
    for (i = 0; str[i] != '\0'; i++)
    {
        if (isupper((int) str[i]))
            dc[i] = tolower((int) str[i]);
    }
    return dc;
}

cst_string *cst_upcase(const cst_string *str)
{
    cst_string *uc;
    int i;

    uc = cst_strdup(str);
    for (i = 0; str[i] != '\0'; i++)
    {
        if (islower((int) str[i]))
            uc[i] = toupper((int) str[i]);
    }
    return uc;
}

int cst_member_string(const char *str, const char *const *slist)
{
    const char *const *p;

    for (p = slist; *p; ++p)
        if (cst_streq(*p, str))
            break;

    return *p != NULL;
}

char *cst_strcat(const char *a, const char *b)
{
    char *r;

    r = cst_alloc(char, cst_strlen(a) + cst_strlen(b) + 1);
    cst_sprintf(r, "%s%s", a, b);
    return r;
}

char *cst_strcat3(const char *a, const char *b, const char *c)
{
    char *r;

    r = cst_alloc(char, cst_strlen(a) + cst_strlen(b) + cst_strlen(c) + 1);
    cst_sprintf(r, "%s%s%s", a, b, c);
    return r;
}

void cp_to_utf8char(const uint32_t cp, unsigned char *utf8char)
{
  if (cp < 0x80)
  {
    utf8char[0] = (char) cp & 0x7F;
    utf8char[1] = 0;
    return;
  }
  else if (cp < 0x800)
  {
    utf8char[0] = (cp >> 6) | 0xC0;
    utf8char[1] = (cp & 0x3F) | 0x80;
    utf8char[2] = 0;
  }
  else if (cp < 0x10000)
  {
    utf8char[0] = (cp >> 12) | 0xE0;
    utf8char[1] = ((cp >> 6) & 0x3F) | 0x80;
    utf8char[2] = (cp & 0x3F) | 0x80;
    utf8char[3] = 0;
  }
  else if (cp < 0x200000)
  {
    utf8char[0] = (cp >> 18) | 0xF0;
    utf8char[1] = ((cp >> 12) & 0x3F) | 0x80;
    utf8char[2] = ((cp >> 6) & 0x3F) | 0x80;
    utf8char[3] = (cp & 0x3F) | 0x80;
    utf8char[4] = 0;
  }
  else
  {
    cst_errmsg("Wrong code point %d\n", cp);
    utf8char[0] = 0xEF; /* replacement character */
    utf8char[1] = 0xBF;
    utf8char[2] = 0xBD;
    utf8char[3] = 0; 
  }
}

uint32_t utf8char_to_cp(const cst_string *const utf8char)
{
    unsigned char c1, c2, c3, c4;
    unsigned char *c = (unsigned char *) utf8char;
    uint32_t cp;
    switch (cst_strlen(c))
    {
    case 1:
        /* 1st byte must be 0xxxxxxx so we mask with b01111111 = 0x7f */
        cp = c[0] & 0x7f;
        return cp;
    case 2:
        /* 1st byte must be 110xxxxx so we mask with b00011111 = 0x1f */
        c1 = c[0] & 0x1f;
        /* 2nd byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c2 = c[1] & 0x3f;
        /* We shift the bits of c1 6 positions to the left, and fill
     * those 6 positions with c2 */
        cp = (c1 << 6) | c2;
        return cp;
    case 3:
        /* 1st byte must be 1110xxxx so we mask with b00011111 = 0x0f */
        c1 = c[0] & 0x0f;
        /* 2nd byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c2 = c[1] & 0x3f;
        /* 3rd byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c3 = c[2] & 0x3f;
        /* We shift the bits of c1 12 positions to the left, and fill
     * the 12 positions with 6 bits from c2 and 6 bits from c3 */
        cp = (c1 << 12) | (c2 << 6) | c3;
        return cp;
    case 4:
        /* 1st byte must be 11110xxx so we mask with b00011111 = 0x07 */
        c1 = c[0] & 0x07;
        /* 2nd byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c2 = c[1] & 0x3f;
        /* 3rd byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c3 = c[2] & 0x3f;
        /* 4th byte must be 10xxxxxx so we mask with b00111111 = 0x3f */
        c4 = c[3] & 0x3f;
        cp = (c1 << 18) | (c2 << 12) | (c3 << 6) | c4;
        return cp;
    default:
        return 0xfffd; /* replacement character */
    }
}



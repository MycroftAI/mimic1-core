/* (C) Sergio Oller 2017
 * All rights reserved
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above
 *   copyright notice, this list of conditions and the following disclaimer
 *   in the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of the  nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "cst_val.h"
#include "cst_lexicon.h"
#include "cst_lts.h"
#include "cutest.h"


#define LTS_CONTEXT_SIZE 4
#define CST_LTS_P_NAME (LTS_CONTEXT_SIZE - 1)
#define CST_LTS_N_NAME LTS_CONTEXT_SIZE
#define CST_LTS_PHONE_a 0
#define CST_LTS_PHONE_b 1
#define CST_LTS_CODE_POINT_a 0x61
#define CST_LTS_CODE_POINT_b 0x62
#define CST_LTS_CODE_POINT_euro 0x20AC



void test_lts(void)
{
    cst_val *pred;
    cst_lts_rule r_model[] = {
        {((CST_LTS_EOR & 0xFF) << 24) | (CST_LTS_PHONE_a & 0x1FFFFF), -1, -1},  /* a */
        {((CST_LTS_EOR & 0xFF) << 24) | (CST_LTS_PHONE_b & 0x1FFFFF), -1, -1},  /* b */
        {((CST_LTS_P_NAME & 0xFF) << 24) | (CST_LTS_CODE_POINT_b & 0x1FFFFF), 1, 0},    /* if prev is b, go to 1, otherwise go to 0 */
        {((CST_LTS_N_NAME & 0xFF) << 24) |
         (CST_LTS_CODE_POINT_euro & 0x1FFFFF), 2, 0}
    };
    const char *const phone_table[] = {
        "a",
        "b"
    };

    int32_t v1[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 16 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 32 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 48 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 64 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    int32_t v3_2_2[128] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        /* 64 */
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };
    int32_t *v3_2[64] = {
        NULL, NULL, v3_2_2, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };
    int32_t **v3[16] = {
        NULL, NULL, v3_2, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL
    };
    map_unicode_to_int letter_index = {
        v1, NULL, v3, NULL, 0xFFFD, 0
    };
    cst_lts_rules r = {
        "test_rules",
        r_model,
        phone_table,
        LTS_CONTEXT_SIZE,
        0,
        &letter_index
    };

    printf("Predicting 'a'\n");
    pred = lts_apply("a", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 1);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "a"));

    printf("Predicting 'b'\n");
    pred = lts_apply("b", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 1);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "b"));

    printf("Predicting 'ab'\n");
    pred = lts_apply("ab", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 2);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "a"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(pred))), "b"));

    printf("Predicting 'aab'\n");
    pred = lts_apply("aab", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 3);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "a"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(pred))), "a"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(val_cdr(pred)))), "b"));

    printf("Predicting 'abb'\n");
    pred = lts_apply("abb", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 3);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "a"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(pred))), "b"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(val_cdr(pred)))), "b"));

    printf("Predicting 'aba'\n");
    pred = lts_apply("aba", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 3);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "a"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(pred))), "b"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(val_cdr(pred)))), "b"));

    printf("Predicting 'b€€'\n");
    pred = lts_apply("b€€", "", &r);
    //val_print(stdout, pred);
    TEST_CHECK(val_length(pred) == 3);
    TEST_CHECK(cst_streq(val_string(val_car(pred)), "b"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(pred))), "b"));
    TEST_CHECK(cst_streq(val_string(val_car(val_cdr(val_cdr(pred)))), "a"));
}

TEST_LIST =
{
    {
    "letter to sound rules", test_lts},
    {
    0}
};

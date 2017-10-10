#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "cst_wave.h"
#include "mimic.h"

#include "cutest.h"


#ifndef A_WAV1
  #define A_WAV1 "hello_world.wav"
#endif

#ifndef A_WAV2
  #define A_WAV2 "hi_again.wav"
#endif

void test_copy(void)
{
   cst_wave *w1 = new_wave();
   cst_wave *w2;
   mimic_core_init();
   TEST_CHECK(cst_wave_load_riff(w1, A_WAV1) == 0);
   w2 = copy_wave(w1);
   TEST_CHECK(w1 != w2);
   TEST_CHECK(w1->num_samples == w2->num_samples);
   TEST_CHECK(w1->sample_rate == w2->sample_rate);
   TEST_CHECK(w1->num_channels == w2->num_channels);
   TEST_CHECK(w1->samples != w2->samples);
   TEST_CHECK(w1->samples[0] == w2->samples[0]);
   TEST_CHECK(w1->samples[10] == w2->samples[10]);
   TEST_CHECK(w1->samples[20] == w2->samples[20]);
   delete_wave(w1);
   delete_wave(w2);
   mimic_core_exit();
}

void test_concat(void)
{
   int original_len;
   cst_wave *w1 = new_wave();
   cst_wave *w2 = new_wave();
   mimic_core_init();
   TEST_CHECK(cst_wave_load_riff(w1, A_WAV1) == 0);
   TEST_CHECK(cst_wave_load_riff(w2, A_WAV2) == 0);
   original_len = w1->num_samples;
   concat_wave(w1, w2);
   TEST_CHECK(w1->num_samples == w2->num_samples + original_len);
   delete_wave(w1);
   delete_wave(w2);

   w1 = new_wave();
   w2 = new_wave();
   TEST_CHECK(cst_wave_load_riff(w1, A_WAV1) == 0);
   TEST_CHECK(cst_wave_load_riff(w2, A_WAV2) == 0);
   
   w2->sample_rate *= 2; // create sample rate mismatch
   fprintf(stderr, "\nExpect error message below:\n");
   fflush(stderr);
   concat_wave(w1, w2);
   fprintf(stderr, "Expect error message above:\n");
   fflush(stderr);
   TEST_CHECK(w1->num_samples != w2->num_samples + original_len);
   delete_wave(w1);
   delete_wave(w2);

   w1 = new_wave();
   w2 = new_wave();

   TEST_CHECK(cst_wave_load_riff(w1, A_WAV1) == 0);
   TEST_CHECK(cst_wave_load_riff(w2, A_WAV2) == 0);
   
   w2->num_channels *= 2; // create channel number mismatch
   fprintf(stderr, "Expect error message below:\n");
   fflush(stderr);
   concat_wave(w1, w2);
   fprintf(stderr, "Expect error message above:\n");
   fflush(stderr);
   TEST_CHECK(w1->num_samples != w2->num_samples + original_len);
   delete_wave(w1);
   delete_wave(w2);
}

void test_create(void)
{
   cst_wave *w = NULL;
   w = new_wave();
   TEST_CHECK(w != NULL);
   TEST_CHECK(w->num_samples == 0);
   TEST_CHECK(w->type == NULL);
   delete_wave(w);
}

void test_delete(void)
{
   delete_wave(NULL);
}

void test_resize(void)
{
   cst_wave *w = new_wave();
   cst_wave_resize(w, 200, 2);
   TEST_CHECK(w->num_samples == 200);
   TEST_CHECK(w->num_channels = 2);
   TEST_CHECK(cst_wave_resize(NULL, 200, 2) < 0);
   delete_wave(w);
}

void test_rescale(void)
{
   int i;
   cst_wave *w = new_wave();
   cst_wave_resize(w, 10, 2);
   for (i = 0; i < 10; i++)
      w->samples[i] = 10 + i;
   cst_wave_rescale(w, 65536 * 2); //scale x2
   for (i = 0; i < 10; i++)
      TEST_CHECK(w->samples[i] == (10 + i) * 2);
   delete_wave(w);
}

TEST_LIST =
{
    {"create wave", test_create},
    {"delete wave", test_delete},
    {"resize wave", test_resize},
    {"rescale wave", test_rescale},
    {"copy wave", test_copy},
    {"concatenate waves", test_concat},
    {0}
};

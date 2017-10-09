/*************************************************************************/
/*                                                                       */
/*                  Language Technologies Institute                      */
/*                     Carnegie Mellon University                        */
/*                        Copyright (c) 2004                             */
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
/*             Author:  Sergio Oller                                     */
/*               Date:  February 2016                                    */
/*************************************************************************/
/*                                                                       */
/*  Audio support based on the PortAudio library                         */
/*                                                                       */
/*************************************************************************/

#include "cst_audio.h"

#ifdef CST_AUDIO_PORTAUDIO

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <portaudio.h>




typedef struct cst_audio_portaudiodata_struct {
    PaStreamParameters *outputParameters;
    double sample_rate;
    long int bytes_per_frame;
    PaStream *stream;
} cst_audio_portaudiodata;


static int audio_error_portaudio(int err)
{
    if (err == paNoError)
    {
        return err;
    }
    cst_errmsg("An error occured while using the portaudio stream\n");
    cst_errmsg("Error number: %d\n", err);
    cst_errmsg("Error message: %s\n", Pa_GetErrorText(err));
    return err;
}


cst_audiodev *audio_open_portaudio(int sps, int channels, cst_audiofmt fmt)
{
    cst_audiodev *ad = cst_alloc(cst_audiodev, 1);
    ad->sps = sps;
    ad->real_sps = sps;
    ad->channels = channels;
    ad->real_channels = channels;
    ad->fmt = fmt;
    ad->real_fmt = fmt;
    ad->byteswap = 0;           /* not used */
    ad->rateconv = NULL;        /* not used */
    cst_audio_portaudiodata *hdl = cst_alloc(cst_audio_portaudiodata, 1);
    ad->platform_data = (void *) hdl;
    hdl->outputParameters = cst_alloc(PaStreamParameters, 1);

    hdl->outputParameters->device = Pa_GetDefaultOutputDevice();        /* default output device */
    if (hdl->outputParameters->device == paNoDevice)
    {
        cst_errmsg("Error: No default output device.\n");
        audio_error_portaudio(paInvalidDevice);
        return NULL;
    }
    hdl->outputParameters->channelCount = channels;     /* stereo output */
    switch (fmt)
    {
    case CST_AUDIO_LINEAR16:
        hdl->outputParameters->sampleFormat = paInt16;
        hdl->bytes_per_frame = channels * 2;
        break;
    case CST_AUDIO_LINEAR8:
        hdl->outputParameters->sampleFormat = paInt8;
        hdl->bytes_per_frame = channels * 1;
        break;
    case CST_AUDIO_MULAW:
        cst_errmsg("Error: MULAW not supported in portaudio.\n");
    default:
        audio_error_portaudio(paSampleFormatNotSupported);
        free(ad);
        free(hdl->outputParameters);
        free(hdl);
        free(ad);
        return NULL;
        break;
    }
    hdl->outputParameters->suggestedLatency =
        Pa_GetDeviceInfo(hdl->outputParameters->device)->
        defaultLowOutputLatency;
    hdl->outputParameters->hostApiSpecificStreamInfo = NULL;
    hdl->sample_rate = (double) sps;

    int err;

    /* Stream */
    err = Pa_OpenStream(&(hdl->stream), NULL,   /* no input */
                        hdl->outputParameters, hdl->sample_rate, 64, paClipOff,   /* we won't output out of range samples? */
                        NULL, NULL);
    if (audio_error_portaudio(err) < 0)
    {
        audio_error_portaudio(err);
        return NULL;
    }
    err = Pa_StartStream(hdl->stream);
    if (audio_error_portaudio(err) < 0)
    {
        audio_error_portaudio(err);
        return NULL;
    }

    return ad;
}

int audio_drain_portaudio(cst_audiodev *ad)
{
    if (ad != NULL)
    {
        cst_audio_portaudiodata *hdl =
            (cst_audio_portaudiodata *) ad->platform_data;
        if (hdl != NULL)
        {
            int err = Pa_AbortStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;
            err = Pa_StartStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;
        }
    }
    return 0;
}

int audio_flush_portaudio(cst_audiodev *ad)
{
    int err;
    if (ad != NULL)
    {
        cst_audio_portaudiodata *hdl =
            (cst_audio_portaudiodata *) ad->platform_data;
        if (hdl != NULL)
        {
            err = Pa_StopStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;
            err = Pa_StartStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;
        }
    }
    return 0;
}

int audio_close_portaudio(cst_audiodev *ad)
{
    int err;
    if (ad != NULL)
    {
        cst_audio_portaudiodata *hdl =
            (cst_audio_portaudiodata *) ad->platform_data;
        if (hdl != NULL)
        {
            err = Pa_StopStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;
            err = Pa_CloseStream(hdl->stream);
            if (audio_error_portaudio(err) < 0)
                return err;

            if (hdl->outputParameters != NULL)
            {
                free(hdl->outputParameters);
            }
            free(ad->platform_data);
        }
        free(ad);
    }
    return 0;
}

int audio_init_portaudio()
{
    cst_errmsg("If audio works ignore messages below\n");
    int err = Pa_Initialize();
    cst_errmsg("If audio works ignore messages above\n");
    audio_error_portaudio(err);
    return err;
}

int audio_exit_portaudio()
{
    Pa_Terminate();
    return 0;
}

int audio_write_portaudio(cst_audiodev *ad, void *buff, int num_bytes)
{
    int err;
    cst_audio_portaudiodata *hdl =
        (cst_audio_portaudiodata *) ad->platform_data;
    long int num_frames = num_bytes / hdl->bytes_per_frame;

    err = Pa_WriteStream(hdl->stream, buff, num_frames);
    if (audio_error_portaudio(err) < 0)
        return err;
    return num_bytes;
}

#endif //CST_AUDIO_PORTAUDIO

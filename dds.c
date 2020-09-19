
#include <alsa/asoundlib.h>
//#include "dds.h"

#define PCM_DEVICE "default"
#define DMATRMBUF_SIZE 2046
#include <initguid.h>
#include "idds.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

//


int main(int argc, char *argv[])
{

	int i;
	int err;
	short buf[DMATRMBUF_SIZE];
	snd_pcm_t *playback_handle;
	snd_pcm_hw_params_t *hw_params;
	float freq = 8000;
	void *handle;

	char *error;

	handle = dlopen ("/home/kevin/eclipse-workspace/dds/sinelib/libsine.so", RTLD_LAZY);
	if (!handle) {
		fputs (dlerror(), stderr);
		exit(1);
	}
	HANDLE(*DllGetClassObject)(REFCLSID objGuid,
			REFIID factoryGuid, void **factoryHandle);

	DllGetClassObject = dlsym(handle, "DllGetClassObject");

	IWaveForm *wave;
	IClassFactory* cf;
	DllGetClassObject(&CLSID_SineWave,&IID_IClassFactory,&cf);

	cf->lpVtbl->CreateInstance(cf,0,&IID_IWaveForm,&wave);
	if (argc > 2)
	{
		freq = atof(argv[2]);
		wave->frequency_L=freq;
		wave->frequency_R=freq;
	}
	wave->lpVtbl->Generate(wave);


	if ((err = snd_pcm_open(&playback_handle, argv[1], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		fprintf(stderr, "cannot open audio device %s (%s)\n",
				argv[1],
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
	{
		fprintf(stderr, "cannot allocate hardware parameter structure (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_any(playback_handle, hw_params)) < 0)
	{
		fprintf(stderr, "cannot initialize hardware parameter structure (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
	{
		fprintf(stderr, "cannot set access type (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0)
	{
		fprintf(stderr, "cannot set sample format (%s)\n",
				snd_strerror(err));
		exit(1);
	}
	int dir = 0;
	unsigned int sample_rate = 44100;

	if ((err = snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &sample_rate, &dir)) < 0)
	{
		fprintf(stderr, "cannot set sample rate (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2)) < 0)
	{
		fprintf(stderr, "cannot set channel count (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	if ((err = snd_pcm_hw_params(playback_handle, hw_params)) < 0)
	{
		fprintf(stderr, "cannot set parameters (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	snd_pcm_hw_params_free(hw_params);

	if ((err = snd_pcm_prepare(playback_handle)) < 0)
	{
		fprintf(stderr, "cannot prepare audio interface for use (%s)\n",
				snd_strerror(err));
		exit(1);
	}

	for (i = 0; i < 10000; ++i)
	{
		wave->lpVtbl->Calculate(wave,(dmabuf_t *)buf, DMATRMBUF_SIZE / 2);

		if ((err = snd_pcm_writei(playback_handle, buf, DMATRMBUF_SIZE / 2)) != DMATRMBUF_SIZE / 2)
		{
			if (err == -EPIPE)
			{
				printf("XRUN.\n");
				snd_pcm_prepare(playback_handle);
			}
			else
			{
				fprintf(stderr, "write to audio interface failed (%s)\n",
						snd_strerror(err));
				exit(1);
			}
		}
	}

	snd_pcm_close(playback_handle);
	exit(0);


}

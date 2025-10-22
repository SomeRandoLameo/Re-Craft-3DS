//most code here is pulled from the 3ds opus decoding example or whatever its called. Look there for any extra comments that might help you
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

#include <opus/opusfile.h>
#include <3ds.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "Sound.h"
//#include "../gui/DebugUI.h"

static const int SAMPLE_RATE = 48000;
static const int SAMPLES_PER_BUF = SAMPLE_RATE * 120 / 1000;
static const int CHANNELS_PER_SAMPLE = 2;
static const int THREAD_AFFINITY = -1;
static const int THREAD_STACK_SZ = 32 * 1024;
static const size_t WAVEBUF_SIZE = SAMPLES_PER_BUF * CHANNELS_PER_SAMPLE * sizeof(int16_t);
ndspWaveBuf s_waveBufs0[3];
ndspWaveBuf s_waveBufs1[3];
int16_t* s_audioBuffer0 = NULL;
int16_t* s_audioBuffer1 = NULL;
LightEvent s_event, s_event1;
volatile bool s_quit0 = false, s_quit1 = false;

const char* opusStrError(int error)
{
	switch (error) {
	case OP_FALSE:
		return "OP_FALSE: A request did not succeed.";
	case OP_HOLE:
		return "OP_HOLE: There was a hole in the page sequence numbers.";
	case OP_EREAD:
		return "OP_EREAD: An underlying read, seek or tell operation "
			"failed.";
	case OP_EFAULT:
		return "OP_EFAULT: A NULL pointer was passed where none was "
			"expected, or an internal library error was encountered.";
	case OP_EIMPL:
		return "OP_EIMPL: The stream used a feature which is not "
			"implemented.";
	case OP_EINVAL:
		return "OP_EINVAL: One or more parameters to a function were "
			"invalid.";
	case OP_ENOTFORMAT:
		return "OP_ENOTFORMAT: This is not a valid Ogg Opus stream.";
	case OP_EBADHEADER:
		return "OP_EBADHEADER: A required header packet was not properly "
			"formatted.";
	case OP_EVERSION:
		return "OP_EVERSION: The ID header contained an unrecognised "
			"version number.";
	case OP_EBADPACKET:
		return "OP_EBADPACKET: An audio packet failed to decode properly.";
	case OP_EBADLINK:
		return "OP_EBADLINK: We failed to find data we had seen before or "
			"the stream was sufficiently corrupt that seeking is "
			"impossible.";
	case OP_ENOSEEK:
		return "OP_ENOSEEK: An operation that requires seeking was "
			"requested on an unseekable stream.";
	case OP_EBADTIMESTAMP:
		return "OP_EBADTIMESTAMP: The first or last granule position of a "
			"link failed basic validity checks.";
	default:
		return "Unknown error.";
	}
}

void DoQuit(int _channel)
{
	switch (_channel)
	{
	case 0: s_quit0 = true;
		LightEvent_Signal(&s_event);
		break;
	case 1:	s_quit1 = true;
		LightEvent_Signal(&s_event1);
		break;
	}
}

bool audioInit(int _channel) {
	ndspChnReset(_channel);
	ndspSetOutputMode(NDSP_OUTPUT_STEREO);
	ndspChnSetInterp(_channel, NDSP_INTERP_POLYPHASE);
	ndspChnSetRate(_channel, SAMPLE_RATE);
	ndspChnSetFormat(_channel, NDSP_FORMAT_STEREO_PCM16);
	const size_t bufferSize = WAVEBUF_SIZE * ARRAY_SIZE(s_waveBufs0);
	if (_channel == 0)
	{
		s_audioBuffer0 = (int16_t*)linearAlloc(bufferSize);
		if (!s_audioBuffer0) {
			printf("Failed to allocate audio buffer\n");
			return false;
		}
	}
	if (_channel == 1)
	{
		s_audioBuffer1 = (int16_t*)linearAlloc(bufferSize);
		if (!s_audioBuffer1) {
			printf("Failed to allocate audio buffer\n");
			return false;
		}
	}
	if (_channel == 0)
	{
		memset(&s_waveBufs0, 0, sizeof(s_waveBufs0));
		int16_t* buffer = _channel == 0 ? s_audioBuffer0 : s_audioBuffer1;
		for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs0); ++i) {
			s_waveBufs0[i].data_vaddr = buffer;
			s_waveBufs0[i].status = NDSP_WBUF_DONE;
			buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
		}
	}
	if (_channel == 1)
	{
		memset(&s_waveBufs1, 0, sizeof(s_waveBufs1));
		int16_t* buffer = _channel == 0 ? s_audioBuffer0 : s_audioBuffer1;
		for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs1); ++i) {
			s_waveBufs1[i].data_vaddr = buffer;
			s_waveBufs1[i].status = NDSP_WBUF_DONE;
			buffer += WAVEBUF_SIZE / sizeof(buffer[0]);
		}
	}
	return true;
}

void audioExit(int _channel) {
	//	ndspChnWaveBufClear(_channel);
	//	printf("ndspChnWaveBufClear done %i\n", _channel);
	ndspChnReset(_channel);
	//printf("ndspChnReset done %i\n", _channel);
	linearFree(_channel == 0 ? s_audioBuffer0 : s_audioBuffer1);
}

bool fillBuffer(int _channel, OggOpusFile* opusFile_, ndspWaveBuf* waveBuf_) {
	int totalSamples = 0;
	while (totalSamples < SAMPLES_PER_BUF) {
		int16_t* buffer = waveBuf_->data_pcm16 + (totalSamples *CHANNELS_PER_SAMPLE);
		const size_t bufferSize = (SAMPLES_PER_BUF - totalSamples) *CHANNELS_PER_SAMPLE;
		const int samples = op_read_stereo(opusFile_, buffer, bufferSize);
		if (samples <= 0) {
			if (samples == 0) break;

			printf("op_read_stereo: error %d (%s)", samples,
				opusStrError(samples));
			break;
		}

		totalSamples += samples;
	}
	if (totalSamples == 0) {
		return false;
	}
	waveBuf_->nsamples = totalSamples;
	ndspChnWaveBufAdd(_channel, waveBuf_);
	DSP_FlushDataCache(waveBuf_->data_pcm16,totalSamples * CHANNELS_PER_SAMPLE * sizeof(int16_t));
	return true;
}
void audioCallback(void* const nul_) {
	(void)nul_;
	if (s_quit0) {
		return;
	}

	LightEvent_Signal(&s_event);
}
void audioThread0(void* const opusFile_) {
	OggOpusFile* const opusFile = (OggOpusFile*)opusFile_;

	while (!s_quit0) {
		for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs0); ++i) {
			if (s_waveBufs0[i].status != NDSP_WBUF_DONE) {
				continue;
			}
			if (!fillBuffer(0, opusFile, &s_waveBufs0[i])) {
				return;
			}
		}
		LightEvent_Wait(&s_event);
	}
}
void audioThread1(void* const opusFile_) {
	OggOpusFile* const opusFile = (OggOpusFile*)opusFile_;

	while (!s_quit1) {
		for (size_t i = 0; i < ARRAY_SIZE(s_waveBufs1); ++i) {
			if (s_waveBufs1[i].status != NDSP_WBUF_DONE) {
				continue;
			}
			if (!fillBuffer(1, opusFile, &s_waveBufs1[i])) {
				s_quit1 = true;
				return;
			}
		}
		LightEvent_Wait(&s_event1);
	}
}

void playopus(Sound* sound) {
	if (sound->background == false && sound->threaid != NULL)
	{
		DoQuit(1);
		threadJoin(sound->threaid, 50000);
		threadFree(sound->threaid);
		//DebugUI_Log("Free thread %p\n", sound->threaid);
		sound->threaid = NULL;
		if (sound->opusFile != NULL)
		{
			op_free(sound->opusFile);
		}
		audioExit(1);
		sound->threaid = NULL;
	}
	s_quit1 = false;
	if (sound->background == true)
	{
		LightEvent_Init(&s_event, RESET_ONESHOT);
	}
	else
	{
		LightEvent_Init(&s_event1, RESET_ONESHOT);
	}
	int error = 0;
	OggOpusFile* opusFile = op_open_file(sound->path, &error);
	if (error)
	{
		//DebugUI_Log("An Error occured opening file %s \n", sound->path);
		return;
	}
	if (!audioInit(sound->background == true ? 0 : 1))
	{
		printf("Failed to initialise audio 0\n");
		return;
	}
	if (sound->background == true)
	{
		ndspSetCallback(audioCallback, NULL);
	}
	int32_t priority = 0x30;
	svcGetThreadPriority(&priority, CUR_THREAD_HANDLE);
	priority -= 1;
	priority = priority < 0x18 ? 0x18 : priority;
	priority = priority > 0x3F ? 0x3F : priority;
	Thread threadId = threadCreate(sound->background == true ? audioThread0 : audioThread1, opusFile, THREAD_STACK_SZ, priority, THREAD_AFFINITY, false);
	sound->threaid = threadId;
	sound->opusFile = opusFile;
	return;
}
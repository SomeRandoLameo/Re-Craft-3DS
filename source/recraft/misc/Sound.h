#pragma once
#include <opus/opusfile.h>

typedef struct{
	char path[128];
	bool background;
	Thread threaid;
	OggOpusFile *opusFile;
}Sound;

void playopus(Sound*);

//why werent they defined?
void DoQuit(int _channel);
void audioExit(int _channel);
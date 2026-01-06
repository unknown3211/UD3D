#pragma once

#include <AL/al.h>
#include <sndfile.h>

class SoundBuffer
{
public:
	SoundBuffer(const char* fileName);
	~SoundBuffer();

	void Play();
	void Pause();
	void Stop();
	void Resume();
	void UpdateBufferStream();
	ALint getSource();
	bool isPlaying();
	void SetGain(const float& val);
private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	SoundBuffer() = delete;
};
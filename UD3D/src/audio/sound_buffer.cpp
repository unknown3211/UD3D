#include "audio/sound_buffer.h"
#include <cstddef>
#include <AL\alext.h>
#include <malloc.h>

SoundBuffer::SoundBuffer(const char* filename)
{
	alGenSources(1, &p_Source);
	alGenBuffers(NUM_BUFFERS, p_Buffers);

	std::size_t frame_size;

	p_SndFile = sf_open(filename, SFM_READ, &p_Sfinfo);
	if (!p_SndFile)
	{
		throw("could not open provided music file -- check path");
	}

	if (p_Sfinfo.channels == 1)
		p_Format = AL_FORMAT_MONO16;
	else if (p_Sfinfo.channels == 2)
		p_Format = AL_FORMAT_STEREO16;
	else if (p_Sfinfo.channels == 3)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (p_Sfinfo.channels == 4)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!p_Format)
	{
		sf_close(p_SndFile);
		p_SndFile = NULL;
		throw("Unsupported channel count from file");
	}

	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
	p_Membuf = static_cast<short*>(malloc(frame_size));
}

SoundBuffer::~SoundBuffer()
{
	alDeleteSources(1, &p_Source);
	if (p_SndFile)
		sf_close(p_SndFile);
	p_SndFile = nullptr;
	free(p_Membuf);
	alDeleteBuffers(NUM_BUFFERS, p_Buffers);
}

void SoundBuffer::Play()
{
	ALsizei i;
	alGetError();
	alSourceRewind(p_Source);
	alSourcei(p_Source, AL_BUFFER, 0);

	for (i = 0; i < NUM_BUFFERS; i++)
	{
		sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error buffering for playback");
	}

	alSourceQueueBuffers(p_Source, i, p_Buffers);
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error starting playback");
	}

}

void SoundBuffer::Pause()
{
	alSourcePause(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundBuffer::Stop()
{
	alSourceStop(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundBuffer::Resume()
{
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundBuffer::UpdateBufferStream()
{
	ALint processed, state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}

	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(p_Source, 1, &bufid);
		processed--;

		slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
				p_Sfinfo.samplerate);
			alSourceQueueBuffers(p_Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error buffering music data");
		}
	}

	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;
		alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error with al");
		}

		if (queued == 0)
			return;

		alSourcePlay(p_Source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error with al");
		}
	}

}

ALint SoundBuffer::getSource()
{
	return p_Source;
}

bool SoundBuffer::isPlaying()
{
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
	return (state == AL_PLAYING);
}

void SoundBuffer::SetGain(const float& val)
{
	float newval = val;
	if (newval < 0)
		newval = 0;
	alSourcef(p_Source, AL_GAIN, val);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}
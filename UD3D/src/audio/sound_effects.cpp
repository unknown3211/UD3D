#include "audio/sound_effects.h"
#include <iostream>

SoundEffects::SoundEffects()
{
	p_Buffer = 0;
	alGenSources(1, &p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

SoundEffects::~SoundEffects()
{
	alDeleteSources(1, &p_Source);
}

void SoundEffects::Play(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error with al");
		}
	}
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundEffects::Stop()
{
	alSourceStop(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundEffects::Pause()
{
	alSourcePause(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundEffects::Resume()
{
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundEffects::SetBufferToPlay(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error with al");
		}
	}
}

void SoundEffects::SetLooping(const bool& loop)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)loop);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundEffects::SetPosition(const float& x, const float& y, const float& z)
{
	alSource3f(p_Source, AL_POSITION, x, y, z);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

bool SoundEffects::isPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}
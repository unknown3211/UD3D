#include "audio/sound.h"
#include <algorithm>

Sound::Sound() : sd(nullptr), att(AL_NONE)
{
	sd = LISTENER->Get();
	if (!sd)
	{
		//Log(error, "Failed To Load Sound !");
	}
}

int Sound::Load(const char* fileName)
{
	if (!sd)
		return -1;

	if (!sfx)
	{
		sfx = std::make_unique<SoundEffects>();
	}

	int soundID = SE_LOAD(fileName);
	if (soundID < 0)
		return -1;

	att = AL_INVERSE_DISTANCE_CLAMPED;
	sd->SetAttunation(att);

	ls.push_back(soundID);
	return soundID;
}

void Sound::Play(int soundID)
{
	if (sfx && IsSoundLoaded(soundID))
	{
		sfx->Play(soundID);
	}
	else
	{
		//Log(error, "No Sound Loaded !");
	}
}

void Sound::Remove(int soundID)
{
	ls.erase(std::remove(ls.begin(), ls.end(), soundID), ls.end());
}

void Sound::RemoveAll()
{
	ls.clear();
	sfx.reset();
}

bool Sound::IsSoundLoaded(int soundID) const
{
	return std::find(ls.begin(), ls.end(), soundID) != ls.end();
}
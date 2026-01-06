#pragma once

#include "audio/sound_device.h"
#include "audio/sound_buffer.h"
#include "audio/sound_effects_lib.h"
#include "audio/sound_effects.h"
#include <memory>
#include <vector>

class Sound
{
public:
	Sound();
	int Load(const char* fileName);
	void Play(int soundID);
	void Remove(int soundID);
	void RemoveAll();
	ALint GetSoundAttunation() const { return att; }
	bool IsSoundLoaded(int soundID) const;
private:
	SoundDevice* sd;
	std::unique_ptr<SoundEffects> sfx;
	std::vector<int> ls;
	ALint att;
};
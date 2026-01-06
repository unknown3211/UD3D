#include "audio/sound_device.h"
#include <AL/alc.h>
#include <AL/al.h>
#include <stdio.h>
#include <vector>

static SoundDevice* s_instance = nullptr;

SoundDevice* SoundDevice::Get()
{
	Init();
	return s_instance;
}

void SoundDevice::Init()
{
	if (s_instance == nullptr)
		s_instance = new SoundDevice();
}

void SoundDevice::GetLocation(float& x, float& y, float& z)
{
	alGetListener3f(AL_POSITION, &x, &y, &z);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundDevice::GetOrientation(float& ori)
{
	alGetListenerfv(AL_ORIENTATION, &ori);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

float SoundDevice::GetGain()
{
	float curr_gain;
	alGetListenerf(AL_GAIN, &curr_gain);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
	return curr_gain;
}

void SoundDevice::SetGain(const float& val)
{
	float newVol = val;
	if (newVol < 0.f)
		newVol = 0.f;
	else if (newVol > 5.f)
		newVol = 5.f;

	alListenerf(AL_GAIN, newVol);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundDevice::SetAttunation(int key)
{
	if (key < 0xD001 || key > 0xD006)
		throw("bad attunation key");

	alDistanceModel(key);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundDevice::SetLocation(const float& x, const float& y, const float& z)
{
	alListener3f(AL_POSITION, x, y, z);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

void SoundDevice::SetOrientation(const float& atx, const float& aty, const float& atz, const float& upx, const float& upy, const float& upz)
{
	std::vector<float> ori;
	ori.push_back(atx);
	ori.push_back(aty);
	ori.push_back(atz);
	ori.push_back(upx);
	ori.push_back(upy);
	ori.push_back(upz);
	alListenerfv(AL_ORIENTATION, ori.data());
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}

SoundDevice::SoundDevice()
{
	p_ALCDevice = alcOpenDevice(nullptr);
	if (!p_ALCDevice)
		throw("failed to get sound device");

	p_ALCContext = alcCreateContext(p_ALCDevice, nullptr);
	if (!p_ALCContext)
		throw("Failed to set sound context");

	if (!alcMakeContextCurrent(p_ALCContext))
		throw("failed to make context current");

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(p_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
		name = alcGetString(p_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	if (!name || alcGetError(p_ALCDevice) != AL_NO_ERROR)
		name = alcGetString(p_ALCDevice, ALC_DEVICE_SPECIFIER);
	printf("Opened \"%s\"\n", name);
}

SoundDevice::~SoundDevice()
{
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(p_ALCContext);
	alcCloseDevice(p_ALCDevice);
}
#include "SoundSystem.h"
#include "ResourceManager.h"
#include <atlconv.h>

FMOD::System*	SoundSystem::system = {};
FMOD::Channel* SoundSystem::channel[SoundChnalList] = {};
float			SoundSystem::volume[SoundChnalList] = {};

SoundSystem::~SoundSystem()
{
	system->release();
	system->close();
}

void SoundSystem::Initialize()
{
	FMOD::System_Create(&system);
	system->init((int)SoundChnalList, FMOD_INIT_NORMAL, 0);
}

void SoundSystem::PlayMusic(std::wstring _key, SoundChannel _channel)
{
	channel[_channel]->stop();
	system->playSound(ResourceManager::LoadResource<SoundResource>(_key).get()->GetResource(), NULL, 0, &channel[_channel]);
	channel[_channel]->setVolume(volume[_channel]);
}

void SoundSystem::StopMusic(SoundChannel _channel)
{
	channel[_channel]->stop();
}

void SoundSystem::SetVolume(float _volume)
{
	for (unsigned int i = 0; i < SoundChnalList; ++i)
	{
		volume[i] = _volume;
		channel[i]->setVolume(volume[i]);
	}
}

void SoundSystem::SetVolume(float _volume, SoundChannel _channel)
{
	volume[_channel] = _volume;
	channel[_channel]->setVolume(volume[_channel]);
}

bool SoundSystem::IsPlay(SoundChannel _channel)
{
	return false;
}

void SoundSystem::CreateSound(std::wstring _key, bool loopcheck, FMOD::Sound*& _sound)
{
	std::string tempPath;
	USES_CONVERSION;
	tempPath = std::string(W2A(_key.c_str()));
	if (loopcheck)
		system->createSound(tempPath.c_str(), FMOD_LOOP_NORMAL, 0, &_sound);
	else
		system->createSound(tempPath.c_str(), FMOD_LOOP_OFF, 0, &_sound);
}

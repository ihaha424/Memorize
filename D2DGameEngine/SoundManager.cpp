#include "SoundManager.h"
#include "ResourceManager.h"
#include <atlconv.h>

FMOD::System* SoundManager::system{};
FMOD::Channel* SoundManager::channel[SoundChnalList] = {};
float			SoundManager::volume[SoundChnalList] = {};

SoundManager::~SoundManager()
{
	system->release();
	system->close();
}


void SoundManager::Initialize()
{
	FMOD::System_Create(&system);
	system->init((int)SoundChnalList, FMOD_INIT_NORMAL, 0);


	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_BasicAttack01.wav", 0);
	ResourceManager::LoadResource(L"TestResource/Sound/Player/Sound_PlayerAttack01.wav", 0);

	SetVolume(30);
}

void SoundManager::PlayMusic(std::wstring _key, SoundChannel _channel)
{
	channel[_channel]->stop();
	system->playSound(ResourceManager::LoadResource<SoundResource>(_key).get()->GetResource(), NULL, 0, &channel[_channel]);
	channel[_channel]->setVolume(volume[_channel]);
}

void SoundManager::StopMusic(SoundChannel _channel)
{
	channel[_channel]->stop();
}

void SoundManager::SetVolume(float _volume)
{
	for (unsigned int i = 0; i < SoundChnalList; ++i)
	{
		volume[i] = _volume;
		channel[i]->setVolume(volume[i]);
	}
}

void SoundManager::SetVolume(float _volume, SoundChannel _channel)
{
	volume[_channel] = _volume;
	channel[_channel]->setVolume(volume[_channel]);
}

bool SoundManager::IsPlay(SoundChannel _channel)
{
	return false;
}

void SoundManager::CreateSound(std::wstring _key, bool loopcheck, FMOD::Sound*& _sound)
{
	std::string tempPath;
	USES_CONVERSION;
	tempPath = std::string(W2A(_key.c_str()));
	if (loopcheck)
		system->createSound(tempPath.c_str(), FMOD_LOOP_NORMAL, 0, &_sound);
	else
		system->createSound(tempPath.c_str(), FMOD_LOOP_OFF, 0, &_sound);
	if(!_sound)
	{
		LOG_ERROR(-1, dbg::text("Sound: \"", tempPath.c_str(), "\" is nullptr.\n"));
	}

}

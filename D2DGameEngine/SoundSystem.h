#pragma once

#ifdef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
#endif

#include "Singleton.h"


class SoundSystem : public Singleton<SoundSystem>
{
	using SoundChannel = int;
private:
	SoundSystem(const SoundSystem&) = delete;
	SoundSystem(SoundSystem&&) noexcept = delete;
	SoundSystem& operator=(const SoundSystem&) = delete;
	SoundSystem& operator=(SoundSystem&&) noexcept = delete;

	static constexpr unsigned int SoundChnalList = 32;

	static FMOD::System*	system;
	static FMOD::Channel*	channel[SoundChnalList];
	static float			volume[SoundChnalList];

public:
	SoundSystem();
	~SoundSystem();

	static void Initialize();

	static void PlayMusic(std::wstring _key, SoundChannel _channel);
	static void StopMusic(SoundChannel _channel);
	static void SetVolume(float _volume);
	static void SetVolume(float _volume, SoundChannel _channel);
	static bool IsPlay(SoundChannel _channel);

	static void CreateSound(std::wstring _key, bool loopcheck, FMOD::Sound*& _sound);

};


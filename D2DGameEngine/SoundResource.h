#pragma once

#ifndef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
#endif
#ifdef _WIN64
#include "fmod.hpp"
#pragma comment (lib, "fmod_vc.lib")
#endif

#include "Resource.h"

class SoundResource : public Resource<SoundResource, FMOD::Sound> {
public:
	SoundResource() {};
	~SoundResource();

	void LoadFile(std::wstring _key);
	void LoadFileLoop(std::wstring _key, bool LoopCheck);

	static const std::type_index TYPE_ID;
};


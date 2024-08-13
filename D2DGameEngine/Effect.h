#pragma once
#include "Resource.h"


class EffectResource: public Resource<EffectResource, ID2D1Bitmap>
{
public:
	EffectResource();
	~EffectResource();

	void LoadFile(std::wstring _key);

	static const std::type_index TYPE_ID;
};


#pragma once
#include "Resource.h"

class SpriteResource : public Resource<SpriteResource, ID2D1Bitmap> {
public:
	SpriteResource() {};
	~SpriteResource();

	void LoadFile(std::wstring _key);

	static const std::type_index TYPE_ID;
};

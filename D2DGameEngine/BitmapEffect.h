#pragma once

#include "IEffect.h"

class BitmapEffect
	: public IEffect
{
public:
	BitmapEffect(class World* _world);
	virtual ~BitmapEffect() {}

	void SetSprite(std::wstring _path);
protected:
	class BitmapComponent* bitmap;

};


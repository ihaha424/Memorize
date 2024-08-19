#pragma once

#include "IEffect.h"

class BitmapEffect
	: public IEffect
{
public:
	BitmapEffect(class World* _world);
	virtual ~BitmapEffect() {}


protected:
	class BitmapComponent* bitmap;

};


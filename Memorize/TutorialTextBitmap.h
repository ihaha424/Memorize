#pragma once
#include "D2DGameEngine/BitmapComponent.h"

class TutorialTextBitmap : public BitmapComponent
{
public:
	TutorialTextBitmap(Actor* _owner);
	virtual ~TutorialTextBitmap();

	virtual void Update(float _dt) override;

	void SetNext() { index++; }

	int index = 1;

	float timer = 0.f;

	class TutorialPlayer* player;

	//index 3->4
	Math::Vector2 playerPos;
};


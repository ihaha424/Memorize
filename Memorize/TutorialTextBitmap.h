#pragma once
#include "D2DGameEngine/BitmapComponent.h"

class TutorialTextBitmap : public BitmapComponent
{
public:
	TutorialTextBitmap(Actor* _owner);
	virtual ~TutorialTextBitmap();

	virtual void Update(float _dt) override;

	void SetNext() { index++; }

	int index = 0;
};


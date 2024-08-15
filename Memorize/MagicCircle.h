#pragma once
#include "D2DGameEngine/SceneComponent.h"

class MagicCircle : public SceneComponent
{
	std::vector<class BitmapComponent*> bms;
public:
	MagicCircle(Actor* _owner);
	virtual ~MagicCircle();

	virtual void Update(float _dt) override;

	void SetToFire();
	void SetToWater();
	void SetToLight();
	void SetToDark();
	void HideAll();
	void ShowOne(int index);
};


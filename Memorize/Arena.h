#pragma once

#include "common.h"

#include "D2DGameEngine/Actor.h"

#include "D2DGameEngine/D2DRenderer.h"

class Arena : public Actor
{
public:
	float earthquakeTimer{ 5.f };
	bool earthquake{ true };
	
	float shockwaveScale{ 0.f };
	float turbulance{ 500.f };
	
	class BitmapComponent* arena;
	
public:

	Arena(class World* _world);

	void Update(float _dt) override;
	void Render(class D2DRenderer* _renderer) override;

};

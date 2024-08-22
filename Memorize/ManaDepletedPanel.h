#pragma once
#include "D2DGameEngine/UIPanel.h"

class ManaDepletedPanel : public UIPanel
{
	float elapsedTime = 0.f;
	float duration = 2.f;
public:
	ManaDepletedPanel(World* _world);
	virtual ~ManaDepletedPanel();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	class UIText* text;
};


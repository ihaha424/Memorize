#pragma once
#include "D2DGameEngine\UIPanel.h"
class CursorUIPanel : public UIPanel
{
	class CursorImage* cursor;
public:
	CursorUIPanel(World* _world);
	virtual ~CursorUIPanel();

	virtual void Update(float _dt) override;

	void SetCursorImage(std::wstring path);
};


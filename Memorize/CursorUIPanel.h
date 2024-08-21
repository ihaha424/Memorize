#pragma once
#include "C:\Users\sjndl\OneDrive\πŸ≈¡ »≠∏È\Memorize\D2DGameEngine\UIPanel.h"
class CursorUIPanel : public UIPanel
{
	class UIImage* cursor;
public:
	CursorUIPanel(World* _world);
	virtual ~CursorUIPanel();

	virtual void Update(float _dt) override;

	void SetCursorImage(std::wstring path);
};


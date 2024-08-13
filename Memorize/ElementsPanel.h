#pragma once
#include "D2DGameEngine/UIPanel.h"

class ElementsPanel : public UIPanel
{
public:
	ElementsPanel(World* _world);
	virtual ~ElementsPanel();

	class UIImage* q;
	class UIImage* w;
	class UIImage* e;
	class UIImage* r;

	void SelectQ();
	void SelectW();
	void SelectE();
	void SelectR();

	void UnselectQ();
	void UnselectW();
	void UnselectE();
	void UnselectR();

	void OnPressedQ();
	void OnPressedW();
};


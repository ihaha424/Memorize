#pragma once
#include "D2DGameEngine/UIPanel.h"
#include "D2DGameEngine/DotTween.h"

class FadeInFadeOut : public UIPanel
{
	class UIImage* image;
public:
	FadeInFadeOut(World* _world);
	virtual ~FadeInFadeOut();


	void SetColor(bool _value = true) { color = _value; } // true: Black | false: White
	void SetFaidInOut(bool _value = true) { inOut = _value; } // true: In | false: Out
	void SetFinishFunction(std::function<void()> _func) { func = _func; }

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	

	DotTween<float>* alphaTween;
private:
	bool	color{true};
	bool	inOut{true};
	std::function<void()> func = nullptr;

	float alpha;
};


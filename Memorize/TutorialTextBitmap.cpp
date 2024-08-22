#include "TutorialTextBitmap.h"

TutorialTextBitmap::TutorialTextBitmap(Actor* _owner) : BitmapComponent(_owner)
{
	
}

TutorialTextBitmap::~TutorialTextBitmap()
{
}

void TutorialTextBitmap::Update(float _dt)
{
	__super::Update(_dt);
	SetSprite((L"TestResource/Tutorial/tutorial_text_" + std::to_wstring(index) + L".png").c_str());

}

#include "WinEnding.h"
#include "../D2DGameEngine/SoundManager.h"
#include "FadeInFadeOut.h"
#include "WinEndingPanel.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "CursorUIPanel.h"


WinEnding::WinEnding(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

WinEnding::~WinEnding()
{}

void WinEnding::Enter()
{
	SoundManager::PlayMusic(L"TestResource/Sound/BG/Sound_ClearBGM.wav", 0);

	WinEndingPanel* cutCin = GetWorld()->GetCanvas()->CreatePannel<WinEndingPanel>(L"EnddingCutsinPanel");

	FadeInFadeOut* playerDie = GetWorld()->GetCanvas()->CreatePannel<FadeInFadeOut>(L"EnddingFadein");
	playerDie->alphaTween->SetDuration(5.f);
	playerDie->alphaTween->SetEasingEffect(EasingEffect::InQuart);
	playerDie->SetFinishFunction([=]()->void {
		cutCin->Activate();
		playerDie->Inactivate();
		GetWorld()->GetCanvas()->CreatePannel<CursorUIPanel>(L"Cursor");
		});



	__super::Enter();
}

void WinEnding::Exit()
{
	__super::Exit();
}

void WinEnding::Clear()
{
}

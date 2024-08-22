#include "Ending.h"
#include "../D2DGameEngine/SoundManager.h"
#include "FadeInFadeOut.h"
#include "EnddingPanel.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "CursorUIPanel.h"

Ending::Ending(World* _world, const std::wstring& _name)
	: Level(_world, _name)
{}

Ending::~Ending()
{}

void Ending::Enter()
{
	SoundManager::PlayMusic(L"TestResource/Sound/BG/Sound_DeathBGM.wav", 0);

	EnddingPanel* cutCin = GetWorld()->GetCanvas()->CreatePannel<EnddingPanel>(L"EnddingCutsinPanel");

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

void Ending::Exit()
{
	__super::Exit();
}

void Ending::Clear()
{
}

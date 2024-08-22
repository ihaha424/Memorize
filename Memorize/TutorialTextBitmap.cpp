#include "TutorialTextBitmap.h"
#include "TutorialPlayer.h"

TutorialTextBitmap::TutorialTextBitmap(Actor* _owner) : BitmapComponent(_owner)
{
	AddTickProperties(TICK_UPDATE);
	SetSprite(L"TestResource/Tutorial/tutorial_text_1.png");
	player = static_cast<TutorialPlayer*>(_owner);
}

TutorialTextBitmap::~TutorialTextBitmap()
{
}

void TutorialTextBitmap::Update(float _dt)
{
	__super::Update(_dt);
	SetSprite((L"TestResource/Tutorial/tutorial_text_" + std::to_wstring(index) + L".png").c_str());

	timer += _dt;

	//index = 1 : 드디어 여기까지 왔네.

	if (index == 1 && timer > 3.f) //긴장한거야? 왜 이렇게 벙쪄있어?
	{
		index = 2;
		timer = 0.f;
	}

	if (index == 2 && timer > 3.f) //늘 그랫듯 우클릭으로 돌아다녀봐.
	{
		index = 3;
		timer = 0.f;
		playerPos = player->GetLocation();
	}

	if (index == 3 && playerPos == player->GetLocation()) //(플레이어 이동 체크, 이후 3초 지나야 함)
	{
		timer = 0.f;
	}
	if (index == 3 && timer > 3.f) //긴장했나보네, 마법도 잊어버렸겠는데.
	{
		index = 4;
		timer = 0.f;
	}

	if (index == 4 && timer > 3.f) //QWER로 주문을 외울 수 있어.
	{
		index = 5;
		timer = 0.f;
	}

	if (index == 5 && timer > 3.f) //QQ를 눌러 간단한 파이어볼 한번 써보겠어?
	{
		index = 6;
		timer = 0.f;
	}
}

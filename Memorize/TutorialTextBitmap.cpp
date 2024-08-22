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

	//index = 1 : ���� ������� �Գ�.

	if (index == 1 && timer > 3.f) //�����Ѱž�? �� �̷��� �����־�?
	{
		index = 2;
		timer = 0.f;
	}

	if (index == 2 && timer > 3.f) //�� �׷��� ��Ŭ������ ���ƴٳ��.
	{
		index = 3;
		timer = 0.f;
		playerPos = player->GetLocation();
	}

	if (index == 3 && playerPos == player->GetLocation()) //(�÷��̾� �̵� üũ, ���� 3�� ������ ��)
	{
		timer = 0.f;
	}
	if (index == 3 && timer > 3.f) //�����߳�����, ������ �ؾ���Ȱڴµ�.
	{
		index = 4;
		timer = 0.f;
	}

	if (index == 4 && timer > 3.f) //QWER�� �ֹ��� �ܿ� �� �־�.
	{
		index = 5;
		timer = 0.f;
	}

	if (index == 5 && timer > 3.f) //QQ�� ���� ������ ���̾ �ѹ� �Ẹ�ھ�?
	{
		index = 6;
		timer = 0.f;
	}
}

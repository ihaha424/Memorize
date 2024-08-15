#include "MagicCircle.h"
#include "Player.h"
#include "GPlayerController.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/BitmapComponent.h"

MagicCircle::MagicCircle(Actor* _owner) : SceneComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
	for (int i = 0; i < 4; i++)
	{
		bms.push_back(_owner->CreateComponent<BitmapComponent>());
		AddChild(bms[i]);
	}
	SetToFire();
	HideAll();
}

MagicCircle::~MagicCircle()
{
}

void MagicCircle::Update(float _dt)
{
	__super::Update(_dt);

	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	Math::Vector2 direction = mousePos - Math::Vector2(GetOwner()->GetLocation().x, GetOwner()->GetLocation().y);
	direction.Normalize();

	SetTranslation(direction.x + 50 * direction.x, 0 );

	for (int i = 0; i < 4; i++)
	{
		bms[i]->SetScale(0.75 * direction.Dot(Math::Vector2(0, 1)) + 0.25, 1);
	}


	Player* player = static_cast<Player*>(GetOwner());
	GPlayerController* controller = static_cast<GPlayerController*>(player->GetController());

	ESkillType curSkillType = controller->GetCurSkillInfo().type;
	ESkillElement curSkillElement = controller->GetCurSkillInfo().element;

	if (curSkillElement == SE_FIRE)
		SetToFire();
	else if (curSkillElement == SE_WATER)
		SetToWater();
	else if (curSkillElement == SE_LIGHT)
		SetToLight();
	else if (curSkillElement == SE_DARKNESS)
		SetToDark();

	if (curSkillType != ST_END)
	{
		//커맨드를 입력중
		int index = controller->GetPlayerCastingIndex() - 1;
		if(index >= 0)
			ShowOne(index);

		if (controller->GetPlayerState() != L"PlayerAttackReady" 
			&& controller->GetPlayerState() != L"PlayerCasting")
			HideAll();
	}

}

void MagicCircle::SetToFire()
{
	bms[0]->SetSprite(L"TestResource/Player/MagicCircle/Fire/Magic_Circle_F1.png");
	bms[1]->SetSprite(L"TestResource/Player/MagicCircle/Fire/Magic_Circle_F2.png");
	bms[2]->SetSprite(L"TestResource/Player/MagicCircle/Fire/Magic_Circle_F3.png");
	bms[3]->SetSprite(L"TestResource/Player/MagicCircle/Fire/Magic_Circle_F4.png");
}

void MagicCircle::SetToWater()
{
	bms[0]->SetSprite(L"TestResource/Player/MagicCircle/Water/Magic_Circle_W1.png");
	bms[1]->SetSprite(L"TestResource/Player/MagicCircle/Water/Magic_Circle_W2.png");
	bms[2]->SetSprite(L"TestResource/Player/MagicCircle/Water/Magic_Circle_W3.png");
	bms[3]->SetSprite(L"TestResource/Player/MagicCircle/Water/Magic_Circle_W4.png");
}

void MagicCircle::SetToLight()
{
	bms[0]->SetSprite(L"TestResource/Player/MagicCircle/Light/Magic_Circle_L1.png");
	bms[1]->SetSprite(L"TestResource/Player/MagicCircle/Light/Magic_Circle_L2.png");
	bms[2]->SetSprite(L"TestResource/Player/MagicCircle/Light/Magic_Circle_L3.png");
	bms[3]->SetSprite(L"TestResource/Player/MagicCircle/Light/Magic_Circle_L4.png");
}

void MagicCircle::SetToDark()
{
	bms[0]->SetSprite(L"TestResource/Player/MagicCircle/Dark/Magic_Circle_D1.png");
	bms[1]->SetSprite(L"TestResource/Player/MagicCircle/Dark/Magic_Circle_D2.png");
	bms[2]->SetSprite(L"TestResource/Player/MagicCircle/Dark/Magic_Circle_D3.png");
	bms[3]->SetSprite(L"TestResource/Player/MagicCircle/Dark/Magic_Circle_D4.png");
}

void MagicCircle::HideAll()
{
	for (int i = 0; i < 4; i++)
	{
		bms[i]->SetStatus(OS_INACTIVE);
	}
}

void MagicCircle::ShowOne(int index)
{
	bms[index]->SetStatus(OS_ACTIVE);
}

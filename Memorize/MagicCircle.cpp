#include "MagicCircle.h"
#include "Player.h"
#include "GPlayerController.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"

MagicCircle::MagicCircle(Actor* _owner) : SceneComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
	circles.resize(7);
	animStates.resize(7);

	for (int i = 0; i < 7; i++)
	{
		circles[i] = _owner->CreateComponent<Animator>();
		AddChild(circles[i]);
		animStates[i] = circles[i]->CreateState<AnimationState>();
		animStates[i]->Trigger(true);
		circles[i]->Initialize(animStates[i]);
	}
	SetToFire();
	HideAll();

	prevType = ESkillType::ST_END;
	prevElement = ESkillElement::SE_END;
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

	for (int i = 0; i < 7; i++)
	{
		circles[i]->SetScale(abs(0.75 * direction.Dot(Math::Vector2(0, 1))) + 0.25, 1);
	}


	Player* player = static_cast<Player*>(GetOwner());
	GPlayerController* controller = static_cast<GPlayerController*>(player->GetController());

	ESkillType curSkillType = controller->GetCurSkillInfo().type;
	ESkillElement curSkillElement = controller->GetCurSkillInfo().element;

	if (prevElement != curSkillElement)
	{
		if (curSkillElement == SE_FIRE)
			SetToFire();
		else if (curSkillElement == SE_WATER)
			SetToWater();
		else if (curSkillElement == SE_LIGHT)
			SetToLight();
		else if (curSkillElement == SE_DARKNESS)
			SetToDark();
		else
			HideAll();
	}

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
	animStates[0]->SetSprite(L"TestResource/Player/Circle/FireCircle01.png");
	animStates[1]->SetSprite(L"TestResource/Player/Circle/FireCircle02.png");
	animStates[2]->SetSprite(L"TestResource/Player/Circle/FireCircle03.png");
	animStates[3]->SetSprite(L"TestResource/Player/Circle/FireCircle04.png");
	animStates[4]->SetSprite(L"TestResource/Player/Circle/FireCircle05.png");
	animStates[5]->SetSprite(L"TestResource/Player/Circle/FireCircle06.png");
	animStates[6]->SetSprite(L"TestResource/Player/Circle/FireCircle07.png");

	for (int i = 0; i < 7; i++)
	{
		animStates[i]->SliceSpriteSheet(260, 260, 0, 0, 0, 0);
		animStates[i]->SetFrameDurations({ 0.05f });
	}
}

void MagicCircle::SetToWater()
{
	animStates[0]->SetSprite(L"TestResource/Player/Circle/WaterCircle01.png");
	animStates[1]->SetSprite(L"TestResource/Player/Circle/WaterCircle02.png");
	animStates[2]->SetSprite(L"TestResource/Player/Circle/WaterCircle03.png");
	animStates[3]->SetSprite(L"TestResource/Player/Circle/WaterCircle04.png");
	animStates[4]->SetSprite(L"TestResource/Player/Circle/WaterCircle05.png");
	animStates[5]->SetSprite(L"TestResource/Player/Circle/WaterCircle06.png");
	animStates[6]->SetSprite(L"TestResource/Player/Circle/WaterCircle07.png");
	for (int i = 0; i < 7; i++)
	{
		animStates[i]->SliceSpriteSheet(260, 260, 0, 0, 0, 0);
		animStates[i]->SetFrameDurations({ 0.05f });
	}
}

void MagicCircle::SetToLight()
{
	animStates[0]->SetSprite(L"TestResource/Player/Circle/LightCircle01.png");
	animStates[1]->SetSprite(L"TestResource/Player/Circle/LightCircle02.png");
	animStates[2]->SetSprite(L"TestResource/Player/Circle/LightCircle03.png");
	animStates[3]->SetSprite(L"TestResource/Player/Circle/LightCircle04.png");
	animStates[4]->SetSprite(L"TestResource/Player/Circle/LightCircle05.png");
	animStates[5]->SetSprite(L"TestResource/Player/Circle/LightCircle06.png");
	animStates[6]->SetSprite(L"TestResource/Player/Circle/LightCircle07.png");
	for (int i = 0; i < 7; i++)
	{
		animStates[i]->SliceSpriteSheet(260, 260, 0, 0, 0, 0);
		animStates[i]->SetFrameDurations({ 0.05f });
	}
}

void MagicCircle::SetToDark()
{
	animStates[0]->SetSprite(L"TestResource/Player/Circle/DarkCircle01.png");
	animStates[1]->SetSprite(L"TestResource/Player/Circle/DarkCircle02.png");
	animStates[2]->SetSprite(L"TestResource/Player/Circle/DarkCircle03.png");
	animStates[3]->SetSprite(L"TestResource/Player/Circle/DarkCircle04.png");
	animStates[4]->SetSprite(L"TestResource/Player/Circle/DarkCircle05.png");
	animStates[5]->SetSprite(L"TestResource/Player/Circle/DarkCircle06.png");
	animStates[6]->SetSprite(L"TestResource/Player/Circle/DarkCircle07.png");
	for (int i = 0; i < 7; i++)
	{
		animStates[i]->SliceSpriteSheet(260, 260, 0, 0, 0, 0);
		animStates[i]->SetFrameDurations({ 0.05f });
	}
}

void MagicCircle::HideAll()
{
	for (int i = 0; i < 7; i++)
	{
		circles[i]->SetStatus(OS_INACTIVE);
	}
}

void MagicCircle::ShowOne(int index)
{
	int i = 0;
	for(i = 0; i < index + 1; i++)
		circles[i]->SetStatus(OS_ACTIVE);
	for(; i < 7; i++)
		circles[i]->SetStatus(OS_INACTIVE);
}

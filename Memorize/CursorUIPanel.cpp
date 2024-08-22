#include "CursorUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "CursorImage.h"

CursorUIPanel::CursorUIPanel(World* _world) : UIPanel(_world)
{
	SetZOrder(INT_MAX);
	cursor = CreateUI<CursorImage>(L"Cursor");
}

CursorUIPanel::~CursorUIPanel()
{
}

void CursorUIPanel::Update(float _dt)
{
	__super::Update(_dt);

	Math::Vector2 destPos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	cursor->SetPosition(destPos.x, destPos.y);

}

void CursorUIPanel::SetCursorImage(std::wstring path)
{
	cursor->SetSprite(path);
}

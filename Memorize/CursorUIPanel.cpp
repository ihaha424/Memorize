#include "CursorUIPanel.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/Mouse.h"
#include "D2DGameEngine/UIImage.h"

CursorUIPanel::CursorUIPanel(World* _world) : UIPanel(_world)
{
	SetZOrder(INT_MAX);
	cursor = CreateUI<UIImage>(L"Cursor");
	cursor->SetSprite(L"TestResource/Cursors/Cursor_Default.png");
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

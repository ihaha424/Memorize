﻿#pragma once

#include "framework.h"

#include <wrl.h>
using namespace Microsoft::WRL;
#include <d2d1.h>
#include <d2d1helper.h>
#include <d2d1effects.h>
#include <d2d1effects_1.h>
#include <d2d1effects_2.h>
#include <dcommon.h>

enum class FontWeight {
	Thin = 100,
	ExtraLight = 200,
	Light = 300,
	Normal = 400,
	Medium = 500,
	SemiBold = 600,
	Bold = 700,
	ExtraBold = 800,
	Heavy = 900
};

enum class FontStyle {
	Normal,
	Italic,
};

enum class FontStretch {
	UltraCondensed = 1,
	ExtraCondensed = 2,
	Condensed = 3,
	SemiCondensed = 4,
	Normal = 5,
	Medium = 5,
	SemiExpended = 6,
	Expended = 7,
	ExtraExpended = 8,
	UltraExpended = 9
};

struct TextFormatInfo {
	const wchar_t* fontFamilyName;
	uint fontSize;
	FontWeight fontWeight{ FontWeight::Normal };
	FontStyle fontStyle{ FontStyle::Normal };
	FontStretch fontStretch{ FontStretch::Normal };
};

/**
 * @brief D2D 렌더링 엔진
 */
class D2DRenderer {
	HWND hWnd;

	// 종료시 텍스트 포멧을 릴리즈 하기 위한 쓰레기통.
	//std::unordered_set<struct IDWriteTextFormat*> textFormatGarbageCollector;

	std::unique_ptr<struct ID2D1HwndRenderTarget> renderTarget;
	std::unique_ptr<struct ID2D1DeviceContext> dc;
	std::unique_ptr<struct IDXGIAdapter3> _DXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스

	struct ID2D1SolidColorBrush* brush;

	Math::Matrix globalTransform{ Math::Matrix::Identity };	// Identity
	std::vector<Math::Matrix> transforms;

public:

	ID2D1HwndRenderTarget* GetRenderTarget() { return renderTarget.get(); }
	ID2D1DeviceContext* GetDC() { return dc.get(); }

	D2DRenderer(HWND hWnd);
	~D2DRenderer();

	void BeginDraw();
	void EndDraw();

	void DrawCircle(
		const Math::Vector2& center, float rad,
		D2D1::ColorF color = D2D1::ColorF::White
	);
	void DrawBox(
		const Math::Vector2& ul,
		const Math::Vector2& lr,
		D2D1::ColorF color = D2D1::ColorF::White
	);
	void DrawCapsule(
		const Math::Vector2& center,
		float halfHeight, float radius,
		D2D1::ColorF color = D2D1::ColorF::White
	);
	void DrawBorder(
		const Math::Vector2& ul, const Math::Vector2& lr,
		D2D_Color color = D2D_Color::White
	);
	void DrawPolygon(
		const std::vector<Math::Vector2>& points,
		D2D_Color color = D2D_Color::White
	);
	/**
	 * @brief 텍스트 포멧 정보를 통해 정해진 위치에 문자열을 그립니다.
	 * @param str 화면에 표시될 문자열
	 * @param textFormatInfo 텍스트 포멧 정보
	 * @param ul 좌측 상단 포지션
	 * @param lr 우측 하단 포지션
	 */
	void DrawString(
		const std::wstring& str, const TextFormatInfo* textFormatInfo,
		const Math::Vector2& ul, const Math::Vector2& lr,
		D2D_Color color = D2D_Color::White
	);

	void DrawSprite(
		D2D_Sprite* sprite
	);

	void DrawSprite(
		D2D_Sprite* sprite,
		const Math::Vector2& ul, 
		const Math::Vector2& lr,
		float opacity = 1.f
	);
	/**
	 * @brief 스프라이트를 렌더 타겟에 그립니다.
	 * @param sprite 스프라이트 포인터
	 * @param dst 스프라이트가 놓일 위치
	 * @param srcArea 스프라이트에서 클립될 범위
	 */
	void DrawSprite(
		D2D_Sprite* sprite,
		const D2D_RectF& dst, 
		const D2D_RectF& srcArea,
		float opacity = 1.f
	);

	// Transform operators
	
	/**
	 * @brief 새로운 트랜스폼 메트릭스를 글로벌 트랜스폼 스택에 넣습니다.
	 * 예시, 다음의 일렬의 코드를 통해서 월드, 뷰 트랜스폼을 스프라이트에 적용합니다. 
	 * ```cpp
	 * // ...
	 *	PushTransform(ViewTransform); 
	 *	PushTransform(WorldTransform);
	 *  DrawSprite(sprite, {0.f, 0.f, 100.f, 100.f});
	 * // ...
	 * ```
	 * @param mat 트랜스폼 스텍에 넣을 매트릭스
	 */
	void PushTransform(const Math::Matrix& mat);
	/**
	 * @brief 글로벌 트랜스폼 스택에서 이전의 트랜스폼 메트릭스를 팝합니다.
	 */
	void PopTransform();
	/**
	 * @brief 글로벌 트랜스폼 스택을 클리어 하고, 렌더 타겟 트랜스폼을 초기화합니다.
	 */
	void ClearTransform();
	/**
	 * @brief 현재 글로벌 트랜스폼 스택에 들어있는 모든 메트릭스의 곱을 반환합니다.
	 * @return 현재 글로벌 트랜스폼 메트릭스
	 */
	Math::Matrix GetGlobalTransform();

	void ResizeScreen(int w, int h);

	void ShowVRAMUsage();

private:

	void CreateRenderTarget();
};

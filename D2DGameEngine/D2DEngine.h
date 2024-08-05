#pragma once

#include "framework.h"

/**
 * @brief D2D 렌더링 엔진
 */
class D2DEngine {
	HWND _hWnd;

	std::unique_ptr<struct ID2D1HwndRenderTarget> _renderTarget;
	std::unique_ptr<struct IDWriteTextFormat> _textFormat;
	std::unique_ptr<struct IDXGIAdapter3> _DXGIAdapter;		// 비디오카드 정보에 접근 가능한 인터페이스

	struct ID2D1SolidColorBrush* _brush;

	D2D_Mat3x2F globalTransform;	// Identity
	std::vector<D2D_Mat3x2F> _transforms;

public:

	struct ID2D1HwndRenderTarget* GetRenderTarget();

	D2DEngine(HWND hWnd);
	~D2DEngine();

	void BeginDraw();
	void EndDraw();

	void DrawCircle(
		const D2D_Point2F& center, float rad,
		D2D1::ColorF color = D2D1::ColorF::White
	);
	void DrawBorder(
		const D2D_Point2F& ul, const D2D_Point2F& lr,
		D2D_Color color = D2D_Color::White
	);
	void DrawPolygon(
		const std::vector<D2D_Point2F>& points,
		D2D_Color color = D2D_Color::White
	);
	void DrawString(
		const std::wstring& str,
		const D2D_Point2F& ul, const D2D_Point2F& lr
	);
	void DrawSprite(
		D2D_Sprite* sprite,
		const D2D_Point2F& ul, const D2D_Point2F& lr
	);
	void DrawSprite(
		D2D_Sprite* sprite,
		const D2D_RectF& dst, const D2D_RectF& srcArea
	);

	// Transform operators
	
	/**
	 * @brief 새로운 트랜스폼 메트릭스를 글로벌 트랜스폼 스택에 넣습니다.
	 * 예시, 다음의 일렬의 코드를 통해서 뷰, 월드 트랜스폼을 스프라이트에 적용합니다. 
	 * ```cpp
	 * // ...
	 *	PushTransform(ViewTransform); 
	 *	PushTransform(WorldTransform);
	 *  DrawSprite(sprite, {0.f, 0.f, 100.f, 100.f});
	 * // ...
	 * ```
	 * @param mat 트랜스폼 스텍에 넣을 매트릭스
	 */
	void PushTransform(const D2D_Mat3x2F& mat);
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
	D2D_Mat3x2F GetGlobalTransform();

	void ResizeScreen(int w, int h);

	void ShowVRAMUsage();

private:

	void CreateRenderTarget();
};

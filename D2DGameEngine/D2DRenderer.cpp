#include "D2DRenderer.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dxgi1_4.h>

#include "FactoryManager.h"

D2DRenderer::D2DRenderer(HWND hWnd) :
	_hWnd{ hWnd }, _renderTarget{ nullptr } {

	// Initialize Factory
	FactoryManager::Create();
	// Initialize the render target
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(_hWnd, &rc);

	// Create a Direct2D render target
	CreateRenderTarget();

	// Create the DXGI adapter
	HRESULT res;
	IDXGIAdapter3* dxgiAdaptor = nullptr;
	res = FactoryManager::GetDXGIFactory4().EnumAdapters(
		0, reinterpret_cast<IDXGIAdapter**>(&dxgiAdaptor)
	);
	_DXGIAdapter.reset(dxgiAdaptor);
}

D2DRenderer::~D2DRenderer() {
	// Release the text format
	//SafeRelease(_textFormat);

	// Release the DXGI adaptor
	SafeRelease(_DXGIAdapter);

	// Free D2D Objects and render target.
	SafeRelease(&_brush);
	SafeRelease(_renderTarget);

	// CoUninitialize
	FactoryManager::Destroy();
}

ID2D1HwndRenderTarget* D2DRenderer::GetRenderTarget() {
	return _renderTarget.get();
}

void D2DRenderer::BeginDraw() {
	_renderTarget->BeginDraw();
	_renderTarget->Clear();
	_renderTarget->SetTransform(D2D_Mat3x2F::Identity());
}

void D2DRenderer::EndDraw() {
	_renderTarget->EndDraw();
}

void D2DRenderer::DrawCircle(
	const Math::Vector2& center, float rad,
	D2D1::ColorF color) {
	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);
	_renderTarget->DrawEllipse(
		{ {center.x, center.y}, rad, rad }, _brush
	);
	_brush->SetColor(tmp);
}

void D2DRenderer::DrawBorder(
	const Math::Vector2& ul, const Math::Vector2& lr,
	D2D_Color color) {
	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);
	_renderTarget->DrawRectangle(
		{ ul.x, ul.y, lr.x, lr.y }, _brush
	);
	_brush->SetColor(tmp);
}

void D2DRenderer::DrawPolygon(
	const std::vector<Math::Vector2>& points,
	D2D_Color color) {
	auto& d2d1Fac = FactoryManager::GetGraphicsFactory();

	ID2D1PathGeometry* path{ nullptr };
	auto res = d2d1Fac.CreatePathGeometry(&path);
	if (!SUCCEEDED(res)) return;

	ID2D1GeometrySink* sink{ nullptr };
	res = path->Open(&sink);
	if (!SUCCEEDED(res)) return;

	std::vector<D2D_Point2F> d2dPoints(points.size());
	for (int i = 0; i < points.size(); ++i) {
		d2dPoints[i] = ToD2DP2F(points[i]);
	}

	sink->BeginFigure(d2dPoints[0],
		D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddLines(d2dPoints.data(), points.size());
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);
	res = sink->Close();
	SafeRelease(&sink);

	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);

	_renderTarget->DrawGeometry(path, _brush, 1.f);

	_brush->SetColor(tmp);
}

/**
 * @brief 
 * @param str 
 * @param dst A struct of FLOAT left, top, right, bottom 
 */
void D2DRenderer::DrawString(
	const std::wstring& str, 
	const TextFormatInfo* textFormatInfo,
	const Math::Vector2& ul,
	const Math::Vector2& lr,
	D2D_Color color) {
	// 텍스트 포멧 설정
	HRESULT res;
	IDWriteTextFormat* textFormat = nullptr;
	res = FactoryManager::GetDWriteFactory().CreateTextFormat(
		textFormatInfo->fontFamilyName,
		nullptr,
		static_cast<DWRITE_FONT_WEIGHT>(textFormatInfo->fontWeight),
		static_cast<DWRITE_FONT_STYLE>(textFormatInfo->fontStyle),
		static_cast<DWRITE_FONT_STRETCH>(textFormatInfo->fontStretch),
		textFormatInfo->fontSize,
		L"",
		&textFormat
	);
	// 텍스트 컬러 설정
	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);

	_renderTarget->DrawText(
		str.c_str(),
		str.size(),
		textFormat,
		{ul.x, ul.y, lr.x, lr.y},
		_brush
	);

	_brush->SetColor(tmp);
}

void D2DRenderer::DrawSprite(
	D2D_Sprite* sprite,
	const Math::Vector2& ul, const Math::Vector2& lr
) {
	if (!sprite) return;
	_renderTarget->DrawBitmap(
		sprite,
		D2D1::RectF(ul.x, ul.y, lr.x, lr.y)
	);
}

void D2DRenderer::DrawSprite(
	D2D_Sprite* sprite,
	const D2D_RectF& dst, 
	const D2D_RectF& srcArea
) {
	if (!sprite) return;
	_renderTarget->DrawBitmap(
		sprite,
		&dst,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&srcArea
	);
}

void D2DRenderer::PushTransform(const Math::Matrix& mat) {
	// 글로벌 트랜스폼을 업데이트 합니다.
	globalTransform = mat * globalTransform;
	// 새로운 트렌스폼을 트랜스폼 스택에 넣습니다.
	_transforms.push_back(mat);
	// 렌더 타겟 트랜스폼을 세팅합니다.
	_renderTarget->SetTransform(ToD2DMat(globalTransform));
}

void D2DRenderer::PopTransform() {
	// 이전의 트랜스폼을 가져옵니다.
	Math::Matrix mat = _transforms.back();
	// 트렌스폼의 역을 계산한 후 글로벌 트랜스폼에
	// 곱해서 이전 트랜스폼을 캔슬 합니다.
	mat.Invert();
	globalTransform = mat * globalTransform;
	// 이전 트렌스폼을 스택에서 팝합니다.
	_transforms.pop_back();
	// 렌더 타겟 트랜스폼을 세팅합니다.
	_renderTarget->SetTransform(ToD2DMat(globalTransform));
}

void D2DRenderer::ClearTransform() {
	globalTransform = Math::Matrix::Identity;
	_transforms.clear();
}

Math::Matrix D2DRenderer::GetGlobalTransform() {
	return globalTransform;
}

void D2DRenderer::ResizeScreen(int w, int h) {
	_renderTarget->Resize(D2D1::SizeU(w, h));
}

void D2DRenderer::ShowVRAMUsage() {
	static TextFormatInfo textInfo{
		.fontFamilyName = L"Arial",
		.fontSize = 20
	};

	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	_DXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
	float usage = videoMemoryInfo.CurrentUsage / 1024.f / 1024.f;

	std::wstring output = L"VRAM: " + std::to_wstring(usage) + L"MB";

	DrawString(output, &textInfo, { 0.f, 0.f }, { 150.f, 24.f });
}

void D2DRenderer::CreateRenderTarget() {
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(_hWnd, &rc);

	// Create a Direct2D render target
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	HRESULT hr = FactoryManager::GetGraphicsFactory()
		.CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				_hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
			),
			&renderTarget
		);

	// Reset the unique pointer
	_renderTarget.reset(renderTarget);

	// Create a brush
	if (SUCCEEDED(hr)) {
		// Create a brush.
		_renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&_brush
		);
	}
}

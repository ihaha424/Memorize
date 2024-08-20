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
	hWnd{ hWnd }, renderTarget{ nullptr } {

	// Initialize Factory
	FactoryManager::Create();
	// Initialize the render target
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(hWnd, &rc);

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
	SafeRelease(&brush);
	SafeRelease(renderTarget);

	// CoUninitialize
	FactoryManager::Destroy();
}

void D2DRenderer::BeginDraw() {
	renderTarget->BeginDraw();
	renderTarget->Clear();
	
	// y 축 플립해서 월드 좌표계에서 위쪽이 +y, 아래가 -y가 되게 함
	globalTransform = globalTransform.CreateScale(1.f, 1.f, 1.f);
	renderTarget->SetTransform(ToD2DMat(globalTransform));
}

void D2DRenderer::EndDraw() {
	renderTarget->EndDraw();
}

void D2DRenderer::DrawCircle(
	const Math::Vector2& center, float rad,
	D2D1::ColorF color) {
	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);
	renderTarget->DrawEllipse(
		{ {center.x, center.y}, rad, rad }, brush
	);
	brush->SetColor(tmp);
}

void D2DRenderer::DrawBox(
	const Math::Vector2& ul, 
	const Math::Vector2& lr,
	D2D1::ColorF color)
{
	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);
	renderTarget->DrawRectangle(
		{ ul.x, ul.y, lr.x, lr.y }, brush
	);
	brush->SetColor(tmp);
}

void D2DRenderer::DrawCapsule(
	const Math::Vector2& center, 
	float halfHeight, float radius,
	D2D1::ColorF color)
{
	auto& d2d1Fac = FactoryManager::GetGraphicsFactory();

	ID2D1PathGeometry* path{ nullptr };
	auto res = d2d1Fac.CreatePathGeometry(&path);
	if (!SUCCEEDED(res)) return;

	ID2D1GeometrySink* sink{ nullptr };
	res = path->Open(&sink);
	if (!SUCCEEDED(res)) return;

	// TODO
	D2D_Point2F ul{ center.x - radius, center.y - halfHeight };
	D2D_Point2F ur{ center.x + radius, center.y - halfHeight };
	D2D_Point2F lr{ center.x + radius, center.y + halfHeight };
	D2D_Point2F ll{ center.x - radius, center.y + halfHeight };

	sink->BeginFigure(ul, D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddArc(
		D2D1::ArcSegment(
			ur,	// end point 
			{ radius, radius },
			180.0f,	// rotation angle
			D2D1_SWEEP_DIRECTION_CLOCKWISE,
			D2D1_ARC_SIZE_SMALL	// sweep 180 degrees or less
		)
	);
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	sink->BeginFigure(ur, D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddLine(lr);
	sink->EndFigure(D2D1_FIGURE_END_OPEN);

	sink->BeginFigure(lr, D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddArc(
		D2D1::ArcSegment(
			ll,	// end point 
			{ radius, radius },
			180.0f,	// rotation angle
			D2D1_SWEEP_DIRECTION_CLOCKWISE,
			D2D1_ARC_SIZE_SMALL	// sweep 180 degrees or less
		)
	);
	sink->EndFigure(D2D1_FIGURE_END_CLOSED);

	sink->BeginFigure(ll, D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddLine(ul);
	sink->EndFigure(D2D1_FIGURE_END_OPEN);

	res = sink->Close();
	SafeRelease(&sink);

	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);

	renderTarget->DrawGeometry(path, brush, 1.f);

	brush->SetColor(tmp);
}

void D2DRenderer::DrawBorder(
	const Math::Vector2& ul, const Math::Vector2& lr,
	D2D_Color color) {
	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);
	renderTarget->DrawRectangle(
		{ ul.x, ul.y, lr.x, lr.y }, brush
	);
	brush->SetColor(tmp);
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

	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);

	renderTarget->DrawGeometry(path, brush, 1.f);

	brush->SetColor(tmp);
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
	D2D_TColor tmp = brush->GetColor();
	brush->SetColor(color);

	renderTarget->DrawText(
		str.c_str(),
		str.size(),
		textFormat,
		{ul.x, ul.y, lr.x, lr.y},
		brush
	);

	brush->SetColor(tmp);
}

void D2DRenderer::DrawSprite(
	D2D_Sprite* sprite,
	const Math::Vector2& ul, 
	const Math::Vector2& lr,
	float opacity
) {
	if (!sprite) return;
	renderTarget->DrawBitmap(
		sprite,
		D2D1::RectF(ul.x, ul.y, lr.x, lr.y),
		opacity
	);
}

void D2DRenderer::DrawSprite(
	D2D_Sprite* sprite,
	const D2D_RectF& dst, 
	const D2D_RectF& srcArea,
	float opacity
) {
	if (!sprite) return;
	renderTarget->DrawBitmap(
		sprite,
		&dst,
		opacity,
		D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
		&srcArea
	);
}

void D2DRenderer::PushTransform(const Math::Matrix& mat) {
	// 글로벌 트랜스폼을 업데이트 합니다.
	globalTransform = mat * globalTransform;
	// 새로운 트렌스폼을 트랜스폼 스택에 넣습니다.
	transforms.push_back(mat);
	// 렌더 타겟 트랜스폼을 세팅합니다.
	renderTarget->SetTransform(ToD2DMat(globalTransform));
}

void D2DRenderer::PopTransform() {
	// 이전의 트랜스폼을 가져옵니다.
	Math::Matrix mat = transforms.back();
	// 트렌스폼의 역을 계산한 후 글로벌 트랜스폼에
	// 곱해서 이전 트랜스폼을 캔슬 합니다.
	globalTransform = mat.Invert() * globalTransform;
	// 이전 트렌스폼을 스택에서 팝합니다.
	transforms.pop_back();
	// 렌더 타겟 트랜스폼을 세팅합니다.
	renderTarget->SetTransform(ToD2DMat(globalTransform));
}

void D2DRenderer::ClearTransform() {
	globalTransform = Math::Matrix::Identity;
	transforms.clear();
}

Math::Matrix D2DRenderer::GetGlobalTransform() {
	return globalTransform;
}

void D2DRenderer::ResizeScreen(int w, int h) {
	renderTarget->Resize(D2D1::SizeU(w, h));
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
	GetClientRect(hWnd, &rc);

	// Create a Direct2D render target
	ID2D1HwndRenderTarget* renderTarget = nullptr;
	HRESULT hr = FactoryManager::GetGraphicsFactory()
		.CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(
				hWnd,
				D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top)
			),
			&renderTarget
		);

	// Reset the unique pointer
	this->renderTarget.reset(renderTarget);

	// Create a brush
	if (SUCCEEDED(hr)) {
		// Create a brush.
		renderTarget->CreateSolidColorBrush(
			D2D1::ColorF(D2D1::ColorF::White),
			&brush
		);
	}

	ID2D1DeviceContext* deviceContext;
	hr = renderTarget->QueryInterface(
		__uuidof(ID2D1DeviceContext),
		(void**)&deviceContext
	);

	dc.reset(deviceContext);
}

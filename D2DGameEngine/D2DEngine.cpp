#include "D2DEngine.h"

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dwrite.lib")

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include <dxgi1_4.h>

#include "FactoryManager.h"

D2DEngine::D2DEngine(HWND hWnd) :
	_hWnd{ hWnd },
	_renderTarget{ nullptr } {

	// Initialize Factory
	FactoryManager::Create();
	// Initialize the render target
	// Obtain the size of the drawing area.
	RECT rc;
	GetClientRect(_hWnd, &rc);

	// Create a Direct2D render target
	CreateRenderTarget();

	// Create a text format
	HRESULT res;
	IDWriteTextFormat* textFormat = nullptr;
	res = FactoryManager::GetDWriteFactory().CreateTextFormat(
		L"Arial",
		nullptr,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"",
		&textFormat
	);
	_textFormat.reset(textFormat);

	// Create the DXGI adapter
	IDXGIAdapter3* dxgiAdaptor = nullptr;
	res = FactoryManager::GetDXGIFactory4().EnumAdapters(
		0, reinterpret_cast<IDXGIAdapter**>(&dxgiAdaptor)
	);
	_DXGIAdapter.reset(dxgiAdaptor);
}

D2DEngine::~D2DEngine() {
	// Release the text format
	SafeRelease(_textFormat);

	// Release the DXGI adaptor
	SafeRelease(_DXGIAdapter);

	// Free D2D Objects and render target.
	SafeRelease(&_brush);
	SafeRelease(_renderTarget);

	// CoUninitialize
	FactoryManager::Destroy();
}

ID2D1HwndRenderTarget* D2DEngine::GetRenderTarget() {
	return _renderTarget.get();
}

void D2DEngine::BeginDraw() {
	_renderTarget->BeginDraw();
	_renderTarget->Clear();
	_renderTarget->SetTransform(D2D_Mat3x2F::Identity());
}

void D2DEngine::EndDraw() {
	_renderTarget->EndDraw();
}

void D2DEngine::DrawCircle(
	const D2D_Point2F& center, float rad,
	D2D1::ColorF color) {
	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);
	_renderTarget->DrawEllipse(
		{ {center.x, center.y}, rad, rad }, _brush
	);
	_brush->SetColor(tmp);
}

void D2DEngine::DrawBorder(
	const D2D_Point2F& ul, const D2D_Point2F& lr,
	D2D_Color color) {
	D2D_TColor tmp = _brush->GetColor();
	_brush->SetColor(color);
	_renderTarget->DrawRectangle(
		{ ul.x, ul.y, lr.x, lr.y }, _brush
	);
	_brush->SetColor(tmp);
}

void D2DEngine::DrawPolygon(
	const std::vector<D2D_Point2F>& points,
	D2D_Color color) {
	auto& d2d1Fac = FactoryManager::GetGraphicsFactory();

	ID2D1PathGeometry* path{ nullptr };
	auto res = d2d1Fac.CreatePathGeometry(&path);
	if (!SUCCEEDED(res)) return;

	ID2D1GeometrySink* sink{ nullptr };
	res = path->Open(&sink);
	if (!SUCCEEDED(res)) return;

	sink->BeginFigure(points[0],
		D2D1_FIGURE_BEGIN_HOLLOW);
	sink->AddLines(points.data(), points.size());
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
void D2DEngine::DrawString(
	const std::wstring& str, 
	const D2D_Point2F& ul, 
	const D2D_Point2F& lr) {
	_renderTarget->DrawText(
		str.c_str(),
		str.size(),
		_textFormat.get(),
		{ul.x, ul.y, lr.x, lr.y},
		_brush
	);
}

void D2DEngine::DrawSprite(
	D2D_Sprite* sprite,
	const D2D_Point2F& ul, const D2D_Point2F& lr
) {
	if (!sprite) return;
	_renderTarget->DrawBitmap(
		sprite,
		D2D1::RectF(ul.x, ul.y, lr.x, lr.y)
	);
}

void D2DEngine::DrawSprite(
	D2D_Sprite* sprite,
	const D2D_RectF& dst, const D2D_RectF& srcArea
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

void D2DEngine::PushTransform(const D2D_Mat3x2F& mat) {
	// 글로벌 트랜스폼을 업데이트 합니다.
	globalTransform = mat * globalTransform;
	// 새로운 트렌스폼을 트랜스폼 스택에 넣습니다.
	_transforms.push_back(mat);
	// 렌더 타겟 트랜스폼을 세팅합니다.
	_renderTarget->SetTransform(globalTransform);
}

void D2DEngine::PopTransform() {
	// 이전의 트랜스폼을 가져옵니다.
	D2D_Mat3x2F mat = _transforms.back();
	// 트렌스폼의 역을 계산한 후 글로벌 트랜스폼에
	// 곱해서 이전 트랜스폼을 캔슬 합니다.
	mat.Invert();
	globalTransform = mat * globalTransform;
	// 이전 트렌스폼을 스택에서 팝합니다.
	_transforms.pop_back();
	// 렌더 타겟 트랜스폼을 세팅합니다.
	_renderTarget->SetTransform(globalTransform);
}

void D2DEngine::ClearTransform() {
	globalTransform = D2D_Mat3x2F::Identity();
	_transforms.clear();
}

D2D_Mat3x2F D2DEngine::GetGlobalTransform() {
	return globalTransform;
}

void D2DEngine::ResizeScreen(int w, int h) {
	_renderTarget->Resize(D2D1::SizeU(w, h));
}

void D2DEngine::ShowVRAMUsage() {
	DXGI_QUERY_VIDEO_MEMORY_INFO videoMemoryInfo;
	_DXGIAdapter->QueryVideoMemoryInfo(0, DXGI_MEMORY_SEGMENT_GROUP_LOCAL, &videoMemoryInfo);
	float usage = videoMemoryInfo.CurrentUsage / 1024.f / 1024.f;

	std::wstring output = L"VRAM: " + std::to_wstring(usage) + L"MB";

	_renderTarget->DrawText(
		output.c_str(),
		output.size(),
		_textFormat.get(),
		D2D1::RectF(0.f, 0.f, 150.f, 24.f),
		_brush
	);
}

void D2DEngine::CreateRenderTarget() {
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

#include <d2d1_1.h>
#include <d2d1effectauthor.h>  
#include <d2d1effecthelpers.h>

// Example GUID used to uniquely identify the effect. It is passed to Direct2D during
// effect registration, and used by the developer to identify the effect for any
// ID2D1DeviceContext::CreateEffect calls in the app. The app should create
// a unique name for the effect, as well as a unique GUID using a generation tool.
DEFINE_GUID(CLSID_SampleEffect, 0xc482c72e, 0x1dd1, 0x4301, 0x98, 0xed, 0x50, 0xb7, 0x81, 0x64, 0x08, 0x0c);

class SampleEffect : public ID2D1EffectImpl
{
public:
	// 2.1 Declare ID2D1EffectImpl implementation methods.
	IFACEMETHODIMP Initialize(
		_In_ ID2D1EffectContext* pContextInternal,
		_In_ ID2D1TransformGraph* pTransformGraph
	);

	IFACEMETHODIMP PrepareForRender(D2D1_CHANGE_TYPE changeType);
	IFACEMETHODIMP SetGraph(_In_ ID2D1TransformGraph* pGraph);

	// 2.2 Declare effect registration methods.
	static HRESULT Register(_In_ ID2D1Factory1* pFactory);
	static HRESULT CreateEffect(_Outptr_ IUnknown** ppEffectImpl);

	// 2.3 Declare IUnknown implementation methods.
	IFACEMETHODIMP_(ULONG) AddRef();
	IFACEMETHODIMP_(ULONG) Release();
	IFACEMETHODIMP QueryInterface(_In_ REFIID riid, _Outptr_ void** ppOutput);

private:
	// Constructor should be private since it should never be called externally.
	SampleEffect();

	LONG m_refCount; // Internal ref count used by AddRef() and Release() methods.
};
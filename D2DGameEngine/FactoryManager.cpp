#include "FactoryManager.h"

#include <objbase.h>
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>
#include <comdef.h>
#include <dxgi1_4.h>

std::unique_ptr<FactoryManager> FactoryManager::_instance(nullptr);

void FactoryManager::CreateGraphicsFactory()
{
	HRESULT hr = D2D1CreateFactory(
		D2D1_FACTORY_TYPE_SINGLE_THREADED,
		&_D2D1Factory
	);
	if (!SUCCEEDED(hr))
		throw std::exception("Creating D2D1 factory failed!");
}

void FactoryManager::CreateDWriteFactory()
{
	// Create a DirectWrite factory.
	HRESULT hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(_DWriteFactory),
		reinterpret_cast<IUnknown**>(&_DWriteFactory)
	);

	if (!SUCCEEDED(hr))
		throw std::exception("Creating DirectWrite factory failed!");
}

void FactoryManager::CreateDXGIFactory4()
{
	HRESULT hr = CreateDXGIFactory1(
		__uuidof(IDXGIFactory4),
		(void**)&_DXGIFactory
	);
	if (!SUCCEEDED(hr))
		throw std::exception("Creating DXGI factory failed!");
}

void FactoryManager::CreateImagingFactory() {
	HRESULT hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&_IWICImagingFactory)
	);
	if (!SUCCEEDED(hr))
		throw std::exception("Creating WIC imaging factory failed!");
}

FactoryManager::FactoryManager()
{
	// COM initialize
	if (!SUCCEEDED(
		CoInitializeEx(
			NULL,
			COINIT_APARTMENTTHREADED
		)
	))
		throw std::exception("CoInitialize failed!");

	CreateGraphicsFactory();
	CreateDWriteFactory();
	CreateDXGIFactory4();
	CreateImagingFactory();
}

FactoryManager::~FactoryManager() {
	// Finalize the D2D1 factory
	SafeRelease(&_D2D1Factory);

	// Finalize the Direct Write factory
	SafeRelease(&_DWriteFactory);

	// Finalize the DXGI factory
	SafeRelease(&_DXGIFactory);

	// Finalize the imaging factory
	SafeRelease(&_IWICImagingFactory);

	// COM finalize
	CoUninitialize();
}

ID2D1Factory& FactoryManager::GetGraphicsFactory()
{
	return *_instance->_D2D1Factory;
}

IDWriteFactory& FactoryManager::GetDWriteFactory()
{
	return *_instance->_DWriteFactory;
}

IDXGIFactory4& FactoryManager::GetDXGIFactory4()
{
	return *_instance->_DXGIFactory;
}

IWICImagingFactory& FactoryManager::GetImagingFactory()
{
	return *_instance->_IWICImagingFactory;
}

FactoryManager& FactoryManager::Create() {
	if (_instance)
		throw std::runtime_error("Recreation of singleton detected!");

	_instance.reset(new FactoryManager());
	return *_instance;
}

void FactoryManager::Destroy() {
	if (!_instance)
		throw std::runtime_error("Destruction of uninitialized singleton detected!");

	_instance.release();
}

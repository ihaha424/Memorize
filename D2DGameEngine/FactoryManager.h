#pragma once

#include "framework.h"

struct ID2D1Factory;
struct IDWriteFactory;
struct IWICImagingFactory;
struct IDXGIFactory4;

class FactoryManager {
	ID2D1Factory* _D2D1Factory;
	IDWriteFactory* _DWriteFactory;
	IDXGIFactory4* _DXGIFactory;

	IWICImagingFactory* _IWICImagingFactory;

	static std::unique_ptr<FactoryManager> _instance;

	void CreateGraphicsFactory();
	void CreateDWriteFactory();
	void CreateDXGIFactory4();
	void CreateImagingFactory();


	FactoryManager(const FactoryManager&) = delete;
	FactoryManager(FactoryManager&&) noexcept = delete;
	FactoryManager& operator=(const FactoryManager&) = delete;
	FactoryManager& operator=(FactoryManager&&) noexcept = delete;

public:
	FactoryManager();
	~FactoryManager();

	static ID2D1Factory& GetGraphicsFactory();
	static IDWriteFactory& GetDWriteFactory();
	static IDXGIFactory4& GetDXGIFactory4();
	static IWICImagingFactory& GetImagingFactory();


	static FactoryManager& Create();
	static void Destroy();
};

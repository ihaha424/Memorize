#include "GameApp.h"

#include "D2DGameEngine/WinApp.h"

#include "D2DGameEngine/D2DRenderer.h"
#include "D2DGameEngine/World.h"
#include "MainLevel.h"
#include "TestLevel.h"
#include "LoadingLevel.h"

GameApp* loadedApp{ nullptr };

void GameApp::Initialize() {
	if (loadedApp) abort();

	// 윈도우 생성
	WinApp::App_Init();
	// TODO: 윈도우 타이틀이랑 스타일 추가할 것.
	WindowStyleFlags styleFlags = WS_OVERLAPPED;
	hWindow = WinApp::App_CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, styleFlags);

	ShowCursor(false);

	// GameEngine의 이니셜라이제이션
	Super::Initialize();

	loadedApp = this;

	world = new World;
	world->CreateLevel<LoadingLevel>(L"LoadingLevel");
	world->CreateLevel<MainLevel>(L"MainLevel");
	world->CreateLevel<TestLevel>(L"TestLevel");
	world->SetNextScene(L"LoadingLevel");
	world->ChangeScene();
	world->BeginPlay();

	// 그 외 이니셜라이제이션 스테이지..
	//
	//
	// ..

	// 초기화 True
	isInitialized = true;
}

void GameApp::Execute() {
	// 게임 루프 실행
	Super::Execute();
}

void GameApp::Shutdown() {
	if (isInitialized) {
		// 그 외 파이널라이제이션 스테이지..
		//
		// 
		// ..

		
		// 게임 엔진 셧다운
		Super::Shutdown();

		// 윈도우 파괴
		WinApp::App_Destroy();
	}

	loadedApp = nullptr;
}


void GameApp::FixedUpdate(float dt) {
	static float elapsedTime{ 0.f };
	elapsedTime += dt;
	world->FixedUpdate(dt);
}

void GameApp::Update(float dt) {
	static float elapsedTime{ 0.f };
	elapsedTime += dt;

#ifndef NDEBUG 
	frameTime += dt;
#endif
	world->BeginPlay();
	world->PreUpdate(dt);
	world->Update(dt);
	world->PostUpdate(dt);

}

void GameApp::Render() {
#ifndef NDEBUG 
	// FPS 계산
	if (frameTime >= 1.0f) {
		frameTime -= 1.0f;
		fps = count;
		count = 0;
	}

	std::wstring windowText;
	windowText.append(L"  FPS: ");
	windowText.append(std::to_wstring(fps));
	SetWindowText(hWindow, windowText.c_str());
#endif
	// 드로우 시작
	d2d->BeginDraw();


	world->Render(d2d);
	// DEBUG: D2D 엔진 디버그 용
	// d2d->ShowVRAMUsage();
	// d2d->DrawCircle({ SCREEN_WIDTH / 2.f, SCREEN_HEIGHT / 2.f }, 200.f);

	// 드로우 끝
	d2d->EndDraw();
#ifndef NDEBUG 
	++count;
#endif
}

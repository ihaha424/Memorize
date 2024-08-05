﻿#include "GameApp.h"

#include "D2DGameEngine/WinApp.h"

GameApp* loadedApp{ nullptr };

void GameApp::Initialize() {
	if (loadedApp) abort();

	// 윈도우 생성
	WinApp::App_Init();
	// TODO: 윈도우 타이틀이랑 스타일 추가할 것.
	WindowStyleFlags styleFlags = WS_OVERLAPPED;
	hWindow = WinApp::App_CreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, TITLE, styleFlags);

	// GameEngine의 이니셜라이제이션
	Super::Initialize();

	loadedApp = this;

	

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

}

void GameApp::Update(float dt) {
	static float elapsedTime{ 0.f };
	elapsedTime += dt;

#ifndef NDEBUG 
	frameTime += dt;
#endif


}

void GameApp::Render() {
#ifndef NDEBUG 
	// FPS 계산
	if (frameTime >= 1.0f) {
		frameTime -= 1.0f;
		fps = count;
		count = 0;
	}
#endif

#ifndef NDEBUG 
	++count;
#endif
}
#include "GameEngine.h"

#include "D2DEngine.h"

#include "Timer.h"
#include <chrono>
#include <thread>

constexpr double FIXED_RATE = 1.0 / 60.0;

void GameEngine::Initialize() 
{
	// NOTE: 엔진 초기화 작업
	// e.g., 리소스 매니저, 팩토리 싱글톤 등...
	_d2d = new D2DEngine(hWindow);
}

void GameEngine::Execute() {
	Run();
}

void GameEngine::Shutdown() 
{
	delete _d2d;
	// NOTE: 엔진 종료 작업
	// e.g., 메모리 삭제, 싱글톤 셧다운 등...
}

void GameEngine::Run() 
{
	// 타이머 셋업
	Timer::InitTimer();
	double prevTime = Timer::GetTick();	// 이전 프레임 타임
	double currTime = 0.0;	// 현재 프레임 타임
	double frameTime = 0.0;	// 델타 타임
	double frameTimeThreshole = 0.25;	// 최대 델타 타임
	double fixedRate = FIXED_RATE;	// 픽스 타임
	double accumulator = 0.0;	// 픽스 타임 트레킹

	MSG msg = { };
	bool bQuit = false;

	// Main loop
	while (!bQuit) {
		// Handle messages on the queue
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			// The window received the destruction call
			if (msg.message == WM_QUIT) {
				bQuit = true;
			}

			// When the window is minimized, stop rendering
			if (msg.message == WM_SHOWWINDOW) {
				if (msg.lParam == SW_PARENTCLOSING)
					stop_rendering = true;
				if (msg.lParam == SW_PARENTOPENING)
					stop_rendering = false;
			}

			// Translate and dispatch message
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 프레임 델타 타임 업데이트
		currTime = Timer::GetTick();
		frameTime = currTime - prevTime;	// 델타 타임
		frameTime *= Timer::GetTimeScale();	// 스케일 된 델타 타임
		prevTime = currTime;

		// 게임 루프 소용돌이를 방어하기 위해 델타타임이 일정시간 이상 넘어가면 
		// 픽스 합니다.
		frameTimeThreshole = 0.25 * Timer::GetTimeScale();
		if (frameTime > frameTimeThreshole) frameTime = frameTimeThreshole;	// Clamp the frame time

		// 프레임 타임을 누적해서 픽스 타임을 추적합니다.
		accumulator += frameTime;
		// 일정 시간마다 픽스 업데이트를 실행합니다.
		fixedRate = FIXED_RATE * Timer::GetTimeScale();	// 스케일 된 픽스 타임
		while (accumulator >= fixedRate) {
			FixedUpdate(fixedRate);	// Update physics at a fixed rate
			accumulator -= fixedRate;
		}

		// 프레임 타임 업데이트를 실햅니다.
		Update(frameTime);

		// 현재 프레임을 렌더합니다.
		//// 최소화된 창은 렌더하지 않습니다.
		if (stop_rendering) {
			// Throttle the speed to avoid the endless spinning
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			continue;
		}

		// NOTE: 렌더할 때 물리 애니메이션들의 어색함을 줄이려면 
		// 픽스 타임을 위한 인터폴레이션, 보간이 필요할 수 있음.
		// const double alpha = accumulator / fixedRate;
		// interpolate(alpha);
		Render();
	}
}

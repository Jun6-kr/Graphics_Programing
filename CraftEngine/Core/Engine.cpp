#include "Engine.h"
#include <Core/Win32Window.h>
#include <Graphics/Renderer.h>
#include <cassert>

#if _DEBUG
#include <iostream>
#endif

namespace Craft
{
	Engine::Engine(uint32_t width, uint_fast32_t height, const std::wstring title)
	{
        // 전역 접근 가능하도록 인스턴스 설정
        assert(!instance);
        instance = this;

		// 창 객체 생성
		window = std::make_unique<Win32Window>(width, height, this, title);

        // 렌더러 객체 생성
        renderer = std::make_unique<Renderer>(*window);
	}

    Engine::~Engine()
    {
        instance = nullptr;
    }

	void Engine::Run()
	{
        // 고해상도 타이머 주파수 가져오기
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);

        // 델타 타임 구하는 람다 함수
        auto GetDeltaTime = [&frequency](int64_t& current, int64_t& previous) -> float
        {
            // 현재 시간 가져오기
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);
            current = counter.QuadPart;

            return static_cast<float>(current - previous) / static_cast<float>(frequency.QuadPart);
        };

        // 프레임 계산을 위한 변수
        int64_t current = 0;
        int64_t previous = 0;

        // 고정 프레임 처리를 위한 값
        const float framerate = 120.f;
        const float oneFrameTime = 1.0f / framerate;

        // 스레드 재우는 간격 설정
        timeBeginPeriod(1);

        // 이벤트(window 메시지) 처리 루프
        MSG message = {};
        while (!isQuit && message.message != WM_QUIT)
        {
            // 창에 메시지가 발생한 경우 처리 - min/max로 메시지의 종류를 필터링 -> 0/0은 모든 메시지 수신
            if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
            {
                TranslateMessage(&message);     // 
                DispatchMessage(&message);      // 특정 함수를 호출하도록 메시지 전달
            }
            // 엔진 루프 처리
            else
            {
                // 프레임 시간 구하고 남은 시간 계산
                float deltaTime = GetDeltaTime(current, previous);
                float remainingTime = oneFrameTime - deltaTime;

                //// 스레드 재우기
                //while (remainingTime >= 0.002f)
                //{
                //    // Sleep에서 0 넣을 때와 1 넣을 때 차이점
                //    Sleep(1);
                //
                //    // 프레임 시간 구하기
                //    deltaTime = GetDeltaTime(current, previous);
                //    // 남은 시간 구하기
                //    remainingTime  = oneFrameTime - deltaTime;
                //}

                while (remainingTime > 0.0f)
                {
                    deltaTime = GetDeltaTime(current, previous);
                    remainingTime  = oneFrameTime - deltaTime;
                }

#if _DEBUG
                std::cout << "deltTime : " << deltaTime
                    << " | FPS : " << (1.0f / deltaTime)
                    << "\n";
#endif

                BeginPlay();
                Tick(deltaTime);
                Draw();

                // 레벨 전환 처리
                if (nextLevel)
                {
                    if (mainLevel)
                    {
                        mainLevel.reset();
                    }

                    mainLevel = nextLevel;
                    nextLevel.reset();

                    // 레벨 초기화 함수 호출
                    mainLevel->Initialized();
                }
                
                // 액터 추가/삭제 처리
                if (mainLevel)
                {
                    mainLevel->ProcessAddAndDestroyActors();
                }

                // 이전 시간 기록
                previous = current;
            }
        }

        // 스레드 간격 복구
        timeEndPeriod(1);
	}
	void Engine::Quit()
	{
        // 종료 플래그 설정
        isQuit = true;
	}

    Engine& Engine::Get()
    {
        assert(instance);
        return *instance;
    }

    void Engine::BeginPlay()
    {
        if (mainLevel)
        {
            mainLevel->BeginPlay();
        }
    }

    void Engine::Tick(float deltaTime)
    {
        if (mainLevel)
        {
            mainLevel->Tick(deltaTime);
        }
    }

    void Engine::Draw()
    {
        // 레벨의 draw가 먼저 처리되어야 한다
        if (mainLevel)
        {
            mainLevel->Draw();
        }

        // 이벤트 전달
        if (renderer)
        {
            renderer->Draw(0.7f, 0.3f, 0.12f, 0);
        }
    }

    void Engine::OnResize(uint32_t width, uint32_t height)
    {
        if (renderer)
        {
            renderer->OnResize(width, height);
        }

        if (window)
        {
            window->OnResize(width, height);
        }
    }

    LRESULT Engine::HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
    {
        // 메시지 처리
        switch (message)
        {
        case WM_CLOSE:
        {
            // 창 닫기 -> 창 객체 삭제
            DestroyWindow(window);
        }
        return 0;

        // 창 삭제 이벤트 처리
        case WM_DESTROY:
        {
            // 프로그램 종료 요청 (종료 메시지 발행)
            PostQuitMessage(0);
        }
        return 0;

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(window, &ps);

            // All painting occurs here, between BeginPaint and EndPaint.

            FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

            EndPaint(window, &ps);
        }
        return 0;

        // 창 크기 메시지 처리
        case WM_SIZE:
        {
            // 변겨왼 너비/높이 구하기
            uint32_t width = LOWORD(lparam);
            uint32_t height = HIWORD(lparam);

            // 메시지 전파
            OnResize(width, height);
        }
        return 0;
        // ESC 키 입력 처리
        case WM_KEYDOWN:
        {
            // 눌린 키가 ESC키 인지 확인
            if (wparam == VK_ESCAPE)
            {
                DestroyWindow(window);
            }
        }

        }

        return DefWindowProc(window, message, wparam, lparam);
    }
}
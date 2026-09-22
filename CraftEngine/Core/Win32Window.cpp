#include "Win32Window.h"
#include <Interface/IMessageHandler.h>

namespace Craft
{
	Win32Window::Win32Window(uint32_t width, uint32_t height, IMessageHandler* messageHandler, const std::wstring title) : width(width), height(height), title(title), messageHandler(messageHandler), instance(GetModuleHandle(nullptr))
	{
        // 창 만들기

        // 창 만들 때 사용되는 정보 구조체
        WNDCLASS wc = { };
        wc.lpfnWndProc = Win32MessageHandler;
        wc.hInstance = instance;
        wc.lpszClassName = className.c_str();
        wc.style = CS_HREDRAW | CS_VREDRAW;     // 다시 그리기 모드?

        // 클래스 등록
        if (!RegisterClass(&wc))
        {
            // 디버그 모드일 떄 강제 중단해주는 함수
            __debugbreak();
            return;
        }

        // 창 크기 조정
        RECT rect = { 0, 0, (long)width, (long)height };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

        // 창 크기 계산
        uint32_t windowWidth = rect.right - rect.left;
        uint32_t windowHeight = rect.bottom - rect.top;

        // 화면 가운데 창 생성하도록 위치 구하기
        uint32_t x = (GetSystemMetrics(SM_CXSCREEN) - windowWidth) / 2;
        uint32_t y = (GetSystemMetrics(SM_CYSCREEN) - windowHeight) / 2;

        // Create the window.
        handle = CreateWindow(
            className.c_str(),                     // Window class
            title.c_str(),    // Window text
            WS_OVERLAPPEDWINDOW,            // Window style

            // Position / Size
            x, y, windowWidth, windowHeight,

            nullptr,        // Parent window    
            nullptr,        // Menu
            instance,       // Instance handle
            this            // Additional application data
        );

        if (handle == nullptr)
        {
            __debugbreak();
            return;
        }

        ShowWindow(handle, SW_SHOW);
	}

	Win32Window::~Win32Window()
	{
        UnregisterClass(className.c_str(), instance);
	}

	LRESULT CALLBACK Win32Window::Win32MessageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
	{
        if (message == WM_CREATE)
        {
            // 윈도우 파라미터 설정
            // 샐성할 대 념겨줬던 추가 파리미터 가져오기
            CREATESTRUCT*  createStruct = reinterpret_cast<CREATESTRUCT*>(lparam);
            if (createStruct)
            {
                Win32Window* win32Window = reinterpret_cast<Win32Window*>(createStruct->lpCreateParams);

                if (win32Window && win32Window->messageHandler)
                {
                    SetWindowLongPtr(window, GWLP_USERDATA, (LONG_PTR)win32Window);
                }
            }

            return 0;
        }

        // 생성 이후의 이벤트 처리
        Win32Window* win32Window = reinterpret_cast<Win32Window*>(GetWindowLongPtr(window, GWLP_USERDATA));
        if (win32Window && win32Window->messageHandler)
        {
            return win32Window->messageHandler->HandleMessage(window, message, wparam, lparam);
        }

        // 방어 코드
        return DefWindowProc(window, message, wparam, lparam);
	}
}
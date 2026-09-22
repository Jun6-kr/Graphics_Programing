#pragma once

#include <Interface/IMessageHandler.h>
#include <memory>
#include <cstdint>
#include <string>

namespace Craft
{
	// 전방 선언
	class Win32Window;
	class Renderer;

	class Engine : public IMessageHandler
	{
	public:
		Engine(uint32_t width = 1280, uint_fast32_t height = 800, const std::wstring title = L"Craft Render Engine");
		virtual ~Engine();

		// 엔진 루프 실행 함수
		void Run();

		// 엔진 종료 함수
		void Quit();


	protected:
		void Draw();

		virtual LRESULT HandleMessage(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

	protected:
		// 창 객체
		std::unique_ptr<Win32Window> window;

		// 렌더러 객체
		std::unique_ptr<Renderer> renderer;
	};
}
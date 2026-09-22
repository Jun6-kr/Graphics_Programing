#pragma once

#include <Windows.h>
#include <string>
#include <cstdint>

namespace Craft
{
	class IMessageHandler;

	class Win32Window
	{
	public:
		Win32Window(uint32_t width = 1280, uint32_t height = 800, IMessageHandler* messageHandler = nullptr, const std::wstring title = L"Craft Render Engine");
		~Win32Window();

		// Getter
		inline uint32_t GetWidth() const { return width; }
		inline uint32_t GetHeight() const { return height; }
		inline HWND GetHandle() const { return handle; }

	private:
		// 창 메시지 처리 함수
		static LRESULT CALLBACK Win32MessageHandler(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

	private:
		// 프로그램 인스턴스 - Windows가 '프로그램'을 식별하기 위한 핸들
		HINSTANCE instance = nullptr;

		// 창 객체 핸들 - Windows가 어떤 "창"인지를 식별하기 위한 핸들
		HWND handle = nullptr;

		// 창 크기
		uint32_t width = 0;
		uint32_t height = 0;

		// 클래스 이름 / 타이틀 이름
		std::wstring className = L"Craft_Render_Widnow_Class";
		std::wstring title;

		// 메시지 핸들러 객체
		IMessageHandler* messageHandler = nullptr;
	};
}



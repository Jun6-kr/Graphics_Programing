#pragma once

#include <Core/Core.h>
#include <cstdint>

namespace Craft
{
	// 키입력/마우스 입력을 처리하는 클래스
	// 프레임 단위로 입력 변경 상태 처리
	class Input : public TGlobalAccessible<Input>
	{
		// 키입력 상태 구조체
		struct KeyState
		{
			// 현재 프레임에 처음 눌렸는지 여부
			bool isKeyDown = false;

			// 현재 프레임에 처음 눌림이 해제됐는지 여부
			bool isKeyUp = false;

			// 현재 눌림 상태인지
			bool isKey = false;
		};

	public:
		Input() = default;
		~Input() = default;

		// 액터나 컴포넌트에서 입력을 확인할 떄 사용할 함수
		bool GetKeyDown(uint32_t keyCode) const;
		bool GetKeyUp(uint32_t keyCode) const;
		bool GetKey(uint32_t keyCode) const;

		// win32 메시지를 키 상태로 변환하는 함수
		void OnKeyDown(uint32_t keyCode);
		void OnKeyUp(uint32_t keyCode);

		// 키입력 상태 전체를 초기화하는 함수
		// -> 창이 포커스를 잃었을 때 처리를 위해
		void ResetAll();

		// 프레임이 종료될 때 초기화 처리
		void ResetInputs();

	private:
		// 키코드가 유효한지 확인하는 함수
		bool IsValidKeyCode(uint32_t keyCode) const;

	private:
		// 윈도우에 정의된 가상 키코드 개수 256개
		static const uint32_t keyStateCount = 256;

		// 키입력 상태를 관리할 배열
		KeyState keyStates[keyStateCount] = {};
	};
}


#include "Input.h"

namespace Craft
{
	bool Input::GetKeyDown(uint32_t keyCode) const
	{
		return IsValidKeyCode(keyCode) && keyStates[keyCode].isKeyDown;
	}

	bool Input::GetKeyUp(uint32_t keyCode) const
	{
		return IsValidKeyCode(keyCode) && keyStates[keyCode].isKeyUp;
	}

	bool Input::GetKey(uint32_t keyCode) const
	{
		return IsValidKeyCode(keyCode) && keyStates[keyCode].isKey;
	}

	bool Input::GetMouseButtonDown(uint32_t button) const
	{
		return IsValidMouseButton(button) && mouseButtonStates[button].isButtonDown;
	}

	bool Input::GetMouseButtonUp(uint32_t button) const
	{
		return IsValidMouseButton(button) && mouseButtonStates[button].isButtonUp;
	}

	bool Input::GetMouseButton(uint32_t button) const
	{
		return IsValidMouseButton(button) && mouseButtonStates[button].isButton;
	}

	void Input::OnKeyDown(uint32_t keyCode)
	{
		// 키코드 값이 유효한지 확인
		if (!IsValidKeyCode(keyCode))
		{
			return;
		}

		KeyState& state = keyStates[keyCode];

		// 눌림이 해제된 상황에서 처음 눌림 상태로 변경된 경우에만 down 기록
		if (!state.isKey)
		{
			state.isKeyDown = true;
		}

		state.isKey = true;
	}

	void Input::OnKeyUp(uint32_t keyCode)
	{
		// 키코드 값이 유효한지 확인
		if (!IsValidKeyCode(keyCode))
		{
			return;
		}

		KeyState& state = keyStates[keyCode];

		// 키가 눌린 상태에서 처음 해제될 때 up 설정
		if (state.isKey)
		{
			state.isKeyUp = true;
		}

		state.isKey = false;
	}

	void Input::OnMouseButtonDown(uint32_t button)
	{
		// 코드가 유효한지 검사
		if (!IsValidMouseButton(button))
		{
			return;
		}

		// 버튼 인덱스에 해당하는 상태 가져오기
		MouseButtonState& state = mouseButtonStates[button];

		// 눌림이 해제된 상태에서만 눌림 기록
		if (!state.isButton)
		{
			state.isButtonDown = true;
		}

		// 눌림 상태로 기록
		state.isButton = true;
	}

	void Input::OnMouseButtonUp(uint32_t button)
	{
		// 코드가 유효한지 검사
		if (!IsValidMouseButton(button))
		{
			return;
		}

		// 버튼 인덱스에 해당하는 상태 가져오기
		MouseButtonState& state = mouseButtonStates[button];

		if (state.isButton)
		{
			state.isButtonUp = true;
		}

		state.isButton = false;
	}

	void Input::SetMousePosition(int x, int y)
	{
		// 전달된 값을 마우스 위치로 설정
		mousePosition = Vector2(static_cast<float>(x), static_cast<float>(y));
	}

	void Input::ResetAll()
	{
		// 모든 키 입력 초기화
		for (KeyState& state : keyStates)
		{
			state.isKeyDown = false;
			state.isKeyUp = false;
			state.isKey = false;
		}

		// 마우스 입력 상태 초기화
		for (MouseButtonState& state : mouseButtonStates)
		{
			state.isButtonDown = false;
			state.isButtonUp = false;
			state.isButton = false;
		}

		// 마우스 위치 초기화
		mousePreviousPosition = mousePosition;
	}

	void Input::ResetInputs()
	{
		for (KeyState& state : keyStates)
		{
			state.isKeyDown = false;
			state.isKeyUp = false;
		}

		// 마우스 입력 상태 초기화
		for (MouseButtonState& state : mouseButtonStates)
		{
			state.isButtonDown = false;
			state.isButtonUp = false;
		}

		// 마우스 위치 초기화
		mousePreviousPosition = mousePosition;
	}

	bool Input::IsValidKeyCode(uint32_t keyCode) const
	{
		return keyCode < keyStateCount;
	}

	bool Input::IsValidMouseButton(uint32_t button) const
	{
		return button < mouseButtonCount;
	}
}
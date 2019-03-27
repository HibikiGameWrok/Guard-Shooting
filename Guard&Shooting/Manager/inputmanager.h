#pragma once


#include <Mouse.h>
#include <Keyboard.h>

#include "../Utility\SingletonBase.h"

namespace System
{
	class InputManager : public Utility::SingletonBase<InputManager>
	{
	public:

		friend Utility::SingletonBase<InputManager>;

	private:

		InputManager();

	public:
		// �}�E�X�̏�Ԏ擾
		DirectX::Mouse::State GetMouseState();

		// Tracker�̎擾
		DirectX::Mouse::ButtonStateTracker GetMouseTracker();

		// �}�E�X���W�̎擾
		int GetMousePosX()
		{
			return GetMouseState().x;
		}
		int GetMousePosY()
		{
			return GetMouseState().y;
		}


		// �L�[�{�[�h�̏�Ԏ擾
		DirectX::Keyboard::State GetKeyState();

		// Tracker�̎擾
		DirectX::Keyboard::KeyboardStateTracker GetKeyTracker();


		// �X�V
		void Update();

	private:
		// �}�E�X
		std::unique_ptr<DirectX::Mouse> mpMouse;
		// Tracker
		DirectX::Mouse::ButtonStateTracker mMTracker;


		// �L�[�{�[�h
		std::unique_ptr<DirectX::Keyboard> mpKey;
		// Tracker
		DirectX::Keyboard::KeyboardStateTracker mKTracker;
	};
}
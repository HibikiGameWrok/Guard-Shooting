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
		// マウスの状態取得
		DirectX::Mouse::State GetMouseState();

		// Trackerの取得
		DirectX::Mouse::ButtonStateTracker GetMouseTracker();

		// マウス座標の取得
		int GetMousePosX()
		{
			return GetMouseState().x;
		}
		int GetMousePosY()
		{
			return GetMouseState().y;
		}


		// キーボードの状態取得
		DirectX::Keyboard::State GetKeyState();

		// Trackerの取得
		DirectX::Keyboard::KeyboardStateTracker GetKeyTracker();


		// 更新
		void Update();

	private:
		// マウス
		std::unique_ptr<DirectX::Mouse> mpMouse;
		// Tracker
		DirectX::Mouse::ButtonStateTracker mMTracker;


		// キーボード
		std::unique_ptr<DirectX::Keyboard> mpKey;
		// Tracker
		DirectX::Keyboard::KeyboardStateTracker mKTracker;
	};
}
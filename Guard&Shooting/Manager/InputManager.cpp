#include "../pch.h"
#include "../Manager/InputManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
System::InputManager::InputManager()
{
	mpMouse = std::make_unique<DirectX::Mouse>();
	mpKey = std::make_unique<DirectX::Keyboard>();
}

/// <summary>
/// マウスの入力状態の取得
/// </summary>
/// <returns>入力状態</returns>
DirectX::Mouse::State System::InputManager::GetMouseState()
{
	return mpMouse->GetState();
}

/// <summary>
/// Trackerの取得
/// </summary>
/// <returns>Tracker</returns>
DirectX::Mouse::ButtonStateTracker System::InputManager::GetMouseTracker()
{
	return mMTracker;
}


DirectX::Keyboard::State System::InputManager::GetKeyState()
{
	return mpKey->GetState();
}

DirectX::Keyboard::KeyboardStateTracker System::InputManager::GetKeyTracker()
{
	return mKTracker;
}

/// <summary>
/// 更新処理
/// </summary>
void System::InputManager::Update()
{
	DirectX::Mouse::State Mouse_state = GetMouseState();
	mMTracker.Update(Mouse_state);
	DirectX::Keyboard::State Key_state = GetKeyState();
	mKTracker.Update(Key_state);
}
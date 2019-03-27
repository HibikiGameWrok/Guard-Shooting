//
//	File	 : Keyboard_Control.h
//
//  Contents : 操作に応じて値を渡す基底クラス
//
//	name	 : Hibiki Yoshiyasu
//
#pragma once
#include "Control.h"
#include "../Utility/SingletonBase.h"

namespace System
{
	class Key_Control : public Control, public Utility::SingletonBase<Key_Control>
	{
	public:
		friend class Utility::SingletonBase<Key_Control>;

		enum Action {
			NONE,		// 無し

			FORWARD,	// 前移動
			BACK,		// 後ろ移動
			
			RIGHT,		// 右移動
			LEFT,		// 左移動
			
			RIGHT_FOR,	// 右斜め前
			RIGHT_BACK,	// 右斜め後ろ
			
			LEFT_FOR,	// 右斜め前
			LEFT_BACK,	// 右斜め後ろ
			
			SHOT,		// 発射
			
			ACTION_NUM
		};
	private:// メンバ変数
		// フラグ変数
		bool m_dashFlag;	// ダッシュフラグ
		bool m_shotFlag;	// 発射フラグ

		// キーボード
		std::unique_ptr<DirectX::Keyboard> m_keyboard;
		// キーボードトラッカー
		DirectX::Keyboard::KeyboardStateTracker m_tracker;
	public:// コンストラクタ&デストラクタ
		Key_Control();
		~Key_Control() {};

		void Update() override;
	public:
		void SetAction(Action action) { m_actionVal = action; }

		// フラグの設定と取得
		void SetDashFlag(bool flag) { m_dashFlag = flag; }
		bool GetDashFlag() { return m_dashFlag; }
		
		void SetShotFlag(bool flag) { m_shotFlag = flag; }
		bool GetShotFlag() { return m_shotFlag; }
	};
}
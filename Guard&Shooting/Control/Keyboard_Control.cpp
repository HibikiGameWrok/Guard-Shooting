//
//	File	 : Keyboard_Control.cpp
//
//  Contents : 操作に応じて値を渡す基底クラスの関数定義
//
//	name	 : Hibiki Yoshiyasu
//
#include "../pch.h"
#include "Keyboard_Control.h"

using namespace System;
using namespace DirectX;

/// <summary>
/// コンストラクタ
/// </summary>
Key_Control::Key_Control()
{
}

/// <summary>
/// 更新処理
/// </summary>
void Key_Control::Update()
{
	// キー入力
	Keyboard::State kb = Keyboard::Get().GetState();
	// トリガー
	m_tracker.Update(kb);

	// 前移動
	if ((kb.Up) || (kb.I))
	{
		SetAction(Action::FORWARD);
	}
	// 後ろ移動
	if ((kb.Down) || (kb.K))
	{
		SetAction(Action::BACK);
	}

	// 右回転
	if ((kb.Right) || (kb.L))
	{
		SetAction(Action::RIGHT);
	}
	// 左回転
	if ((kb.Left) || (kb.J))
	{
		SetAction(Action::LEFT);
	}

	// 右斜め移動
	if ((kb.Right && kb.Up) || (kb.L && kb.I))
	{
		SetAction(Action::RIGHT_FOR);
	}
	if ((kb.Right && kb.Down) || (kb.L && kb.K))
	{
		SetAction(Action::RIGHT_BACK);
	}

	// 左斜め移動
	if ((kb.Left && kb.Up) || (kb.J && kb.I))
	{
		SetAction(Action::LEFT_FOR);
	}
	if ((kb.Left && kb.Down) || (kb.J && kb.K))
	{
		SetAction(Action::LEFT_BACK);
	}

	// ダッシュフラグをtrue
	if ((kb.Q)	&& 
			  ((kb.Up)
			|| (kb.I)
			|| (kb.Down)
			|| (kb.K)
			|| (kb.Right && kb.Up)
			|| (kb.L && kb.I)
			|| (kb.Left && kb.Down)
			|| (kb.J && kb.K))
		)
	{
		m_dashFlag = true;
	}
	else
	{
		m_dashFlag = false;
	}	

	// 発射
	if (m_tracker.pressed.E)
	{
		m_shotFlag = true;
	}
	else
	{
		m_shotFlag = false;
	}
}


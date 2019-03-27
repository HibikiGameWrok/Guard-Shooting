//
//	File	 : Keyboard_Control.cpp
//
//  Contents : ����ɉ����Ēl��n�����N���X�̊֐���`
//
//	name	 : Hibiki Yoshiyasu
//
#include "../pch.h"
#include "Keyboard_Control.h"

using namespace System;
using namespace DirectX;

/// <summary>
/// �R���X�g���N�^
/// </summary>
Key_Control::Key_Control()
{
}

/// <summary>
/// �X�V����
/// </summary>
void Key_Control::Update()
{
	// �L�[����
	Keyboard::State kb = Keyboard::Get().GetState();
	// �g���K�[
	m_tracker.Update(kb);

	// �O�ړ�
	if ((kb.Up) || (kb.I))
	{
		SetAction(Action::FORWARD);
	}
	// ���ړ�
	if ((kb.Down) || (kb.K))
	{
		SetAction(Action::BACK);
	}

	// �E��]
	if ((kb.Right) || (kb.L))
	{
		SetAction(Action::RIGHT);
	}
	// ����]
	if ((kb.Left) || (kb.J))
	{
		SetAction(Action::LEFT);
	}

	// �E�΂߈ړ�
	if ((kb.Right && kb.Up) || (kb.L && kb.I))
	{
		SetAction(Action::RIGHT_FOR);
	}
	if ((kb.Right && kb.Down) || (kb.L && kb.K))
	{
		SetAction(Action::RIGHT_BACK);
	}

	// ���΂߈ړ�
	if ((kb.Left && kb.Up) || (kb.J && kb.I))
	{
		SetAction(Action::LEFT_FOR);
	}
	if ((kb.Left && kb.Down) || (kb.J && kb.K))
	{
		SetAction(Action::LEFT_BACK);
	}

	// �_�b�V���t���O��true
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

	// ����
	if (m_tracker.pressed.E)
	{
		m_shotFlag = true;
	}
	else
	{
		m_shotFlag = false;
	}
}


//
//	File: CollisionCapsule.cpp
//
// �@�J�v�Z����������Obje3D�N���X
//
//	name: Hibiki Yoshiyasu
#include "../../pch.h"
#include "CollisionCapsule.h"
#include "../../Game.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

//void CollisionCapsule::SetCollision(DX::DeviceResources * deviceResources, Collision::Capsule capsule)
//{
//	// �f�o�C�X�͈�񂵂��g��Ȃ��̂Ń��[�J���ϐ�
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	// ���E���̏��ݒ�
//	m_collision = capsule;
//
//	// �f�o�b�N�p���E�����f���̍쐬
//	m_debugObj =
//		make_unique<DebugCapsule>(
//			device,
//			m_collision.a,		// ���ԕ��̐����̊J�n�n�_
//			m_collision.b,		// ���ԕ��̐����̏I���n�_
//			m_collision.r		// �����̔��a
//			);
//}

void CollisionCapsule::SetCollision(Collision::Capsule capsule)
{
	// ���E���̏��ݒ�
	m_collision = capsule;
}

Collision::Capsule CollisionCapsule::GetCollision()
{
	Collision::Capsule capsule;

	// ���E���̒��S���W�����[���h�s��ɂ����W�ϊ�����
	capsule.a = Vector3::Transform(m_collision.a, m_world);
	capsule.b = Vector3::Transform(m_collision.b, m_world);
	capsule.r = m_collision.r;

	// ���[�J���ϐ�sphere��Ԃ�
	return capsule;
}

void CollisionCapsule::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	// �f�o�b�N�p���E���̕`��
	//m_debugObj->Draw(
	//	m_context,
	//	*states,
	//	m_world,
	//	view,
	//	projection
	//);
}

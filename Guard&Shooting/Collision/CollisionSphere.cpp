//
//	File: CollisionSphere.cpp
//
// �@���E����������Obje3D�N���X
//
//	name: Hibiki Yoshiyasu
#include "../../pch.h"
#include "CollisionSphere.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

//void CollisionSphere::SetCollision(DX::DeviceResources * deviceResources,Collision::Sphere sphere)
//{
//	// �f�o�C�X�͈�񂵂��g��Ȃ��̂Ń��[�J���ϐ�
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//	// ���E���̏��ݒ�
//	m_collision = sphere;
//
//	// �f�o�b�N�p���E�����f���̍쐬
//	m_debugObj = make_unique<DebugSphere>(
//		device,
//		m_collision.c,		// ���E���̒��S
//		m_collision.r		// ���E���̔��a
//		);
//}

void CollisionSphere::SetCollision(Collision::Sphere sphere)
{
	// ���E�����̏��ݒ�
	m_collision = sphere;
}

Collision::Sphere CollisionSphere::GetCollision()
{
	Collision::Sphere sphere;

	// ���E���̒��S���W�����[���h�s��ɂ����W�ϊ�����
	sphere.c = Vector3::Transform(m_collision.c,m_world);
	sphere.r = m_collision.r;

	return sphere;	// ���[�J���ϐ�sphere��Ԃ�
}

void CollisionSphere::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
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

#include "../../pch.h"
#include "CollisionBox.h"
#include "../../Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
///  �����蔻��̎󂯎��
/// </summary>
/// <param name="box">Box�̓����蔻��͈�</param>
//void CollisionBox::SetCollision(DX::DeviceResources * deviceResources,Collision::Box box)
//{
//	// �f�o�C�X�͈�񂵂��g��Ȃ��̂Ń��[�J���ϐ�
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	// ���E���̏��ݒ�
//	m_collision = box;
//	
//	// �f�o�b�N�p���E�����f���̍쐬
//	m_debugObj = make_unique<DebugBox>(
//		device,
//		m_collision.c,		// ���E���̒��S
//		m_collision.r		// ���E���̔��a
//		);
//}

void CollisionBox::SetCollision(Collision::Box box)
{
	// ���E���l�p�`�̏��ݒ�
	m_collision = box;
}

/// <summary>
///  �����蔻��̎󂯓n��
/// </summary>
/// <returns>���W�Ƒ傫�����s��ɕϊ����đ������Box�̓����蔻��</returns>
Collision::Box CollisionBox::GetCollision()
{
	Collision::Box box;

	m_rot = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(m_dir));

	// �{�b�N�X�̒��S���W�����[���h�s��ɂ����W�ϊ�����
	Matrix world = Matrix::CreateFromQuaternion(m_rot) * Matrix::CreateTranslation(m_pos);
	Matrix radius = Matrix::CreateScale(m_scale);
	box.c = Vector3::Transform(m_collision.c, world);
	box.r = Vector3::Transform(m_collision.r, radius);

	return box;	// ���[�J���ϐ�box��Ԃ�
}

/// <summary>
///  �f�o�b�O�̓����蔻���`��
/// </summary>
void CollisionBox::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	// �f�o�b�N�p�R���W�����̕`��
	// ���[���h���W��Create����Set����
	//�@�u�傫���v�~�u�p�x�v�~�u���W�v�s�����
	Matrix world = Matrix::CreateScale(m_scale)*
		Matrix::CreateFromQuaternion(m_rot)*
		Matrix::CreateTranslation(m_pos);

	//m_debugObj->Draw(
	//	m_context,
	//	*states,
	//	world,
	//	view,
	//	projection
	//);
}

/// <summary>
///  ��̓����蔻��
/// </summary>
/// <param name="i">�R���W�����̐�</param>
/// <param name="box">�R���W�����̃f�[�^��n��</param>
//void CollisionBox::Set2Collision(DX::DeviceResources * deviceResources,int i, Collision::Box box)
//{
//	// �f�o�C�X�͈�񂵂��g��Ȃ��̂Ń��[�J���ϐ�
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	m_collision_2box[i] = box;
//
//	// �f�o�b�N�p���E�����f���̍쐬
//	m_debugMultipleObj[i] = make_unique<DebugBox>(
//		device,
//		m_collision_2box[i].c,		// ���E���̒��S
//		m_collision_2box[i].r		// ���E���̔��a
//		);
//}

/// <summary>
///  ��̃R���W�������󂯓n��
/// </summary>
/// <param name="i">�R���W�����̐�</param>
/// <returns>��̃R���W������n��</returns>
//Collision::Box CollisionBox::Get2Collision(int i)
//{
//	Collision::Box box;
//
//	// �{�b�N�X�̒��S���W�����[���h�s��ɂ����W�ϊ�����
//	Matrix world = Matrix::CreateTranslation(m_pos);
//	Matrix radius = Matrix::CreateScale(m_scale);
//	box.c = Vector3::Transform(m_collision_2box[i].c, world);
//	box.r = Vector3::Transform(m_collision_2box[i].r, radius);
//	
//	return box;
//}

/// <summary>
///  ��̃R���W������`�悷��
/// </summary>
//void CollisionBox::Draw2Collision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
//{
//	// �f�o�b�N�p�R���W�����̕`
//	// ���[���h���W��Create����Set����
//	//�@�u�傫���v�~�u�p�x�v�~�u���W�v�s�����
//	Matrix world = Matrix::CreateScale(m_scale)*
//		Matrix::CreateTranslation(m_pos);
//	
//	for (int i = 0; i < 2; i++)
//	{
//		//m_debugMultipleObj[i]->Draw(
//		//	m_context,
//		//	*states,
//		//	world,
//		//	view,
//		//	projection
//		//);
//	}
//}

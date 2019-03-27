//----------------------------------------
// MyEffect.cpp
//----------------------------------------

#include "../pch.h"
#include "Myeffect.h"

#include "../DeviceResources.h"
#include "../StepTimer.h"
#include <SimpleMath.h>
#include <Effects.h>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <WICTextureLoader.h>
#include <CommonStates.h>
#include "../BinaryFile.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �쐬�֐�
/// </summary>
void MyEffect::Create()
{
}

/// <summary>
/// �������֐�
/// </summary>
/// <param name="life">�����l</param>
/// <param name="pos">���W</param>
/// <param name="velocity">���x�x�N�g��</param>
void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_startPosition = m_position = pos;
	m_startVelocity = m_velocity = velocity;
	m_startLife = m_life = life;
	m_gravity = Vector3(0, 0.001f, 0);
}

/// <summary>
/// �X�V�֐�
/// </summary>
/// <param name="elapsedTime">�X�V����</param>
/// <returns>true == �X�V�L��, false == �X�V����</returns>
bool MyEffect::Update(float elapsedTime)
{
	// ��]
	m_position += m_velocity * 2.0f;

	return true;
}



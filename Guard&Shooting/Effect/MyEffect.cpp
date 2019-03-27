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
/// 作成関数
/// </summary>
void MyEffect::Create()
{
}

/// <summary>
/// 初期化関数
/// </summary>
/// <param name="life">生存値</param>
/// <param name="pos">座標</param>
/// <param name="velocity">速度ベクトル</param>
void MyEffect::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 velocity)
{
	m_startPosition = m_position = pos;
	m_startVelocity = m_velocity = velocity;
	m_startLife = m_life = life;
	m_gravity = Vector3(0, 0.001f, 0);
}

/// <summary>
/// 更新関数
/// </summary>
/// <param name="elapsedTime">更新時間</param>
/// <returns>true == 更新有り, false == 更新無し</returns>
bool MyEffect::Update(float elapsedTime)
{
	// 回転
	m_position += m_velocity * 2.0f;

	return true;
}



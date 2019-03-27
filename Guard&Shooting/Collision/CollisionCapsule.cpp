//
//	File: CollisionCapsule.cpp
//
// 　カプセルを持ったObje3Dクラス
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
//	// デバイスは一回しか使わないのでローカル変数
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	// 境界球の情報設定
//	m_collision = capsule;
//
//	// デバック用境界球モデルの作成
//	m_debugObj =
//		make_unique<DebugCapsule>(
//			device,
//			m_collision.a,		// 中間部の線分の開始地点
//			m_collision.b,		// 中間部の線分の終了地点
//			m_collision.r		// 線分の半径
//			);
//}

void CollisionCapsule::SetCollision(Collision::Capsule capsule)
{
	// 境界球の情報設定
	m_collision = capsule;
}

Collision::Capsule CollisionCapsule::GetCollision()
{
	Collision::Capsule capsule;

	// 境界球の中心座標をワールド行列により座標変換する
	capsule.a = Vector3::Transform(m_collision.a, m_world);
	capsule.b = Vector3::Transform(m_collision.b, m_world);
	capsule.r = m_collision.r;

	// ローカル変数sphereを返す
	return capsule;
}

void CollisionCapsule::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	// デバック用境界球の描画
	//m_debugObj->Draw(
	//	m_context,
	//	*states,
	//	m_world,
	//	view,
	//	projection
	//);
}

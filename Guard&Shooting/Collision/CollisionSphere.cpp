//
//	File: CollisionSphere.cpp
//
// 　境界球を持ったObje3Dクラス
//
//	name: Hibiki Yoshiyasu
#include "../../pch.h"
#include "CollisionSphere.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

//void CollisionSphere::SetCollision(DX::DeviceResources * deviceResources,Collision::Sphere sphere)
//{
//	// デバイスは一回しか使わないのでローカル変数
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//	// 境界球の情報設定
//	m_collision = sphere;
//
//	// デバック用境界球モデルの作成
//	m_debugObj = make_unique<DebugSphere>(
//		device,
//		m_collision.c,		// 境界球の中心
//		m_collision.r		// 境界球の半径
//		);
//}

void CollisionSphere::SetCollision(Collision::Sphere sphere)
{
	// 境界線球の情報設定
	m_collision = sphere;
}

Collision::Sphere CollisionSphere::GetCollision()
{
	Collision::Sphere sphere;

	// 境界球の中心座標をワールド行列により座標変換する
	sphere.c = Vector3::Transform(m_collision.c,m_world);
	sphere.r = m_collision.r;

	return sphere;	// ローカル変数sphereを返す
}

void CollisionSphere::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
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

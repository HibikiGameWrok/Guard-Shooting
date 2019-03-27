#include "../../pch.h"
#include "CollisionBox.h"
#include "../../Game.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;

/// <summary>
///  当たり判定の受け取り
/// </summary>
/// <param name="box">Boxの当たり判定範囲</param>
//void CollisionBox::SetCollision(DX::DeviceResources * deviceResources,Collision::Box box)
//{
//	// デバイスは一回しか使わないのでローカル変数
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	// 境界球の情報設定
//	m_collision = box;
//	
//	// デバック用境界球モデルの作成
//	m_debugObj = make_unique<DebugBox>(
//		device,
//		m_collision.c,		// 境界球の中心
//		m_collision.r		// 境界球の半径
//		);
//}

void CollisionBox::SetCollision(Collision::Box box)
{
	// 境界線四角形の情報設定
	m_collision = box;
}

/// <summary>
///  当たり判定の受け渡し
/// </summary>
/// <returns>座標と大きさを行列に変換して代入したBoxの当たり判定</returns>
Collision::Box CollisionBox::GetCollision()
{
	Collision::Box box;

	m_rot = Quaternion::CreateFromAxisAngle(Vector3(0.0f, 1.0f, 0.0f), XMConvertToRadians(m_dir));

	// ボックスの中心座標をワールド行列により座標変換する
	Matrix world = Matrix::CreateFromQuaternion(m_rot) * Matrix::CreateTranslation(m_pos);
	Matrix radius = Matrix::CreateScale(m_scale);
	box.c = Vector3::Transform(m_collision.c, world);
	box.r = Vector3::Transform(m_collision.r, radius);

	return box;	// ローカル変数boxを返す
}

/// <summary>
///  デバッグの当たり判定を描画
/// </summary>
void CollisionBox::DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
{
	// デバック用コリジョンの描画
	// ワールド座標にCreate時にSetした
	//　「大きさ」×「角度」×「座標」行列を代入
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
///  二つの当たり判定
/// </summary>
/// <param name="i">コリジョンの数</param>
/// <param name="box">コリジョンのデータを渡す</param>
//void CollisionBox::Set2Collision(DX::DeviceResources * deviceResources,int i, Collision::Box box)
//{
//	// デバイスは一回しか使わないのでローカル変数
//	ID3D11Device* device = deviceResources->GetD3DDevice();
//
//	m_collision_2box[i] = box;
//
//	// デバック用境界球モデルの作成
//	m_debugMultipleObj[i] = make_unique<DebugBox>(
//		device,
//		m_collision_2box[i].c,		// 境界球の中心
//		m_collision_2box[i].r		// 境界球の半径
//		);
//}

/// <summary>
///  二つのコリジョンを受け渡し
/// </summary>
/// <param name="i">コリジョンの数</param>
/// <returns>二つのコリジョンを渡す</returns>
//Collision::Box CollisionBox::Get2Collision(int i)
//{
//	Collision::Box box;
//
//	// ボックスの中心座標をワールド行列により座標変換する
//	Matrix world = Matrix::CreateTranslation(m_pos);
//	Matrix radius = Matrix::CreateScale(m_scale);
//	box.c = Vector3::Transform(m_collision_2box[i].c, world);
//	box.r = Vector3::Transform(m_collision_2box[i].r, radius);
//	
//	return box;
//}

/// <summary>
///  二つのコリジョンを描画する
/// </summary>
//void CollisionBox::Draw2Collision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection)
//{
//	// デバック用コリジョンの描
//	// ワールド座標にCreate時にSetした
//	//　「大きさ」×「角度」×「座標」行列を代入
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

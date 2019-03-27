//
//	File: CollisionBox.h
//
// 　境界球を持ったObje3Dクラス
//
//	name: Hibiki Yoshiyasu
#pragma once
// インクルードは噛み合わせ注意
#include "Debug/DebugBox.h"
#include "Collision.h"
#include "../../Data/Draw3DData.h"

class CollisionBox : public Draw3DData
{
public:
	// 境界Boxの情報設定関数
	//void SetCollision(DX::DeviceResources * deviceResources,Collision::Box box);
	// 境界Boxの情報設定関数
	void SetCollision(Collision::Box box);

	//void Set2Collision(DX::DeviceResources * deviceResources,int i, Collision::Box box);

	// 境界Boxの情報取得関数
	Collision::Box GetCollision();

	//Collision::Box Get2Collision(int i);

	// デバック用境界球表示関数
	void DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

	//void Draw2Collision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);
private:
	// 境界球の情報
	Collision::Box m_collision;

	//Collision::Box m_collision_2box[2];

	// 境界球表示オブジェクト
	std::unique_ptr<DebugBox> m_debugObj;

	//std::unique_ptr<DebugBox> m_debugMultipleObj[2];

};

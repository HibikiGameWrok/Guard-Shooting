//
//	File: CollisionCapsule.h
//
// 　境界球を持ったObje3Dクラス
//
//	name: Hibiki Yoshiyasu
#pragma once
// インクルードは噛み合わせ注意
#include "Debug/DebugCapsule.h"
#include "Collision.h"
#include "../../Data/Draw3DData.h"

class CollisionCapsule : public Draw3DData
{
public:
	// 境界球の情報設定関数
	//void SetCollision(DX::DeviceResources * deviceResources,Collision::Capsule capsule);
	// 境界球の情報設定関数
	void SetCollision(Collision::Capsule capsule);
	// 境界球の情報取得関数
	Collision::Capsule GetCollision();

	// デバック用境界球表示関数
	void DrawCollision(DirectX::CommonStates * states, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix projection);

private:
	// 境界球の情報
	Collision::Capsule m_collision;

	// 境界球表示オブジェクト
	std::unique_ptr<DebugCapsule> m_debugObj;

};

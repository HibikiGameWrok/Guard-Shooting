#pragma once

#include "Camera.h"

class GameCamera : public Camera
{
public:
	// コンストラクタ
	GameCamera();
	~GameCamera();

private:
	// 回転角
	float m_angle;

public:
	// タイトル用カメラ
	void TitleCamera();

	// プレイ用カメラ
	void PlayCamera(DirectX::SimpleMath::Vector3 target,float direction);

};
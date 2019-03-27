#include "../../pch.h"
#include "MyCamera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

GameCamera::GameCamera()
	: m_angle(0.0f)
{

}

GameCamera::~GameCamera()
{

}

/// <summary>
/// タイトルシーン用カメラ
/// </summary>
void GameCamera::TitleCamera()
{
	Vector3 eye(7.0f, 4.0f, 0.0f);

	// 中視点は(0.0f,0.0f,0.0f)でカメラをY軸回転させている
	m_angle += 0.5f;
	Matrix rotY = Matrix::CreateRotationY(XMConvertToRadians(m_angle));
	eye = Vector3::Transform(eye, rotY);
	SetPositionTarget(eye, Vector3(0.0f, 0.0f, 0.0f));
}

/// <summary>
/// プレイシーン用カメラ
/// </summary>
void GameCamera::PlayCamera(DirectX::SimpleMath::Vector3 target,float direction)
{
	Vector3 eye = Vector3(0.0f, 1.55f, 1.5f);

	Matrix rot = Matrix::CreateRotationX(-2.0f) * Matrix::CreateRotationY(direction) ;

	eye = Vector3::Transform(eye, rot);
	eye += target;
	SetPositionTarget(eye, target);
}

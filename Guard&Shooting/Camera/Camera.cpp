//--------------------------------------------------------------------------------------
// File: Camera.cpp
//
// カメラクラス
//
// Date: 2018.7.9
// Author: Hideyasu Imase
//--------------------------------------------------------------------------------------
#include "../../pch.h"
#include "Camera.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

// 適当な値です
const float Camera::CAMERA_K = 1.0f / 40.0f;	//	ばね係数
const float Camera::CAMERA_M = 1.0f / 2.0f;		//	摩擦係数
const float Camera::CAMERA_DISTANCE = 10.0f;	//	基準の距離

Camera::Camera()
	: m_resetFlag(true), m_k(CAMERA_K), m_m(CAMERA_M), m_distance(CAMERA_DISTANCE)
{
}

void Camera::ResetCamera()
{
	m_resetFlag = true;
}

void Camera::SetPositionTarget(Vector3& eye, Vector3& target)
{
	// カメラ切り替え時など補間したくない場合はInitializeCamera関数を呼びだしてください
	if (m_resetFlag == true)
	{
		m_resetFlag = false;
		m_eyePt = eye;
		m_targetPt = target;
		m_eyeSpeed = m_targetSpeed = Vector3(0.0f, 0.0f, 0.0f);
		return;
	}

	// 視点と注目点の距離ベクトルを取得
	m_vec =  target - eye;
	float dis = m_vec.Length();

	// ばね係数をカメラとターゲットの距離に応じて変更する
	m_eyeSpeed += (eye - m_eyePt) * m_k * m_distance / dis;
	m_eyeSpeed -= m_eyeSpeed * m_m;
	m_eyePt += m_eyeSpeed;

	// ばね係数をカメラとターゲットの距離に応じて変更する
	m_targetSpeed += (target - m_targetPt) * m_k * m_distance / dis;
	m_targetSpeed -= m_targetSpeed * m_m;
	m_targetPt += m_targetSpeed;
}

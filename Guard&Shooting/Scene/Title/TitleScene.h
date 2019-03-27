#pragma once
#include "../../ADX2/ADX2Le.h"
#include "../../Manager/DrawManager.h"
#include "../../Manager/InputManager.h"
#include "../../TaskSystem/TaskBase.h"

#include "../../Effect/EffectManager.h"
#include "../../Camera/MyCamera.h"

class TitleScene : public Task::TaskBase
{
private:
	MyLibrary::ADX2Le *m_pAdx2le;

	unsigned __int8 m_randOni;

	unsigned int m_changeTime;
	bool m_changeFalg;
	EffectManager*	m_pEffectManager;	// エフェクトマネージャー

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// カメラ
	GameCamera m_camera;
public:
	TitleScene();
	~TitleScene();
public:
	// 開始処理
	void Start() override;

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Draw() override;
	void DrawBegin() override;
	void DrawEnd() override;

};

//
//	File	 : PlayScene.h
//
//  Contents : ゲームをプレイするシーンクラス
//
//	name	 : Hibiki Yoshiyasu
//
#pragma once
#include "../../ADX2/ADX2Le.h"
#include "../../StepTimer.h"

#include "../../DeviceResources.h"

#include "../../Manager/DrawManager.h"
#include "../../Manager/InputManager.h"

#include "../../TaskSystem/TaskBase.h"

#include "../../GameObj/UI/TeachPlayStartUI.h"
#include "../../GameObj/UI/CountDownUI.h"
#include "../../GameObj/UI/JudgeUI.h"
#include "../../GameObj/UI/StaminaBer.h"
#include "../../GameObj/UI/GameTime.h"

#include "../../GameObj/Shadow/Shadow.h"
#include "../../GameObj/Cpu/Cpu.h"
#include "../../GameObj/Player/Player.h"

#include "../../GameObj/Ball/Ball.h"

#include "../../GameObj/Wall/Plane/BoardWall.h"
#include "../../Manager/WallManager.h"

#include "../../Camera/MyCamera.h"

#include "../../Effect/EffectManager.h"

class PlayScene : public Task::TaskBase
{
public:
	static const int MAX_SHADOW = 2;
	static const int MAX_WALL = 32;
	static const int HALF_WALL = MAX_WALL / 2;
	static const int MAX_OBSTACLE_WALL = 5;
private:
	MyLibrary::ADX2Le *m_pAdx2le;

	TeachPlayStartUI* m_pTeachPlayStart; // 開始する前に表示するUI

	CountDownUI* m_pCountDown;  // カウントダウンするUI

	StaminaGauge* m_pStamina;		// スタミナバー
	GameTime* m_pTime;			// 時間	
	JudgeUI* m_pJudgeUI;		// 誰が鬼なのかを判定するUI

	Shadow* m_pShadow[MAX_SHADOW];			// 影モデル

	Player* m_pPlayer;			// プレイヤー
	Cpu* m_pCpu;				// コンピュータ

	Ball* m_pBall;				// ボール

	//WallManager* m_pOuterWall;	// 壁マネージャー
	//WallManager* m_pInnerWall;	// 壁マネージャー

	BoardWall* m_pBWall[32];	// 外枠の壁※今後マネージャーを通して作成する
	BoardWall* m_pBWall_a[5];	// 障害物の壁

	GameCamera m_camera;		// カメラ

	DX::StepTimer  m_timer;	// タイマー

	EffectManager*	m_pEffectManager;	// エフェクトマネージャー
private:
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;

	// ゲームが開始するフラグ
	bool m_startFlag;

	// フレーム数
	int m_frameTime;
	// 秒数
	int m_secondsTime;

	// シーン切り替えタイム
	int m_changeTime;

	// 現在の鬼
	unsigned __int8 m_nowOni;

	// 衝突しているかどうか
	bool m_hitFlag;

public:
	PlayScene() {};
	PlayScene(unsigned __int8 nowOni);
	~PlayScene();

public:
	// 開始処理
	void Start() override;

	void Create2D();
	void Create3D();

	// 更新
	bool Update(float elapsedTime);
	
	// 描画
	void Draw() override;
	void DrawBegin() override;
	void DrawEnd() override;

	// ゲーム開始処理
	void StartGame();

	// シーン切り替え関数
	bool SceneChange();

	// 設定更新
	void SetUpFunc();

public:
	// ビュー行列の設定
	void SetView(DirectX::SimpleMath::Matrix view) { m_view = view; }
	DirectX::SimpleMath::Matrix GetView() { return m_view; }

	// 射影行列の設定
	void SetProjection(DirectX::SimpleMath::Matrix projection) { m_projection = projection; }
	DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }
};

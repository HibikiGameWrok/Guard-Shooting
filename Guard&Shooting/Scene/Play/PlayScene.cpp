//
//	File	 : PlayScene.cpp
//
//  Contents : ゲームをプレイするシーンクラスの定義
//
//	name	 : Hibiki Yoshiyasu
//

#include "../../pch.h"
#include "../../DeviceResources.h"

#include "PlayScene.h"
#include "../Result/ResultScene.h"

#include "../../GameObj/UI/StaminaBer.h"
#include "../../GameObj/UI/KeyOperationUI.h"
#include "../../GameObj/UI/ImageInstructionKey.h"

#include "../../GameObj/SkyBox/SkyBox.h"
#include "../../GameObj/Floor/Floor.h"

#include "../../GameObj/Wall/Plane/BoardWall.h"

#include "../../Collision/Collision.h"

using BtnStateTracker = DirectX::Keyboard;

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace System;

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="nowOni">鬼は誰なのか示す値</param>
/// nowOni : '1' = プレイヤーが鬼 , '2以降' = Cpuが鬼 
PlayScene::PlayScene(unsigned __int8 nowOni)
	:m_startFlag(false)
	,m_nowOni(nowOni)
	, m_pEffectManager(nullptr)
	, m_changeTime(0)
	, m_hitFlag(true)
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayScene::~PlayScene()
{

}

/// <summary>
/// 開始処理
/// </summary>
void PlayScene::Start()
{
	// ADX2Leを生成
	m_pAdx2le = ADX2Le::GetInstance();
	m_pAdx2le->LoadAcb(L"PlaySceneCue.acb", L"PlaySceneCue.awb");
	m_pAdx2le->Play(0);

	// 画角を設定
	RECT size = DX::DeviceResources::GetInstance().GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	float fovAngleY = XMConvertToRadians(45.0f);

	// 射影行列のパーステクティブビューを設定
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);

	Create2D(); // 2Dオブジェクトを作成
	Create3D();	// 3Dオブジェクトを作成
}

/// <summary>
/// 2DSprite作成
/// </summary>
void PlayScene::Create2D()
{
	m_pCountDown = new CountDownUI(Vector2(600.0f,500.0f));	// ３カウントするUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pCountDown);

	m_pTeachPlayStart = new TeachPlayStartUI();   // 開始前に表示するUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pTeachPlayStart);

	m_pTime = new GameTime();	// ゲームの進行タイムUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pTime);

	m_pStamina = new StaminaGauge();	// スタミナバーUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pStamina);

	m_pJudgeUI = new JudgeUI();		// 鬼は誰か判断するUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pJudgeUI);
	m_pJudgeUI->SetNowOni(m_nowOni);

	KeyOperation* pKeyOperation = new KeyOperation();	// 移動キーUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), pKeyOperation);

	InstructionKey* pInstructionKey = new InstructionKey();	 // アクションキーUI
	Task::TaskManager::AddTask(GetThisTaskHandle(), pInstructionKey);
}

/// <summary>
/// 3DModel作成
/// </summary>
void PlayScene::Create3D()
{
	SkyBox* pSkyBox = new SkyBox();										// 背景オブジェクト
	Task::TaskManager::AddTask(GetThisTaskHandle(), pSkyBox, 0);

	Floor* pFloor = new Floor();										// 床のオブジェクト
	Task::TaskManager::AddTask(GetThisTaskHandle(), pFloor, 0);

	m_pPlayer = new Player(Vector3(-8.0f, 0.3f, 17.0f), 90);				// プレイヤー
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pPlayer, 1);

	m_pCpu = new Cpu(Vector3(11.0f, 0.3f, -3.0f), -90.0f);					// コンピュータ
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pCpu, 2);

	m_pBall = new Ball();													// ボール
	Task::TaskManager::AddTask(GetThisTaskHandle(), m_pBall, 3);

	for (int i = 0; i < MAX_SHADOW; i++)
	{
		m_pShadow[i] = new Shadow();								    // 影のモデルを作成
		Task::TaskManager::AddTask(GetThisTaskHandle(), m_pShadow[i], 0);
	}
	// 影のモデルにプレイヤーの座標を伝える
	m_pShadow[0]->SetPos(Vector3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y - 0.28f, m_pPlayer->GetPos().z));
	m_pShadow[1]->SetPos(Vector3(m_pCpu->GetPos().x, m_pCpu->GetPos().y - 0.28f, m_pCpu->GetPos().z));

#pragma region "壁:壁管理クラスで出す"
	// 四角型当たり判定の半径設定
	// 横向きの壁
	Vector3 boxRadius_FandB = Vector3(0.15f, 0.15f, 0.05f);
	// 縦向きの壁
	Vector3 boxRadius_LandR = Vector3(0.05f, 0.15f, 0.15f);

	m_pBWall[0] = new BoardWall(Vector3(0, 0, 19), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall[1] = new BoardWall(Vector3(3, 0, 19), 0, boxRadius_FandB);
	m_pBWall[2] = new BoardWall(Vector3(-3, 0, 19), 0, boxRadius_FandB);
	m_pBWall[3] = new BoardWall(Vector3(6, 0, 19), 0, boxRadius_FandB);
	m_pBWall[4] = new BoardWall(Vector3(-6, 0, 19), 0, boxRadius_FandB);
	m_pBWall[5] = new BoardWall(Vector3(9, 0, 19), 0, boxRadius_FandB);
	m_pBWall[6] = new BoardWall(Vector3(-9, 0, 19), 0, boxRadius_FandB);
	m_pBWall[7] = new BoardWall(Vector3(12, 0, 19), 0, boxRadius_FandB);

	m_pBWall[8] = new BoardWall(Vector3(0, 0, -5), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall[9] = new BoardWall(Vector3(3, 0, -5), 0, boxRadius_FandB);
	m_pBWall[10] = new BoardWall(Vector3(-3, 0, -5), 0, boxRadius_FandB);
	m_pBWall[11] = new BoardWall(Vector3(6, 0, -5), 0, boxRadius_FandB);
	m_pBWall[12] = new BoardWall(Vector3(-6, 0, -5), 0, boxRadius_FandB);
	m_pBWall[13] = new BoardWall(Vector3(9, 0, -5), 0, boxRadius_FandB);
	m_pBWall[14] = new BoardWall(Vector3(-9, 0, -5), 0, boxRadius_FandB);
	m_pBWall[15] = new BoardWall(Vector3(12, 0, -5), 0, boxRadius_FandB);

	m_pBWall[16] = new BoardWall(Vector3(13, 0, -3), 90, boxRadius_LandR);		// 板状の壁
	m_pBWall[17] = new BoardWall(Vector3(13, 0, 0), 90, boxRadius_LandR);
	m_pBWall[18] = new BoardWall(Vector3(13, 0, 3), 90, boxRadius_LandR);
	m_pBWall[19] = new BoardWall(Vector3(13, 0, 6), 90, boxRadius_LandR);
	m_pBWall[20] = new BoardWall(Vector3(13, 0, 9), 90, boxRadius_LandR);
	m_pBWall[21] = new BoardWall(Vector3(13, 0, 12), 90, boxRadius_LandR);
	m_pBWall[22] = new BoardWall(Vector3(13, 0, 15), 90, boxRadius_LandR);
	m_pBWall[23] = new BoardWall(Vector3(13, 0, 18), 90, boxRadius_LandR);

	m_pBWall[24] = new BoardWall(Vector3(-10, 0, -3), 90, boxRadius_LandR);		// 板状の壁
	m_pBWall[25] = new BoardWall(Vector3(-10, 0, 0), 90, boxRadius_LandR);
	m_pBWall[26] = new BoardWall(Vector3(-10, 0, 3), 90, boxRadius_LandR);
	m_pBWall[27] = new BoardWall(Vector3(-10, 0, 6), 90, boxRadius_LandR);
	m_pBWall[28] = new BoardWall(Vector3(-10, 0, 9), 90, boxRadius_LandR);
	m_pBWall[29] = new BoardWall(Vector3(-10, 0, 12), 90, boxRadius_LandR);
	m_pBWall[30] = new BoardWall(Vector3(-10, 0, 15), 90, boxRadius_LandR);
	m_pBWall[31] = new BoardWall(Vector3(-10, 0, 18), 90, boxRadius_LandR);
	for (int i = 0; i < MAX_WALL; i++)
	{
		Task::TaskManager::AddTask(GetThisTaskHandle(), m_pBWall[i]);
	}

	m_pBWall_a[0] = new BoardWall(Vector3(11, 0, -1), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall_a[1] = new BoardWall(Vector3(-8, 0, 15), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall_a[2] = new BoardWall(Vector3(0, 0, 8), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall_a[3] = new BoardWall(Vector3(4, 0, 5), 0, boxRadius_FandB);		// 板状の壁
	m_pBWall_a[4] = new BoardWall(Vector3(-3, 0, 13), 0, boxRadius_FandB);		// 板状の壁
	for (int i = 0; i < MAX_OBSTACLE_WALL; i++)
	{
		Task::TaskManager::AddTask(GetThisTaskHandle(), m_pBWall_a[i]);
	}
#pragma endregion

	//m_pOuterWall = new WallManager(32,L"Resources\\WallEdit\\OuterWall_Data.csv");		// 外枠の壁を管理するクラス
	//Task::TaskManager::AddTask(GetThisTaskHandle(), m_pOuterWall);

	//m_pInnerWall = new WallManager(5, L"Resources\\WallEdit\\InnerWall_Data.csv");		// 内側の壁を管理するクラス
	//Task::TaskManager::AddTask(GetThisTaskHandle(), m_pInnerWall);
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns>true = 更新されている,false = 更新されていない</returns>
bool PlayScene::Update(float elapsedTime)
{
	// ゲームを開始する処理
	StartGame();
	// ゲームが開始された時
	if (m_startFlag != false)
	{
		// 開始前に描画するUIを削除
		if (m_pTeachPlayStart != nullptr) {
			Task::TaskManager::RemoveTask(m_pTeachPlayStart->GetThisTaskHandle());
			m_pTeachPlayStart = nullptr;
		}

		// カウントを始める
		m_pTime->SetStopFlag(false);

		// 当たり判定等の更新(後に消す)
		SetUpFunc();

		// 影のモデルにプレイヤーの座標を伝える
		m_pShadow[0]->SetPos(Vector3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y - 0.28f, m_pPlayer->GetPos().z));
		m_pShadow[1]->SetPos(Vector3(m_pCpu->GetPos().x, m_pCpu->GetPos().y - 0.28f, m_pCpu->GetPos().z));

		// シーン切り替えの処理が終わったら更新を止める
		if (SceneChange() == false)
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void PlayScene::Draw()
{
	// プレイ用カメラを設定
	m_camera.PlayCamera(m_pPlayer->GetPos(), m_pPlayer->GetDir());

	// ビュー行列の作成
	m_view = Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		Vector3::Up
	);

	// 描画マネージャーのビュー行列に設定
	System::Draw3DManager::GetInstance().SetView(m_view);

	// エフェクトをビルボード化
	if (m_pEffectManager != nullptr)
	{
		// エフェクトの描画設定
		m_pEffectManager->SetRenderState(m_camera.GetEyePosition(),m_view, m_projection);
	}
}

/// <summary>
/// 描画開始処理
/// </summary>
void PlayScene::DrawBegin()
{
	System::DrawManager::GetInstance().Begin();
}

/// <summary>
/// 描画終了処理
/// </summary>
void PlayScene::DrawEnd()
{
	System::DrawManager::GetInstance().End();
}

/// <summary>
/// ゲーム開始処理
/// </summary>
void PlayScene::StartGame()
{
	if(m_startFlag == false)
	{
		m_pCountDown->SetMoveFlag(true);
	}

	if (m_pCountDown->GetMoveFlag() == false)
	{
		// ゲームが始まっている状態
		m_startFlag = true;
	}
}

/// <summary>
/// シーンを切り替える処理
/// </summary>
bool PlayScene::SceneChange()
{
	// タイムが切れた時
	if (m_pTime->GetFinishFlag() == true)
	{
		// カウントをする
		m_changeTime++;
	}
	// カウントが1秒の時音を鳴らす
	if (m_changeTime > 0 && m_changeTime < 2)
	{
		m_pAdx2le->Play(4);
	}

	// 180フレームカウントされた時シーン切り替え
	if (m_changeTime > 180)
	{
		// 誰が鬼なのかを変数に代入
		m_nowOni = m_pJudgeUI->GetNowOni();
		// 結果を表示する為にリザルトシーンへ誰が鬼なのかを伝える
		ResultScene* pResultScene = new ResultScene(m_nowOni);
		Task::TaskManager::AddTask(pResultScene, 0);
		return false;
	}
	return true;
}

/// <summary>
/// 当たり判定や設定関数を呼ぶ処理(後に消す)
/// </summary>
void PlayScene::SetUpFunc()
{
	// 鬼がだれか毎フレーム設定
	m_pJudgeUI->SetNowOni(m_nowOni);
	// スタミナバーがあるかどうかをプレイヤーに伝える
	m_pStamina->SetDashFlag(m_pPlayer->GetSrowFlag());
	m_pPlayer->SetDashFlag(m_pStamina->GetBerZeroFlag());

	// プレイヤーが鬼の時に
	if (m_nowOni == 1)
	{
		// 発射の命令をした時
		if (m_pPlayer->GetShotFlag() == true) 
		{
			// ボールがまだ発射されていない時
			if (m_pBall->GetShotFlag() == false) 
			{
				m_hitFlag = false;
				// ボールの座標をプレイヤーの中心にする
				m_pBall->SetPos(m_pPlayer->GetPos());
				// ボールを発射
				m_pBall->SetShotFlag(true);
				// 当たった時の音を出す
				m_pAdx2le->Play(2);
			}
		}
	}

	// CPUが鬼の時
	if (m_nowOni == 2){
		// CPU自身に鬼になったと知らせる
		m_pCpu->SetOniFlag(true);
		// 発射の命令をした時
		if (m_pCpu->GetShotFlag() == true)
		{
			m_hitFlag = false;
			// ボールがまだ発射されていない時
			if (m_pBall->GetShotFlag() == false)
			{
				// ボールの座標をプレイヤーの中心にする
				m_pBall->SetPos(m_pCpu->GetPos());
				// ボールを発射
				m_pBall->SetShotFlag(true);
				// 当たった時の音を出す
				m_pAdx2le->Play(2);
			}
			// 発射できる状態に戻す
			m_pCpu->SetShotFlag(false);
		}
	}
	else
	{
		// 鬼ではないと知らせる
		m_pCpu->SetOniFlag(false);
	}

#pragma region "当たり判定処理(管理クラスに処理を移行後消す)"
#pragma region 壁との当たり判定
	// 前後の壁に当たった時
	for (int i = 0; i < HALF_WALL; i++)
	{
		// プレイヤーと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pPlayer->GetCollision(), m_pBWall[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 pPos = m_pPlayer->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall[i]->GetPos();

			// 壁の中心ｚ軸から幅0.05fよりもプレイヤーのｚ軸が超えていたら
			if (wPos.z - 0.05f > pPos.z)
			{
				pPos = Vector3(pPos.x, pPos.y, wPos.z - 0.8f);
				m_pPlayer->SetPos(pPos);
			}
			else if (wPos.z + 0.05f < pPos.z)
			{
				pPos = Vector3(pPos.x, pPos.y, wPos.z + 0.8f);
				m_pPlayer->SetPos(pPos);
			}
		}

		// コンピュータと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pCpu->GetCollision(), m_pBWall[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 cPos = m_pCpu->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall[i]->GetPos();

			// 壁の中心ｚ軸から幅0.05fよりもプレイヤーのｚ軸が超えていたら
			if (wPos.z - 0.05f > cPos.z)
			{
				cPos = Vector3(cPos.x, cPos.y, wPos.z - 0.8f);
				m_pCpu->SetPos(cPos);
			}
			else if (wPos.z + 0.05f < cPos.z)
			{
				cPos = Vector3(cPos.x, cPos.y, wPos.z + 0.8f);
				m_pCpu->SetPos(cPos);
			}
		}
	}

	// 左右の壁に当たった時
	for (int i = HALF_WALL; i < MAX_WALL; i++)
	{
		// プレイヤーと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pPlayer->GetCollision(), m_pBWall[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 pPos = m_pPlayer->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall[i]->GetPos();

			// 壁の中心x軸から幅0.1fよりもプレイヤーのx軸が超えていたら
			if (wPos.x - 0.05f < pPos.x) {
				pPos = Vector3(wPos.x + 0.8f, pPos.y, pPos.z);
				m_pPlayer->SetPos(pPos);
			}
			else
				if (wPos.x + 0.05f > pPos.x)
				{
					pPos = Vector3(wPos.x - 0.8f, pPos.y, pPos.z);
					m_pPlayer->SetPos(pPos);
				}
		}

		// コンピュータと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pCpu->GetCollision(), m_pBWall[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 cPos = m_pCpu->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall[i]->GetPos();

			// 壁の中心x軸から幅0.1fよりもプレイヤーのx軸が超えていたら
			if (wPos.x - 0.05f < cPos.x)
			{
				cPos = Vector3(wPos.x + 0.8f, cPos.y, cPos.z);
				m_pCpu->SetPos(cPos);
			}
			else
				if (wPos.x + 0.05f > cPos.x)
				{
					cPos = Vector3(wPos.x - 0.8f, cPos.y, cPos.z);
					m_pCpu->SetPos(cPos);
				}
		}
	}
	
	// 玉が外枠の壁に当たった時
	for (int i = 0; i < MAX_WALL; i++)
	{
		// 玉と壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pBall->GetCollision(), m_pBWall[i]->GetCollision()) == true) {
			m_pBall->SetShotFlag(false);
		}
	}

	// 障害物の壁に当たった時
	for (int i = 0; i < MAX_OBSTACLE_WALL; i++)
	{
		// プレイヤーと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pPlayer->GetCollision(), m_pBWall_a[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 pPos = m_pPlayer->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall_a[i]->GetPos();
			// 壁の中心ｚ軸から幅0.05fよりもプレイヤーのｚ軸が超えていたら
			if (wPos.z - 0.05f > pPos.z)
			{
				pPos = Vector3(pPos.x, pPos.y, wPos.z - 0.8f);
				m_pPlayer->SetPos(pPos);
			}
			else if (wPos.z + 0.05f < pPos.z)
			{
				pPos = Vector3(pPos.x, pPos.y, wPos.z + 0.8f);
				m_pPlayer->SetPos(pPos);
			}
		}

		// コンピュータと壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pCpu->GetCollision(), m_pBWall_a[i]->GetCollision()) == true) {
			// 当たった時のプレイヤーの座標
			Vector3 cPos = m_pCpu->GetPos();
			// 当てられた壁の座標
			Vector3 wPos = m_pBWall_a[i]->GetPos();
			// 壁の中心ｚ軸から幅0.05fよりもプレイヤーのｚ軸が超えていたら
			if (wPos.z - 0.05f > cPos.z)
			{
				cPos = Vector3(cPos.x, cPos.y, wPos.z - 0.8f);
				m_pCpu->SetPos(cPos);
			}
			else if (wPos.z + 0.05f < cPos.z)
			{
				cPos = Vector3(cPos.x, cPos.y, wPos.z + 0.8f);
				m_pCpu->SetPos(cPos);
			}
		}
	}

	// 玉が障害物の壁に当たった時
	for (int i = 0; i < MAX_OBSTACLE_WALL; i++)
	{
		// 玉と壁の当たり判定
		if (Collision::HitCheck_Sphere2Box(m_pBall->GetCollision(), m_pBWall_a[i]->GetCollision()) == true) {
			m_pBall->SetShotFlag(false);
		}
	}
#pragma endregion 

#pragma region プレイヤーか敵と玉との当たり判定
	// 連続で当たっていなければ
	if (m_hitFlag == false)
	{
		// 鬼がプレイヤーじゃない時
		if (m_nowOni != 1)
		{
			// 玉がプレイヤーに当たったら
			if (Collision::HitCheck_Sphere2Sphere(m_pBall->GetCollision(), m_pPlayer->GetCollision()) == true)
			{
				// 連続で判定させない様にする
				m_hitFlag = true;
				// 当たった時の音を鳴らす
				m_pAdx2le->Play(1);
				// 発射しない状態にする
				m_pBall->SetShotFlag(false);
				// 現在の鬼をプレイヤーにする
				m_nowOni = 1;
			}
		}

		// 鬼がCPUじゃない時
		if (m_nowOni != 2)
		{
			// 玉がコンピュータに当たった時
			if (Collision::HitCheck_Sphere2Sphere(m_pBall->GetCollision(), m_pCpu->GetCollision()) == true)
			{
				// 連続で判定させない様にする
				m_hitFlag = true;
				// 当たった時の音を鳴らす
				m_pAdx2le->Play(1);
				// 発射フラグをオフ
				m_pBall->SetShotFlag(false);
				// 現在の鬼をコンピュータにする
				m_nowOni = 2;
				// UIを動かす為の指示を設定
				m_pJudgeUI->SetMoveUI(true);
				if (m_nowOni == 1)
				{
					// スタンさせる
					m_pCpu->SetStanFlag(true);
				}
				// エフェクトを出す
				if (m_pEffectManager == nullptr)
				{
					// ランダムの幅
					/*int range = 1000;*/
					// エフェクトを生成
					m_pEffectManager = new EffectManager();
					m_pEffectManager->CreateA(5, L"Resources\\Textures\\Damage_effect.png");
					m_pEffectManager->Initialize(10, Vector3(m_pCpu->GetPos()), Vector3(0, 3, 0)/*Vector3(((rand() % (range * 2)) - range) / (float)range * 0.2f, ((rand() % (range * 2)) + range) / (float)range * 0.2f, 0)*/);
					Task::TaskManager::AddTask(GetThisTaskHandle(), m_pEffectManager, 4);
				}
			}
		}
	}
#pragma endregion
#pragma endregion

	// 玉が発射されていない時
	if (m_pBall->GetShotFlag() == false)
	{
		// 鬼がプレイヤー以外の時
		if (m_nowOni != 1) 
		{	// 鬼がプレイヤー以外なら		
			m_pBall->SetPos(Vector3(m_pCpu->GetPos().x, m_pCpu->GetPos().y + 0.4f, m_pCpu->GetPos().z));
			m_pBall->SetRot(m_pCpu->GetRot());
			m_pBall->SetShotFlag(false);
		}

		// プレイヤーが鬼の時
		if (m_nowOni == 1) 
		{
			m_pBall->SetPos(Vector3(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + 0.4f, m_pPlayer->GetPos().z));
			m_pBall->SetRot(m_pPlayer->GetRot());
		}
	}

	// 鬼のAIを更新
	m_pCpu->SetTargetPos(m_pPlayer->GetPos());

	// エフェクトが出ている時に３秒後に消す
	static int timer = 0;
	if (m_pEffectManager != nullptr)
	{
		// エフェクトが出ている間カウント
		timer++;
		if (timer > 180)
		{
			// エフェクトが３秒経ったら出ているエフェクトのタスクを削除
			Task::TaskManager::RemoveTask(m_pEffectManager->GetThisTaskHandle());
			// 保管していた変数の中身を初期化
			m_pEffectManager = nullptr;
			timer = 0;
		}
	}
}



#include "../../pch.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "TitleScene.h"

#include "../Play/PlayScene.h"

#include "../../GameObj/UI/TitleName.h"
#include "../../GameObj/UI/TitleBackGround.h"
#include "../../GameObj/UI/PleaseSpaceUI.h"

#include "../Result/ResultScene.h"

#include "../../GameObj/Player/Player.h"

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace System;

/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene()
	:m_changeTime(0)
	,m_changeFalg(false)
	,m_pEffectManager(nullptr)
	, m_randOni(0)
{
	// srand関数で、乱数パターンを初期化する
	srand((unsigned)time(NULL));
	// 毎回異なる乱数を取得
	m_randOni = rand() % 2 + 1;
}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Start()
{
	m_pAdx2le = ADX2Le::GetInstance();
	m_pAdx2le->LoadAcb(L"TitleSceneCue.acb", L"TitleSceneCue.awb");
	m_pAdx2le->Play(0);

	// 背景
	Player* pPlayer = new Player(Vector3(0.0f,-0.3f,0.0f),90);
	Task::TaskManager::AddTask(GetThisTaskHandle(), pPlayer, 0);

	// タイトル名
	TitleName* pTitleName = new TitleName();
	Task::TaskManager::AddTask(GetThisTaskHandle(), pTitleName,1);
	
	// 開始方法を明記されたUI
	PleaseSpaceUI* pPleaseSpace = new PleaseSpaceUI(Vector2(800 / 2, 400));
	Task::TaskManager::AddTask(GetThisTaskHandle(), pPleaseSpace,2);

}

/// <summary>
/// 更新処理
/// </summary>
bool TitleScene::Update(float elapsedTime)
{

	// スペースキーでゲーム開始(PlaySceneへ移行)
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space)) {
		if (m_changeFalg != true)
		{
			m_pAdx2le->Play(1);
		}
		m_changeFalg = true;
		
		m_pEffectManager = new EffectManager();
		m_pEffectManager->CreateAB(1, L"Resources\\Textures\\Black_out.png", L"Resources\\Textures\\floor.png");
		m_pEffectManager->Initialize(1, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f));
		Task::TaskManager::AddTask(GetThisTaskHandle(), m_pEffectManager, 3);
	}

	// シーン移行する際にクールタイムをカウント
	if (m_changeFalg == true)
	{
		m_changeTime++;
	}

	// 240 == 4秒経過したらシーン移行
	if (m_changeTime > 120)
	{
		PlayScene* pPlay = new PlayScene(m_randOni);	  // シーンを作成
		Task::TaskManager::AddTask(pPlay, 0);			  // シーンを追加
		return false;
	}
	return true;
}


/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw()
{
	m_camera.TitleCamera();

	m_view = Matrix::CreateLookAt(
		m_camera.GetEyePosition(),
		m_camera.GetTargetPosition(),
		Vector3::Up
	);

	if (m_pEffectManager != nullptr)
	{
		// エフェクトの描画設定
		m_pEffectManager->SetRenderState(Vector3(0,0,0),m_view, m_projection);
	}
}


/// <summary>
/// 描画開始
/// </summary>
void TitleScene::DrawBegin()
{
	System::DrawManager::GetInstance().Begin();
}

/// <summary>
/// 描画終了
/// </summary>
void TitleScene::DrawEnd()
{
	System::DrawManager::GetInstance().End();
}
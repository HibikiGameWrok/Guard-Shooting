#include "../../pch.h"
#include "ResultScene.h"

#include "../Title\\TitleScene.h"

#include "../../GameObj/UI/PleaseSpaceUI.h"
#include "../../GameObj/UI/ResultBackGround.h"
#include "../../GameObj/UI/ResultName.h"
#include "../../GameObj/UI/ResultWhoOni.h"

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace System;

ResultScene::ResultScene(unsigned __int8 oniResult)
	:m_oniResult(oniResult), m_changeFalg(false), m_changeTime(0)
{
}

ResultScene::~ResultScene()
{
}

void ResultScene::Start()
{
	m_pAdx2le = ADX2Le::GetInstance();
	m_pAdx2le->LoadAcb(L"ResultSceneCue.acb", L"ResultSceneCue.awb");
	m_pAdx2le->Play(0);

	// 背景
	ResultBackGround* pResultBackGround = new ResultBackGround;
	Task::TaskManager::AddTask(GetThisTaskHandle(), pResultBackGround);
	// 終了方法を明記されたUI
	PleaseSpaceUI* pPleaseSpace = new PleaseSpaceUI(Vector2(800 / 2, 500));
	Task::TaskManager::AddTask(GetThisTaskHandle(), pPleaseSpace);
	// リザルトという名前の画像を表示
	ResultName* pResultName = new ResultName;
	Task::TaskManager::AddTask(GetThisTaskHandle(), pResultName);

	// 誰が鬼だったか
	ResultWhoOni* pResultWhoOni = new ResultWhoOni(m_oniResult);
	Task::TaskManager::AddTask(GetThisTaskHandle(), pResultWhoOni);
}

bool ResultScene::Update(float elapsedTime)
{
	// スペースキーを押してタイトルシーンに移動
	if (InputManager::GetInstance().GetKeyTracker().IsKeyPressed(Keyboard::Space)) {
		if (m_changeFalg != true)
		{
			m_pAdx2le->Play(1);
		}
		m_changeFalg = true;
	}

	// シーン移行する際にクールタイムをカウント
	if (m_changeFalg == true)
	{
		m_changeTime++;
	}


	// 240 == 4秒経過したらシーン移行
	if (m_changeTime > 120)
	{
		TitleScene* pPlay = new TitleScene();
		Task::TaskManager::AddTask(pPlay, 0);
		return false;
	}
	return true;
}

void ResultScene::Draw()
{
}

void ResultScene::DrawBegin()
{
	System::DrawManager::GetInstance().Begin();
}

void ResultScene::DrawEnd()
{
	System::DrawManager::GetInstance().End();
}

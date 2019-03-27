#include "../pch.h"
#include "StartScene.h"
#include "../Scene/Title/TitleScene.h"

using namespace MyGame;

/// <summary>
/// コンストラクタ
/// </summary>
StartScene::StartScene()
{

}

/// <summary>
/// デストラクタ
/// </summary>
StartScene::~StartScene()
{

}

/// <summary>
/// 
/// </summary>
void StartScene::Start()
{
	// タイトルシーンタスクを追加
	TitleScene* pTitle = new TitleScene();
	Task::TaskManager::AddTask(pTitle, 0);
}

/// <summary>
/// 更新
/// </summary>
/// <returns></returns>
bool StartScene::Update(float elapsedTime)
{
	// 自タスクの削除
	return false;
}

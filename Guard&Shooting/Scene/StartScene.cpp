#include "../pch.h"
#include "StartScene.h"
#include "../Scene/Title/TitleScene.h"

using namespace MyGame;

/// <summary>
/// �R���X�g���N�^
/// </summary>
StartScene::StartScene()
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
StartScene::~StartScene()
{

}

/// <summary>
/// 
/// </summary>
void StartScene::Start()
{
	// �^�C�g���V�[���^�X�N��ǉ�
	TitleScene* pTitle = new TitleScene();
	Task::TaskManager::AddTask(pTitle, 0);
}

/// <summary>
/// �X�V
/// </summary>
/// <returns></returns>
bool StartScene::Update(float elapsedTime)
{
	// ���^�X�N�̍폜
	return false;
}

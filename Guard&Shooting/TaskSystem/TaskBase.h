#pragma once
#include "../../pch.h"
#include "TaskManager.h"
#include "../StepTimer.h"

namespace Task
{
	class TaskBase
	{
	public:	// 基本処理

		TaskBase();

		virtual ~TaskBase();

		// 開始処理
		virtual void Start() = 0;

		// 更新
		virtual bool Update(float elapsedTime) = 0;
		/*virtual bool Update(DX::StepTimer timer) = 0;*/

		virtual bool LastUpdate() { return true; };

		// 2DSprite描画
		virtual void Draw() {};
		virtual void DrawBegin() {};
		virtual void DrawEnd() {};

		// タスクハンドルの設定
		void SetThisTaskHandle(TaskHandle hTask)
		{
			mhThisTask = hTask;
		}

		// タスクハンドルの取得
		TaskHandle GetThisTaskHandle()
		{
			return mhThisTask;
		}
	private:
		// タスクハンドル
		TaskHandle mhThisTask;
	};
}
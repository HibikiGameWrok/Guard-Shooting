#pragma once
#include "../TaskSystem/TaskBase.h"

namespace MyGame
{
	class StartScene : public Task::TaskBase
	{
	public:
		StartScene();
		~StartScene();
		// 開始処理
		void Start();

		// 更新
		bool Update(float elapsedTime);
	};
}
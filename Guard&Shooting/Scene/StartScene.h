#pragma once
#include "../TaskSystem/TaskBase.h"

namespace MyGame
{
	class StartScene : public Task::TaskBase
	{
	public:
		StartScene();
		~StartScene();
		// �J�n����
		void Start();

		// �X�V
		bool Update(float elapsedTime);
	};
}
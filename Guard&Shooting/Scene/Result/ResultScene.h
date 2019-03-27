#pragma once
#include "../../ADX2/ADX2Le.h"
#include "../../Manager/DrawManager.h"
#include "../../Manager/InputManager.h"
#include "../../TaskSystem/TaskBase.h"
#include "../../StepTimer.h"

class ResultScene : public Task::TaskBase
{
private:
	MyLibrary::ADX2Le *m_pAdx2le;
	unsigned __int8 m_oniResult;
	unsigned int m_changeTime;
	bool m_changeFalg;
public:
	ResultScene(unsigned __int8 oniResult);
	~ResultScene();
	// 開始処理
	void Start() override;

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Draw() override;
	void DrawBegin() override;
	void DrawEnd() override;

};

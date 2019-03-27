//
//	File	 : CollisionManager.h
//
//  Contents : 衝突時の管理クラス宣言
//
//	name	 : Hibiki Yoshiyasu
//
#pragma once

#include "../../TaskSystem/TaskBase.h"

class CollisionManager : public Task::TaskBase
{

	// 開始処理
	void Start() override;
	// 更新
	bool Update(float elapsedTime);


};
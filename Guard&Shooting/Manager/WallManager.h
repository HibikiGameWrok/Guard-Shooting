#pragma once
#include <vector>
#include <fstream>
#include <sstream>
#include "../TaskSystem/TaskBase.h"
#include "../GameObj/Wall/Plane/BoardWall.h"


class WallManager : public Task::TaskBase
{
private:
	enum Buff
	{
		X_POS = 0,
		Y_POS = 1,
		Z_POS = 2,
		DIR = 3,
		NUM,
	};

private:
	int m_countWall; // 生成したい壁の数
	const wchar_t* m_pCsvFile; // .csvファイル
	BoardWall* m_pBwall;


	// 四角型当たり判定の半径設定
	// 横向きの壁
	DirectX::SimpleMath::Vector3 m_boxRadius_FB;
	// 縦向きの壁
	DirectX::SimpleMath::Vector3 m_boxRadius_LR;
public:
	WallManager(int count, const wchar_t* csvFileName);
	~WallManager();

	// 初期化
	void Start();
	// 更新処理
	bool Update(float elapsedTime);
	// 描画
	void Draw();

	void LoadWallData();
};
#include "../pch.h"
#include "WallManager.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// コンストラクタ
/// </summary>
WallManager::WallManager(int count, const wchar_t* csvFileName)
	:m_countWall(count), m_pCsvFile(csvFileName)
	, m_pBwall(nullptr)
	, m_boxRadius_FB(Vector3(0.15f, 0.15f, 0.05f)), m_boxRadius_LR(Vector3(0.05f, 0.15f, 0.15f))
{
}

/// <summary>
/// デストラクタ
/// </summary>
WallManager::~WallManager()
{
}

/// <summary>
/// 開始処理
/// </summary>
void WallManager::Start()
{
	// .csvで壁情報を読み込む
	LoadWallData();
}

/// <summary>
/// 更新処理
/// </summary>
/// <returns>更新しているかどうか</returns>
bool WallManager::Update(float elapsedTime)
{
	return true;
}

/// <summary>
/// 描画処理
/// </summary>
void WallManager::Draw()
{

}

/// <summary>
/// .csvの壁情報を読み込み
/// </summary>
void WallManager::LoadWallData()
{
	// ファイルのオープン
	ifstream ifs(m_pCsvFile);
	// 行を保管する一時的変数
	string line;

	// 行の読み込み
	for (int lineNum = 0; getline(ifs, line); lineNum++)
	{
		// 行が生成数より大きかった場合
		if (lineNum >= m_countWall)
		{
			break;
		}

		// 1列づつ見る
		istringstream iss(line);
		// 1列を保管する一時的変数
		string lineBuf;

		// .csvの各列を保管する一時的変数
		float buf[NUM] = {};
		// 1行の各列の値を変数に保管
		for (int i = 0; getline(iss, lineBuf, ','); i++)
		{
			buf[i] = static_cast<float>(atof(lineBuf.c_str()));
		}

		// 壁を生成
		Task::TaskManager::AddTask(GetThisTaskHandle(), new BoardWall(Vector3(buf[Buff::X_POS], buf[Buff::Y_POS], buf[Buff::Z_POS]), buf[Buff::DIR]));
	}
}

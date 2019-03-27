#include "../pch.h"
#include "WallManager.h"

using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

/// <summary>
/// �R���X�g���N�^
/// </summary>
WallManager::WallManager(int count, const wchar_t* csvFileName)
	:m_countWall(count), m_pCsvFile(csvFileName)
	, m_pBwall(nullptr)
	, m_boxRadius_FB(Vector3(0.15f, 0.15f, 0.05f)), m_boxRadius_LR(Vector3(0.05f, 0.15f, 0.15f))
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
WallManager::~WallManager()
{
}

/// <summary>
/// �J�n����
/// </summary>
void WallManager::Start()
{
	// .csv�ŕǏ���ǂݍ���
	LoadWallData();
}

/// <summary>
/// �X�V����
/// </summary>
/// <returns>�X�V���Ă��邩�ǂ���</returns>
bool WallManager::Update(float elapsedTime)
{
	return true;
}

/// <summary>
/// �`�揈��
/// </summary>
void WallManager::Draw()
{

}

/// <summary>
/// .csv�̕Ǐ���ǂݍ���
/// </summary>
void WallManager::LoadWallData()
{
	// �t�@�C���̃I�[�v��
	ifstream ifs(m_pCsvFile);
	// �s��ۊǂ���ꎞ�I�ϐ�
	string line;

	// �s�̓ǂݍ���
	for (int lineNum = 0; getline(ifs, line); lineNum++)
	{
		// �s�����������傫�������ꍇ
		if (lineNum >= m_countWall)
		{
			break;
		}

		// 1��Â���
		istringstream iss(line);
		// 1���ۊǂ���ꎞ�I�ϐ�
		string lineBuf;

		// .csv�̊e���ۊǂ���ꎞ�I�ϐ�
		float buf[NUM] = {};
		// 1�s�̊e��̒l��ϐ��ɕۊ�
		for (int i = 0; getline(iss, lineBuf, ','); i++)
		{
			buf[i] = static_cast<float>(atof(lineBuf.c_str()));
		}

		// �ǂ𐶐�
		Task::TaskManager::AddTask(GetThisTaskHandle(), new BoardWall(Vector3(buf[Buff::X_POS], buf[Buff::Y_POS], buf[Buff::Z_POS]), buf[Buff::DIR]));
	}
}

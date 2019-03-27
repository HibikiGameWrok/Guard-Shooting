#include "../../pch.h"
#include "Draw3DManager.h"

using namespace System;
using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma region Constructor
// �R���X�g���N�^
Draw3DManager::Draw3DManager()
{
}
#pragma endregion 

#pragma region Destructor
// �f�X�g���N�^
Draw3DManager::~Draw3DManager()
{
}
#pragma endregion

#pragma region Initialize
// ������
void Draw3DManager::Initialize(ID3D11Device * pDevise, ID3D11DeviceContext * pContext)
{
	//�����o�[�ϐ��Ƀf�o�C�X�ƃR���e�L�X�g��ݒ�
	SetDevise(pDevise);
	SetContext(pContext);
}

void Draw3DManager::Initialize(DX::DeviceResources * deviceResources)
{
	//�����o�[�ϐ��Ƀf�o�C�X�ƃR���e�L�X�g��ݒ�
	SetDevise(deviceResources->GetD3DDevice());
	SetContext(deviceResources->GetD3DDeviceContext());
}
#pragma endregion

#pragma region Create
// �t�@�C���p�X�̐ݒ菈��
bool System::Draw3DManager::SetFilePath(std::unique_ptr<DirectX::Model>& modelFile, wchar_t * pEffectFile, wchar_t * pCmoFile)
{
	// �f�o�C�X�������̂Ŏ��s
	if (m_pDevice == NULL)
	{
		return false;
	}

	// �}�e���A���̃t�@�C���p�X��o�^
	EffectFactory effect(GetDevise());
	effect.SetDirectory(pEffectFile);

	modelFile = Model::CreateFromCMO(GetDevise(), pCmoFile, effect);

	return true;
}
#pragma endregion

#pragma region Draw
// �`�揈��
void System::Draw3DManager::Draw(std::unique_ptr<DirectX::Model>& modelFile)
{
	// �R�����X�e�[�g
	DirectX::CommonStates states(m_pDevice);

	modelFile->Draw(
		m_pContext,
		states,
		m_world,
		m_view,
		m_projection
	);
}

void System::Draw3DManager::Draw(std::unique_ptr<DirectX::Model>& modelFile, DirectX::SimpleMath::Matrix & world, std::function<void __cdecl()> setCustomState)
{
	// �R�����X�e�[�g
	DirectX::CommonStates states(m_pDevice);

	modelFile->Draw(
		m_pContext,
		states,
		world,
		m_view,
		m_projection,
		false,
		setCustomState
	);
}
#pragma endregion

#include "../../pch.h"
#include "Draw3DManager.h"

using namespace System;
using namespace DirectX;
using namespace DirectX::SimpleMath;

#pragma region Constructor
// コンストラクタ
Draw3DManager::Draw3DManager()
{
}
#pragma endregion 

#pragma region Destructor
// デストラクタ
Draw3DManager::~Draw3DManager()
{
}
#pragma endregion

#pragma region Initialize
// 初期化
void Draw3DManager::Initialize(ID3D11Device * pDevise, ID3D11DeviceContext * pContext)
{
	//メンバー変数にデバイスとコンテキストを設定
	SetDevise(pDevise);
	SetContext(pContext);
}

void Draw3DManager::Initialize(DX::DeviceResources * deviceResources)
{
	//メンバー変数にデバイスとコンテキストを設定
	SetDevise(deviceResources->GetD3DDevice());
	SetContext(deviceResources->GetD3DDeviceContext());
}
#pragma endregion

#pragma region Create
// ファイルパスの設定処理
bool System::Draw3DManager::SetFilePath(std::unique_ptr<DirectX::Model>& modelFile, wchar_t * pEffectFile, wchar_t * pCmoFile)
{
	// デバイスが無いので失敗
	if (m_pDevice == NULL)
	{
		return false;
	}

	// マテリアルのファイルパスを登録
	EffectFactory effect(GetDevise());
	effect.SetDirectory(pEffectFile);

	modelFile = Model::CreateFromCMO(GetDevise(), pCmoFile, effect);

	return true;
}
#pragma endregion

#pragma region Draw
// 描画処理
void System::Draw3DManager::Draw(std::unique_ptr<DirectX::Model>& modelFile)
{
	// コモンステート
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
	// コモンステート
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

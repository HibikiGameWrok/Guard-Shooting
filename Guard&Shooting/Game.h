// 
//	File	 : Game.h
//
//  Contents : IDeviceNotifyを継承したGameクラス
//
//	name	 : Hibiki Yoshiyasu

#pragma once

#include "DeviceResources.h"
#include "StepTimer.h"

#include "DebugCamera.h"
#include "GridFloor.h"

#include "ADX2\ADX2Le.h"

#include "GameObj\Model\ModelManager.h"

#include "GameObj\UI\JudgeUI.h"

#include "Camera\MyCamera.h"

#include "TaskSystem\TaskManager.h"

#include "Effect\Myeffect.h"
#include "Effect\EffectManager.h"

// A basic game implementation that creates a D3D11 device and
// provides a game loop.
class Game : public DX::IDeviceNotify
{
public:

    Game();
	~Game();
    // Initialization and management
    void Initialize(HWND window, int width, int height);

    // Basic game loop
    void Tick();

    // IDeviceNotify
    virtual void OnDeviceLost() override;
    virtual void OnDeviceRestored() override;

    // Messages
    void OnActivated();
    void OnDeactivated();
    void OnSuspending();
    void OnResuming();
    void OnWindowSizeChanged(int width, int height);

    // Properties
    void GetDefaultSize( int& width, int& height ) const;

#pragma region Getter

	// デバイスを取得する関数
	ID3D11Device* GetDevice() {
		return m_deviceResources->GetD3DDevice();
	}

	// コンテキストを取得する関数
	ID3D11DeviceContext* GetContext() {
		return m_deviceResources->GetD3DDeviceContext();
	}

	// ビュー行列を取得する関数
	const DirectX::SimpleMath::Matrix& GetView() {
		return m_view;
	}

	// 射影行列を取得する関数
	const DirectX::SimpleMath::Matrix& GetProjection() {
		return m_projection;
	}

private:
	// 更新処理
    void Update(DX::StepTimer const& timer);
	// 描画処理
    void Render();

	// クリア処理
    void Clear();

	// 作成処理
    void CreateDeviceDependentResources();
	// 画面作成処理
    void CreateWindowSizeDependentResources();

private:
    // Device resources.
    DX::DeviceResources*    m_deviceResources;

    // Rendering loop timer.
    DX::StepTimer                           m_timer;

	// キーボード
	std::unique_ptr<DirectX::Keyboard> m_keyboard;
	// キーボードトラッカー
	DirectX::Keyboard::KeyboardStateTracker m_tracker;

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	
	// 入力レイアウト 
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout; 
	
	// エフェクト
	std::unique_ptr<DirectX::AlphaTestEffect> m_batchEffect; 

	// プリミティブバッチ 
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_batch; 

private:
	// タスクマネージャ
	Task::TaskManager m_TaskManager;
	// モデルマネージャ
	ModelManager* m_modelManager;

	// エフェクトマネージャー
	EffectManager*	m_effectManager;
};
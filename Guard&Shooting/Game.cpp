//
//	File	 : Game.cpp
//
//  Contents : ゲーム全体の初期化、更新処理、描画処理etc...をする
//
//	name	 : Hibiki Yoshiyasu
//

#include "pch.h"
#include "Game.h"
#include "Manager\InputManager.h"
#include "Manager\DrawManager.h"
#include "Manager\Draw3DManager.h"
#include "Scene\StartScene.h"

#pragma warning( disable: 4099 )

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

extern void ExitGame();

using namespace MyLibrary;
using namespace DirectX;
using namespace DirectX::SimpleMath;

using Microsoft::WRL::ComPtr;

/// <summary>
/// コンストラクタ
/// </summary>
Game::Game()
{
    m_deviceResources = &DX::DeviceResources::GetInstance();
    m_deviceResources->RegisterDeviceNotify(this);
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{
	// サウンドメモリ解放
	ADX2Le *adx2le = ADX2Le::GetInstance();
	adx2le->Finalize();
	// 全てのタスクを消す
	Task::TaskManager::RemoveTaskAll();
	m_TaskManager.CleanTask(true);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

	// 入力クラスの更新
	System::InputManager::GetInstance().Update();

	// DrawManager初期化
	System::DrawManager::GetInstance().Initialize(m_deviceResources->GetD3DDevice(), m_deviceResources->GetD3DDeviceContext());

	// Draw3DManager初期化
	System::Draw3DManager::GetInstance().Initialize(m_deviceResources);

	// 自作タスクシーン登録
	MyGame::StartScene* pStartTask = new MyGame::StartScene();
	Task::TaskManager::AddTask(pStartTask);
	
	// サウンド初期化
	ADX2Le *adx2le = ADX2Le::GetInstance();
	adx2le->Initialize(L"BallOni.acf");
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&](){
		Update(m_timer); 
	});

    Render();

	// キー入力
	Keyboard::State kb = Keyboard::Get().GetState();

	if (kb.Escape)
	{
		ExitGame();
		OnDeviceLost();
	}
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
	float elapsedTime = float(timer.GetElapsedSeconds());

	// TODO: Add your game logic here.
	elapsedTime;
	float time = float(timer.GetTotalSeconds());

	// 入力クラスの更新
	System::InputManager::GetInstance().Update();

	// プレイヤーの操作
	// キー入力
	Keyboard::State kb = Keyboard::Get().GetState();
	// トリガー
	m_tracker.Update(kb);

	// サウンド更新
	ADX2Le *adx2le = ADX2Le::GetInstance();
	adx2le->Update();

	// タスクマネージャの更新
	m_TaskManager.Update(time);

	// 追加予約されたタスクを正式に追加
	m_TaskManager.AddReserve();

	// 管理タスクがない（削除してもいいタスク）の削除
	m_TaskManager.CleanTask();
}


#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}
	Clear();
	m_deviceResources->PIXBeginEvent(L"Render");
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();	
	
	System::Draw3DManager::GetInstance().SetProjection(m_projection);

	// タスクマネージャの描画
	m_TaskManager.Draw();

	m_deviceResources->PIXEndEvent();
	// Show the new frame.
	m_deviceResources->Present();

}
#pragma endregion

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::DarkBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 800;
    height = 600;
}
#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();

	// TODO: Initialize device dependent objects here (independent of window size).
	device;

	//-------------------------------------------------------
	// エフェクトの作成
	m_batchEffect = std::make_unique<AlphaTestEffect>(device);
	m_batchEffect->SetAlphaFunction(D3D11_COMPARISON_EQUAL); // EQUALは等しい
	m_batchEffect->SetReferenceAlpha(255); // アルファ値を参照

	// 入力レイアウト生成 
	void const* shaderByteCode;
	size_t byteCodeLength;
	m_batchEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	device->CreateInputLayout(VertexPositionTexture::InputElements, VertexPositionTexture::InputElementCount,
		shaderByteCode, byteCodeLength, m_inputLayout.GetAddressOf());

	// プリミティブバッチの作成 
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionTexture>>(context);

#pragma region "L字の壁"
//#pragma region "当たり判定"
//	m_box_lWall[0].c = Vector3(-1.5f,0.15f,0.0f);
//	m_box_lWall[0].r = Vector3(0.05f,0.15f,0.2f);
//
//	m_box_lWall[1].c = Vector3(0.0f, 0.15f, -1.5f);
//	m_box_lWall[1].r = Vector3(0.2f, 0.15f, 0.05f);
#pragma endregion
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.
	ID3D11Device* device = m_deviceResources->GetD3DDevice();
	ID3D11DeviceContext* context = m_deviceResources->GetD3DDeviceContext();
	// ウインドウサイズからアスペクト比を算出する
	RECT size = m_deviceResources->GetOutputSize();
	float aspectRatio = float(size.right) / float(size.bottom);
	// 画角を設定
	float fovAngleY = XMConvertToRadians(45.0f);
		
	// 射影行列を作成する
	m_projection = Matrix::CreatePerspectiveFieldOfView(
		fovAngleY,
		aspectRatio,
		0.01f,
		1000.0f
	);
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion


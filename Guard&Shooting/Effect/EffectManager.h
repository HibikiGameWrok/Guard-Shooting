#pragma once

#include "../DeviceResources.h"
#include "../StepTimer.h"
#include <SimpleMath.h>
#include "MyEffect.h"
#include "Model.h"
#include "../TaskSystem/TaskBase.h"

#include <list>

class EffectManager : public Task::TaskBase
{
public:
	static const float SCREEN_W;
	static const float SCREEN_H;

private:
	// 出すエフェクトの数を管理
	std::list<MyEffect*>		m_effectList;


	// 画像のデータ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_a;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_b;


	DX::DeviceResources*					m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	std::unique_ptr<DirectX::CommonStates>	m_states;


	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;


	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::vector<DirectX::VertexPositionColorTexture>  m_vertex;


	// カメラベクトル
	DirectX::SimpleMath::Vector3 m_camera;
	// ワールド行列
	DirectX::SimpleMath::Matrix m_world;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;


	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	// エフェクトの変化をしる為の変数
	DirectX::SimpleMath::Matrix m_billBoard;

	// 更新時間
	float m_timer;
	// 色を変化しない
	float m_color;

public:
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

public:
	EffectManager();
	~EffectManager();

public:
	void Start();

	void CreateA(int count, const wchar_t* effectFileName);
	void CreateAB(int count, const wchar_t* effectFileName_a, const wchar_t* effectFileName_b);

	void Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir);
	bool Update(float elapsedTime);
	
	void Render();
	void Lost();

	void SetRenderState(DirectX::SimpleMath::Vector3 cameraEye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);

	void Draw();
	void Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
};
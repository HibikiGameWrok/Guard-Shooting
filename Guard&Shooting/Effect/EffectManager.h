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
	// �o���G�t�F�N�g�̐����Ǘ�
	std::list<MyEffect*>		m_effectList;


	// �摜�̃f�[�^
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_a;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture_b;


	DX::DeviceResources*					m_deviceResources;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;
	std::unique_ptr<DirectX::CommonStates>	m_states;


	// ���_�V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	// �s�N�Z���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	// �W�I���g���V�F�[�_
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_GeometryShader;


	// �v���~�e�B�u�o�b�`
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_batch;
	// ���̓��C�A�E�g
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	std::vector<DirectX::VertexPositionColorTexture>  m_vertex;


	// �J�����x�N�g��
	DirectX::SimpleMath::Vector3 m_camera;
	// ���[���h�s��
	DirectX::SimpleMath::Matrix m_world;
	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;
	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;


	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_tex;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_rtv;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_srv;

	// �G�t�F�N�g�̕ω�������ׂ̕ϐ�
	DirectX::SimpleMath::Matrix m_billBoard;

	// �X�V����
	float m_timer;
	// �F��ω����Ȃ�
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
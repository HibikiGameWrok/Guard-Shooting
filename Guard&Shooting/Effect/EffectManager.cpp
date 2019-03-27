#include "../pch.h"
#include "EffectManager.h"
#include "../Manager/Draw3DManager.h"
#include <WICTextureLoader.h>
#include "../BinaryFile.h"
#include "d3d11.h"
#include <Effects.h>
#include <Model.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;

const float EffectManager::SCREEN_W = 800;
const float EffectManager::SCREEN_H = 600;

const std::vector<D3D11_INPUT_ELEMENT_DESC> EffectManager::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(Vector3) + sizeof(Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/// <summary>
/// �R���X�g���N�^
/// </summary>
EffectManager::EffectManager()
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
EffectManager::~EffectManager()
{
	Lost();
}

/// <summary>
/// �J�n�����֐�
/// </summary>
void EffectManager::Start()
{
	m_timer = 0;
}

/// <summary>
/// �G�t�F�N�g���������֐�
/// </summary>
/// <param name="count">������</param>
/// <param name="effectFileName">�t�@�C���p�X</param>
void EffectManager::CreateA(int count, const wchar_t * effectFileName)
{
	m_deviceResources = &DX::DeviceResources::GetInstance();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name = effectFileName;
	DirectX::CreateWICTextureFromFile(device, name, nullptr, m_texture_a.GetAddressOf());

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	for (int i = 0; i < count; i++) {
		MyEffect* effect = new MyEffect();
		m_effectList.push_back(effect);
	}
}

/// <summary>
/// �G�t�F�N�g���������֐�
/// </summary>
/// <param name="count">������</param>
/// <param name="effectFileName_a">�t�@�C���p�XA</param>
/// <param name="effectFileName_b">�t�@�C���p�XB</param>
void EffectManager::CreateAB(int count, const wchar_t * effectFileName_a, const wchar_t * effectFileName_b)
{
	m_deviceResources = &DX::DeviceResources::GetInstance();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name_a = effectFileName_a;
	const wchar_t* name_b = effectFileName_b;
	DirectX::CreateWICTextureFromFile(device, name_a, nullptr, m_texture_a.GetAddressOf());
	DirectX::CreateWICTextureFromFile(device, name_b, nullptr, m_texture_b.GetAddressOf());

	// �R���p�C�����ꂽ�V�F�[�_�t�@�C����ǂݍ���
	BinaryFile	VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile	GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS_Title.cso");
	BinaryFile	PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS_Title.cso");

	// ���C�A�E�g�𐶐�
	device->CreateInputLayout(&INPUT_LAYOUT[0],	INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// ���_�V�F�[�_�쐬
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// �W�I���g���V�F�[�_�쐬
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// �s�N�Z���V�F�[�_�쐬
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// �G���[
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// �v���~�e�B�u�o�b�`�̍쐬
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionColorTexture>>(m_deviceResources->GetD3DDeviceContext());

	m_states = std::make_unique<CommonStates>(device);

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	device->CreateBuffer(&bd, nullptr, &m_CBuffer);

	for (int i = 0; i < count; i++) {
		MyEffect* effect = new MyEffect();
		m_effectList.push_back(effect);
	}
}

/// <summary>
/// �G�t�F�N�g�����������֐�
/// </summary>
void EffectManager::Lost() {
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		delete (*itr);
	}
	m_effectList.clear();
}

/// <summary>
/// �������֐�
/// </summary>
/// <param name="life">��������</param>
/// <param name="pos">���W</param>
/// <param name="dir">�p�x</param>
void EffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir)
{
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		// �p�x������x�N�g���ɕϊ�
		Vector3 vel = dir;
		// �����x�N�g���𐳋K��
		vel.Normalize(); 

		// �����_���̕���
		vel *= static_cast<float>(sin(rand())) + 1.0f;
		Vector3 raff = Vector3(vel.y, -vel.x, 0);
		raff *= static_cast<float>(sin(rand()))*0.3f;
		vel = vel + raff;

		// ���x��^����
		vel *= 0.1f;

		// �e�G�t�F�N�g��������
		(*itr)->Initialize(life, pos, vel);
	}
}

/// <summary>
/// �X�V�����֐�
/// </summary>
/// <param name="elapsedTime">�X�V����</param>
/// <returns>true == �X�V���� false == �X�V���Ȃ�</returns>
bool EffectManager::Update(float elapsedTime)
{
	// �X�V����
	/*m_timer = elapsedTime;*/
	// (��)��������0.02f���Z
	m_timer += 0.02f;

	// �������ꂽ�e�G�t�F�N�g�̍X�V
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		(*itr)->Update(m_timer);
	}
	return true;
}

/// <summary>
/// �`�揈���֐�
/// </summary>
void EffectManager::Render()
{
	m_vertex.clear();
	Vector3 wPos;

	for (auto itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		wPos = (*itr)->GetPosition();
		Vector3 vel = (*itr)->GetVelocity();
		{
			VertexPositionColorTexture vertex;
			vertex = VertexPositionColorTexture(Vector3::Zero, Vector4(vel.x,vel.y,vel.z,1), Vector2(0.0f,3.0f));
			m_vertex.push_back(vertex);
		}
	}
	// �r���{�[�h��(�o��������W�A�J�����̍��W�A�J�����̌���)
	m_world = Matrix::CreateBillboard(wPos, m_camera, Vector3::Up);
	// �`��
	Draw(m_world,m_view, m_proj);
}

/// <summary>
/// �`�悷��ׂ̏���ݒ肷��֐�
/// </summary>
/// <param name="cameraEye">�r���{�[�h�ɋ����錻�݂̃J�����̍��W</param>
/// <param name="view">�J�������</param>
/// <param name="proj">��ʏ��</param>
void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 cameraEye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = cameraEye;
	m_view = view;
	m_proj = proj;
}

/// <summary>
/// �^�X�N�̕`��֐�
/// </summary>
void EffectManager::Draw()
{
	Render();
}

/// <summary>
/// �G�t�F�N�g�}�l�[�W���[
/// </summary>
/// <param name="world">���[���h�s��</param>
/// <param name="view">�r���[�s��</param>
/// <param name="proj">�ˉe�s��</param>
void EffectManager::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();
 
	ConstBuffer cbuff;

	// �`��}�l�[�W���[�̃r���[�s��ɐݒ�
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	
	cbuff.Diffuse = Vector4(cosf(m_timer), m_timer, m_color, 1);

	/*cbuff.time = cosf(m_timer);*/ //�R�T�C���̒l�������O�`�P�̒l�ɂ��Ȃ���΂Ȃ�Ȃ�
	//cbuff.Diffuse = Vector4(m_position.x, m_position.y, m_position.z, 1);

	//�萔�o�b�t�@�̓��e�X�V
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// �������菈��
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// �[�x�o�b�t�@�ɎQ�Ƃ���
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// �J�����O�͍�����
	context->RSSetState(m_states->CullNone());

	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	ID3D11SamplerState* sampler[1] = { m_states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);
	context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_GeometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_PixelShader.Get(), nullptr, 0);

	//�@�摜��ǂݍ���
	// 0 = 0�Ԗڂɓǂݍ���
	context->PSSetShaderResources(0, 1, m_texture_a.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture_b.GetAddressOf());

	context->IASetInputLayout(m_inputLayout.Get());
	
	// ������������`��
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_batch->End();
	
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
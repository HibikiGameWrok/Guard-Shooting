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
/// コンストラクタ
/// </summary>
EffectManager::EffectManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
EffectManager::~EffectManager()
{
	Lost();
}

/// <summary>
/// 開始処理関数
/// </summary>
void EffectManager::Start()
{
	m_timer = 0;
}

/// <summary>
/// エフェクト生成処理関数
/// </summary>
/// <param name="count">生成数</param>
/// <param name="effectFileName">ファイルパス</param>
void EffectManager::CreateA(int count, const wchar_t * effectFileName)
{
	m_deviceResources = &DX::DeviceResources::GetInstance();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name = effectFileName;
	DirectX::CreateWICTextureFromFile(device, name, nullptr, m_texture_a.GetAddressOf());

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	device->CreateInputLayout(&INPUT_LAYOUT[0],
		INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());
	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチの作成
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
/// エフェクト生成処理関数
/// </summary>
/// <param name="count">生成数</param>
/// <param name="effectFileName_a">ファイルパスA</param>
/// <param name="effectFileName_b">ファイルパスB</param>
void EffectManager::CreateAB(int count, const wchar_t * effectFileName_a, const wchar_t * effectFileName_b)
{
	m_deviceResources = &DX::DeviceResources::GetInstance();
	auto device = m_deviceResources->GetD3DDevice();

	const wchar_t* name_a = effectFileName_a;
	const wchar_t* name_b = effectFileName_b;
	DirectX::CreateWICTextureFromFile(device, name_a, nullptr, m_texture_a.GetAddressOf());
	DirectX::CreateWICTextureFromFile(device, name_b, nullptr, m_texture_b.GetAddressOf());

	// コンパイルされたシェーダファイルを読み込み
	BinaryFile	VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile	GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS_Title.cso");
	BinaryFile	PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS_Title.cso");

	// レイアウトを生成
	device->CreateInputLayout(&INPUT_LAYOUT[0],	INPUT_LAYOUT.size(),
		VSData.GetData(), VSData.GetSize(),
		m_inputLayout.GetAddressOf());

	// 頂点シェーダ作成
	if (FAILED(device->CreateVertexShader(VSData.GetData(), VSData.GetSize(), NULL, m_VertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}
	// ジオメトリシェーダ作成
	if (FAILED(device->CreateGeometryShader(GSData.GetData(), GSData.GetSize(), NULL, m_GeometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(device->CreatePixelShader(PSData.GetData(), PSData.GetSize(), NULL, m_PixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// プリミティブバッチの作成
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
/// エフェクトを消す処理関数
/// </summary>
void EffectManager::Lost() {
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		delete (*itr);
	}
	m_effectList.clear();
}

/// <summary>
/// 初期化関数
/// </summary>
/// <param name="life">生存時間</param>
/// <param name="pos">座標</param>
/// <param name="dir">角度</param>
void EffectManager::Initialize(float life, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Vector3 dir)
{
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		// 角度を方向ベクトルに変換
		Vector3 vel = dir;
		// 方向ベクトルを正規化
		vel.Normalize(); 

		// ランダムの方向
		vel *= static_cast<float>(sin(rand())) + 1.0f;
		Vector3 raff = Vector3(vel.y, -vel.x, 0);
		raff *= static_cast<float>(sin(rand()))*0.3f;
		vel = vel + raff;

		// 速度を与える
		vel *= 0.1f;

		// 各エフェクトを初期化
		(*itr)->Initialize(life, pos, vel);
	}
}

/// <summary>
/// 更新処理関数
/// </summary>
/// <param name="elapsedTime">更新時間</param>
/// <returns>true == 更新する false == 更新しない</returns>
bool EffectManager::Update(float elapsedTime)
{
	// 更新時間
	/*m_timer = elapsedTime;*/
	// (仮)生成時に0.02f加算
	m_timer += 0.02f;

	// 生成された各エフェクトの更新
	for (std::list<MyEffect*>::iterator itr = m_effectList.begin(); itr != m_effectList.end(); itr++)
	{
		(*itr)->Update(m_timer);
	}
	return true;
}

/// <summary>
/// 描画処理関数
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
	// ビルボード化(出現する座標、カメラの座標、カメラの向き)
	m_world = Matrix::CreateBillboard(wPos, m_camera, Vector3::Up);
	// 描画
	Draw(m_world,m_view, m_proj);
}

/// <summary>
/// 描画する為の情報を設定する関数
/// </summary>
/// <param name="cameraEye">ビルボードに教える現在のカメラの座標</param>
/// <param name="view">カメラ情報</param>
/// <param name="proj">画面情報</param>
void EffectManager::SetRenderState(DirectX::SimpleMath::Vector3 cameraEye, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_camera = cameraEye;
	m_view = view;
	m_proj = proj;
}

/// <summary>
/// タスクの描画関数
/// </summary>
void EffectManager::Draw()
{
	Render();
}

/// <summary>
/// エフェクトマネージャー
/// </summary>
/// <param name="world">ワールド行列</param>
/// <param name="view">ビュー行列</param>
/// <param name="proj">射影行列</param>
void EffectManager::Draw(DirectX::SimpleMath::Matrix world, DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();
 
	ConstBuffer cbuff;

	// 描画マネージャーのビュー行列に設定
	cbuff.matView = view.Transpose();
	cbuff.matProj = proj.Transpose();
	cbuff.matWorld = world.Transpose();
	
	cbuff.Diffuse = Vector4(cosf(m_timer), m_timer, m_color, 1);

	/*cbuff.time = cosf(m_timer);*/ //コサインの値をだす０～１の値にしなければならない
	//cbuff.Diffuse = Vector4(m_position.x, m_position.y, m_position.z, 1);

	//定数バッファの内容更新
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);
	ID3D11BlendState* blendstate = m_states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);
	// 深度バッファに参照する
	context->OMSetDepthStencilState(m_states->DepthRead(), 0);
	// カリングは左周り
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

	//　画像を読み込む
	// 0 = 0番目に読み込む
	context->PSSetShaderResources(0, 1, m_texture_a.GetAddressOf());
	context->PSSetShaderResources(1, 1, m_texture_b.GetAddressOf());

	context->IASetInputLayout(m_inputLayout.Get());
	
	// 半透明部分を描画
	m_batch->Begin();
	m_batch->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertex[0], m_vertex.size());
	m_batch->End();
	
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}
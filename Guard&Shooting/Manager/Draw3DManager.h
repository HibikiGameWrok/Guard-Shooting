//・・・・・・・・・・・・・・・・・・・・
//	ファイル名：Draw3DManager.h
//
//　作成日：2018/12/05(水曜日)
//	制作者：吉安響
//	
//	内容　：3Dモデルを描画する為のデータ構造体とシングルトンクラス
//
//・・・・・・・・・・・・・・・・・・・・

#pragma once
#include "../../DeviceResources.h"
#include "../Utility/SingletonBase.h"
#include "../Utility/Flag.h"

namespace System
{
	// 描画をする為のシングルトンクラス
	class Draw3DManager : public Utility::SingletonBase<Draw3DManager>
	{
	public:
		friend class Utility::SingletonBase<Draw3DManager>;

	private: // 変数
		// デバイス
		ID3D11Device* m_pDevice;
		// コンテキスト
		ID3D11DeviceContext* m_pContext;

		// ワールド座標
		DirectX::SimpleMath::Matrix m_world;
		// ビュー座標
		DirectX::SimpleMath::Matrix m_view;
		// 射影座標
		DirectX::SimpleMath::Matrix m_projection;

	protected: // コンストラクタ&デストラクタ
		Draw3DManager();
		~Draw3DManager();

	public: // 基本処理

		// 初期化処理
		void Initialize(ID3D11Device* pDevise, ID3D11DeviceContext* pContext);
		void Initialize(DX::DeviceResources* deviceResources);
	
		// ファイルパスの設定処理
		bool SetFilePath(std::unique_ptr<DirectX::Model>& modelFile, wchar_t* pEffectFile, wchar_t* pCmoFile);

		// 描画処理
		void Draw(std::unique_ptr<DirectX::Model>& modelFile);
		/*void Draw(std::unique_ptr<DirectX::Model>& modelFile, DirectX::SimpleMath::Matrix& world);*/
		void Draw(std::unique_ptr<DirectX::Model>& modelFile, DirectX::SimpleMath::Matrix& world, std::function<void __cdecl()> setCustomState = nullptr);

	public: // 設定関数&取得関数

		// デバイスの設定&取得
		void SetDevise(ID3D11Device* pDevise){ m_pDevice = pDevise;	}
		ID3D11Device* GetDevise() { return m_pDevice; }

		// コンテキストの設定&取得
		void SetContext(ID3D11DeviceContext* pContext) { m_pContext = pContext;	}
		ID3D11DeviceContext* GetContext() { return m_pContext; }

		// ワールド座標の設定&取得
		void SetWorld(DirectX::SimpleMath::Matrix& world) { m_world = world; }
		DirectX::SimpleMath::Matrix GetWorld() { return m_world; }

		// ビュー座標の設定&取得
		void SetView(DirectX::SimpleMath::Matrix view) { m_view = view;	}
		DirectX::SimpleMath::Matrix GetView() { return m_view; }

		// 射影座標の設定&取得
		void SetProjection(DirectX::SimpleMath::Matrix projection) { m_projection = projection;	}
		DirectX::SimpleMath::Matrix GetProjection() { return m_projection; }
	};
}
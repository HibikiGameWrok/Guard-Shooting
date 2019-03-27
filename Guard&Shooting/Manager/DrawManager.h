#pragma once

#include <list>

#include "../Utility/SingletonBase.h"

#include "WICTextureLoader.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"

#include "../Utility/Flag.h"

namespace System
{
	typedef ID3D11ShaderResourceView* DxTexture;

	// 描画データをまとめた構造体
	struct DrawData
	{
	private:
		enum eDrawManagerFlag
		{
			NOT_TEXTURE_DELETE = (1<<0)
		};
		// フラグ
		Utility::Flag mFlag;
		// テクスチャ
		DxTexture mpTexture;
		// 座標
		DirectX::SimpleMath::Vector2 mPos;
		// 角度
		float mRot;
		// 切り取り情報
		RECT mRect;
		// 色
		DirectX::SimpleMath::Vector4 mColor;
		// 中心点
		DirectX::XMFLOAT2 mOrigin;
		// スケール
		DirectX::XMFLOAT2 mScale;

	public:
		// コンストラクタ
		DrawData()
			: mpTexture(NULL)
			, mPos()
			, mRot(0.0f)
			, mRect()
			, mColor(1.0f, 1.0f, 1.0f, 1.0f)
			, mOrigin(0.0f,0.0f)
			, mScale(1.0f, 1.0f)
		{
		}
		// デストラクタ
		~DrawData()
		{
			if (mpTexture == NULL)
			{
				return;
			}

			// フラグによる削除制限
			if (mFlag.Is(eDrawManagerFlag::NOT_TEXTURE_DELETE))
			{
				return;
			}

			mpTexture->Release();
		}

	public: // getter
		// テクスチャの取得
		DxTexture* GetTexture() { return &mpTexture; }
		// 座標の取得
		DirectX::SimpleMath::Vector2 GetPos() { return mPos; }
		// 回転度の取得
		float GetRot() { return mRot; }
		// 表示範囲の取得
		RECT GetRect() { return mRect; }
		const RECT* GetRectPtn() { return &mRect; }
		// 色の取得
		DirectX::SimpleMath::Vector4 GetColor() { return mColor; }
		// 中心の取得
		DirectX::XMFLOAT2 GetOrigin() { return mOrigin; }
		// 大きさの取得
		DirectX::XMFLOAT2 GetScale() { return  mScale; }
	public: // setter
		// テクスチャの設定
		void SetTexture(DxTexture pTexture) {
			mpTexture = pTexture;
			mFlag.On(eDrawManagerFlag::NOT_TEXTURE_DELETE);
		}
		// 座標の設定
		void SetPos(float x, float y) { mPos.x = x; mPos.y = y; }
		void SetPos(DirectX::SimpleMath::Vector2 pos) { mPos = pos; }
		// 回転度の設定
		void SetRot(float r) { mRot = r; }
		// 表示範囲の設定
		void SetRect(LONG left, LONG top, LONG right, LONG bottom) {
			mRect.left = left;
			mRect.top = top;
			mRect.right = right;
			mRect.bottom = bottom;
		}
		void SetRect(LONG right, LONG bottom) { SetRect(0, 0, right, bottom); }
		// 色の設定
		void SetColor(DirectX::SimpleMath::Vector4 color) { mColor = color; }
		// 中心の設定
		void SetOrigin(float x, float y) { mOrigin.x = x; mOrigin.y = y; }
		void SetOrigin(DirectX::SimpleMath::Vector2 origin) { mOrigin = origin; }
		// 大きさの設定
		void SetScale(float x, float y) { mScale.x = x; mScale.y = y; }
		void SetScale(DirectX::SimpleMath::Vector2 scale) { mScale = scale; }
	};


	class DrawManager : public Utility::SingletonBase<DrawManager>
	{
	public:
		friend class Utility::SingletonBase<DrawManager>;

	private:
		// デバイス
		ID3D11Device* mpDevice;
		// スプライトバッチ
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	protected:
		// コンストラクタ
		DrawManager() {};
		// デストラクタ
		~DrawManager() {};

	public:
		// 初期化処理
		void Initialize(ID3D11Device* pDevise, ID3D11DeviceContext* pContext);
		// 描画
		void Draw(DrawData& data);
		// 描画前後処理
		void Begin();
		void End();
		// テクスチャ読み込み
		bool LoadTexture(DrawData& data, wchar_t* pFileName);
		bool LoadTexture(DrawData& data, const wchar_t* pFileName);
		bool LoadTexture(DxTexture& pTexture, wchar_t* pFileName);
		bool LoadTexture(DxTexture& pTexture, const wchar_t* pFileName);
		// テクスチャ削除
		/*void DeleteTexture(DrawData& pData);*/
		// デバイスの設定
		void SetDevise(ID3D11Device* pDevise)
		{
			mpDevice = pDevise;
		}
		// デバイスの取得
		ID3D11Device* GetDevise()
		{
			return mpDevice;
		}
	};
}
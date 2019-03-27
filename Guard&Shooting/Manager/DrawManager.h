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

	// �`��f�[�^���܂Ƃ߂��\����
	struct DrawData
	{
	private:
		enum eDrawManagerFlag
		{
			NOT_TEXTURE_DELETE = (1<<0)
		};
		// �t���O
		Utility::Flag mFlag;
		// �e�N�X�`��
		DxTexture mpTexture;
		// ���W
		DirectX::SimpleMath::Vector2 mPos;
		// �p�x
		float mRot;
		// �؂�����
		RECT mRect;
		// �F
		DirectX::SimpleMath::Vector4 mColor;
		// ���S�_
		DirectX::XMFLOAT2 mOrigin;
		// �X�P�[��
		DirectX::XMFLOAT2 mScale;

	public:
		// �R���X�g���N�^
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
		// �f�X�g���N�^
		~DrawData()
		{
			if (mpTexture == NULL)
			{
				return;
			}

			// �t���O�ɂ��폜����
			if (mFlag.Is(eDrawManagerFlag::NOT_TEXTURE_DELETE))
			{
				return;
			}

			mpTexture->Release();
		}

	public: // getter
		// �e�N�X�`���̎擾
		DxTexture* GetTexture() { return &mpTexture; }
		// ���W�̎擾
		DirectX::SimpleMath::Vector2 GetPos() { return mPos; }
		// ��]�x�̎擾
		float GetRot() { return mRot; }
		// �\���͈͂̎擾
		RECT GetRect() { return mRect; }
		const RECT* GetRectPtn() { return &mRect; }
		// �F�̎擾
		DirectX::SimpleMath::Vector4 GetColor() { return mColor; }
		// ���S�̎擾
		DirectX::XMFLOAT2 GetOrigin() { return mOrigin; }
		// �傫���̎擾
		DirectX::XMFLOAT2 GetScale() { return  mScale; }
	public: // setter
		// �e�N�X�`���̐ݒ�
		void SetTexture(DxTexture pTexture) {
			mpTexture = pTexture;
			mFlag.On(eDrawManagerFlag::NOT_TEXTURE_DELETE);
		}
		// ���W�̐ݒ�
		void SetPos(float x, float y) { mPos.x = x; mPos.y = y; }
		void SetPos(DirectX::SimpleMath::Vector2 pos) { mPos = pos; }
		// ��]�x�̐ݒ�
		void SetRot(float r) { mRot = r; }
		// �\���͈͂̐ݒ�
		void SetRect(LONG left, LONG top, LONG right, LONG bottom) {
			mRect.left = left;
			mRect.top = top;
			mRect.right = right;
			mRect.bottom = bottom;
		}
		void SetRect(LONG right, LONG bottom) { SetRect(0, 0, right, bottom); }
		// �F�̐ݒ�
		void SetColor(DirectX::SimpleMath::Vector4 color) { mColor = color; }
		// ���S�̐ݒ�
		void SetOrigin(float x, float y) { mOrigin.x = x; mOrigin.y = y; }
		void SetOrigin(DirectX::SimpleMath::Vector2 origin) { mOrigin = origin; }
		// �傫���̐ݒ�
		void SetScale(float x, float y) { mScale.x = x; mScale.y = y; }
		void SetScale(DirectX::SimpleMath::Vector2 scale) { mScale = scale; }
	};


	class DrawManager : public Utility::SingletonBase<DrawManager>
	{
	public:
		friend class Utility::SingletonBase<DrawManager>;

	private:
		// �f�o�C�X
		ID3D11Device* mpDevice;
		// �X�v���C�g�o�b�`
		std::unique_ptr<DirectX::SpriteBatch> mSpriteBatch;

	protected:
		// �R���X�g���N�^
		DrawManager() {};
		// �f�X�g���N�^
		~DrawManager() {};

	public:
		// ����������
		void Initialize(ID3D11Device* pDevise, ID3D11DeviceContext* pContext);
		// �`��
		void Draw(DrawData& data);
		// �`��O�㏈��
		void Begin();
		void End();
		// �e�N�X�`���ǂݍ���
		bool LoadTexture(DrawData& data, wchar_t* pFileName);
		bool LoadTexture(DrawData& data, const wchar_t* pFileName);
		bool LoadTexture(DxTexture& pTexture, wchar_t* pFileName);
		bool LoadTexture(DxTexture& pTexture, const wchar_t* pFileName);
		// �e�N�X�`���폜
		/*void DeleteTexture(DrawData& pData);*/
		// �f�o�C�X�̐ݒ�
		void SetDevise(ID3D11Device* pDevise)
		{
			mpDevice = pDevise;
		}
		// �f�o�C�X�̎擾
		ID3D11Device* GetDevise()
		{
			return mpDevice;
		}
	};
}
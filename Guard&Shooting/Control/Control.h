//
//	File	 : Control.h
//
//  Contents : 操作に応じて値を渡す基底クラス
//
//	name	 : Hibiki Yoshiyasu
//
#pragma once

class Control
{
protected:// メンバ変数
	__int8 m_actionVal;	// 操作に応じて入る値

public:// コンストラクタ&デストラクタ
	Control(): m_actionVal(0){};
	~Control() {};

public:// メンバ関数
	virtual void Update() = 0;

public :// 設定関数&取得関数
	void SetActionVal(__int8 actionVal) { m_actionVal = actionVal; }
	__int8 GetActionVal() { return m_actionVal; }
};

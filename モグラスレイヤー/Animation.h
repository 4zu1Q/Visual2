#pragma once
#include "DxLib.h"

class Animation
{
public:
	Animation();
	virtual ~Animation();

	void Load();
	void Delete();

	void Init();
	void Update();

	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo ,int modelHandle);

	//アニメーションの変更
	void ChangeAnim(int animIndex,int modelHandle);

private:
	
	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合
							//0.0f:prevが再生される
							//1.0f:currentが再生される



};


#pragma once
#include<DxLib.h>

class MoveDirectionVec
{
public:
	MoveDirectionVec() :
		m_directionVec(VGet(0.0f, 0.0f, 0.0f))
	{

	};

	~MoveDirectionVec();

	void Update();

	//移動方向取得
	VECTOR GetDirectionVec() const { return m_directionVec; }
	//移動方向セット
	void SetDirectionVec();

private:
	//移動方向ベクトル
	VECTOR m_directionVec;
};


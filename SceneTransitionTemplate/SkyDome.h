#pragma once
#include <memory>
#include "DxLib.h"

class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void Update(VECTOR pos);
	void Draw();

private:
	//スカイドームの回転率
	float m_rot;
	//スカイドームのモデル
	int m_handle = -1;

	VECTOR m_pos = VGet(0, 0, 0);

};


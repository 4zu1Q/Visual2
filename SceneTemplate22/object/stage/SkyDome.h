#pragma once
#include <memory>
#include "DxLib.h"

class Player;
class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void Update();
	void Draw();

private:
	//スカイドームの回転率
	float m_rot;
	//スカイドームのモデル
	int m_handle = -1;

	VECTOR m_pos = VGet(0, 0, 0);

};

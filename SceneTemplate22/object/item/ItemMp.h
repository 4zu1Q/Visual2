#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class ItemMp
{
public:
	ItemMp();
	virtual ~ItemMp();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

private:

	//モデルハンドル
	int m_modelH;
	//座標
	VECTOR m_pos;
	//移動
	VECTOR m_move;

	//回転
	float m_rot;

};


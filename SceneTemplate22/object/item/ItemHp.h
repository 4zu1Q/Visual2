#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class ItemHp
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool HpHit(std::shared_ptr<Player> pPlayer);

private:

	//モデルハンドル
	int m_modelH;
	//座標
	VECTOR m_pos;
	//移動
	VECTOR m_move;
	//半径
	float m_radius;
	//回転
	float m_rot;

};


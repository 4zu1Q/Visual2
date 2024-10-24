#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class ItemBase
{
public:
	ItemBase();
	virtual ~ItemBase();

	//メンバー変数にアクセスする
	void SetModelHandle(int modelH) { m_modelH = modelH; }

	void Start(VECTOR pos) { m_pos = pos; }

	virtual void Update();
	virtual void Draw(std::shared_ptr<Player> pPlayer);

	//アイテムが消える処理
	void ItemLost();

	//座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool ItemHit(std::shared_ptr<Player> pPlayer);

protected:

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


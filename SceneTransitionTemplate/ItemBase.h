#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class ItemBase
{
public:
	ItemBase();
	virtual ~ItemBase() {};

	//メンバー変数にアクセスする
	void SetHandle(int handle) { m_handle = handle; }
	
	void Start(VECTOR pos) { m_pos = pos; }
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//アイテムが消える処理
	void ItemLost();

	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	bool ItemSphereFlag(std::shared_ptr<Player> pPlayer);


protected:

	//std::shared_ptr<Player> m_pPlayer;

	//ハンドル
	int m_handle;

	//座標
	VECTOR m_pos;

	//半径
	float m_radius;
};


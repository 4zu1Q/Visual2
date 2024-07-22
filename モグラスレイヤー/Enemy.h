#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy
{
public:

	Enemy();
	virtual ~Enemy();

	void Load();
	void Delete();

	void Init();
	void Update(Player& player);
	void Draw();

	//プレイヤーと敵の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの攻撃と敵の当たり判定
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);


	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

private:
	//モデルハンドル
	//int m_modelHandle[8];
	int m_modelHandle;
	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;

	//表示情報
	VECTOR m_pos;

	//当たり判定の半径
	float m_radius;

};


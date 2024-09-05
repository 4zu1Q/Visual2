#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class EnemyBase
{
public:
	EnemyBase();
	virtual ~EnemyBase() {};

	//メンバー変数にアクセスする
	void SetHandle(int handle) { m_handle = handle; }

	void Start(VECTOR pos) { m_pos = pos; }
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//敵が消える処理
	void EnemyLost();

	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }


	//プレイヤーと敵の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの攻撃と敵の当たり判定
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの必殺技と敵の当たり判定
	bool SkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//敵の攻撃判定
	bool DamageSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//敵の索敵範囲
	bool SearchSphereFlag(std::shared_ptr<Player> pPlayer);
	//敵が止まる範囲
	bool StopSphereFlag(std::shared_ptr<Player> pPlayer);

protected:


	//ハンドル
	int m_handle;

	//座標
	VECTOR m_pos;
	VECTOR m_attackPos;

	//Hp
	int m_hp;

	//当たり判定の半径
	float m_radius;
	float m_searchRadius;
	float m_stopRadius;


};


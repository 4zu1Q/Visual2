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
	void Update();
	void Draw();

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


	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//敵のHPを取得
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	

	//プレイヤーダメージのフラグ取得
	void SetDamage(const bool damage) { m_isDamage = damage; }

	//敵が何をするかをきめる
	enum State
	{
		kIdle,	//待機
		kRun,	//走る
		kAttack,	 //攻撃
		kPowerAttack,//溜め攻撃
	};

	State GetState() { return m_state; }
	void SetState(State state) { m_state = state; }

private:

	std::shared_ptr<Player> m_pPlayer;

	//モデルハンドル
	//int m_modelHandle[8];
	int m_modelHandle;
	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	int m_damageFrame;
	bool m_isDamage;
	

	//表示情報
	VECTOR m_pos;
	VECTOR m_attackPos;
	VECTOR m_velocity;

	//当たり判定の半径
	float m_radius;
	float m_searchRadius;
	float m_stopRadius;

	//State変数
	State m_state;

#ifdef _DEBUG
	bool m_isDebugFlag = false;
#endif

};


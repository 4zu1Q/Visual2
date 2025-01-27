#pragma once
#include "BossBase.h"

#include <memory>

class AnimController;
class ActionTime;

class Player;

class BossPower : public BossBase
{
public:

	BossPower();
	virtual ~BossPower();

	void Initialize(std::shared_ptr<MyLib::Physics> physics) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;

	void Update(std::shared_ptr<MyLib::Physics> physics, Player& player);
	void Draw();

	const VECTOR& GetPosDown() const;
	void SetPosDown(const VECTOR pos);

	const VECTOR& GetPosUp() const;

	const float& GetHp() const { return m_hp; }

private:

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void WalkUpdate();
	void DashUpdate();
	void Attack1Update();
	void Attack2Update();
	void Attack3Update();
	void AvoidUpdate();
	void AttackCoolTimeUpdate();
	void DownUpdate();
	void DeadUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnWalk();
	void OnDash();
	void OnAttack1();
	void OnAttack2();
	void OnAttack3();
	void OnAvoid();
	void OnAttackCoolTime();
	void OnDown();
	void OnDead();

private:

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Player> m_pPlayer;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(BossPower::*)();
	UpdateFunc_t m_updateFunc;

	bool m_isAttack;

	//次の行動に移るための変数
	int m_actionTime;
	int m_actionKind;


	//攻撃判定の半径
	float m_attackRadius;

	float m_angle;
	float m_nextAngle;

	float m_hp;

	VECTOR m_playerPos;

	float m_length;

	//座標
	VECTOR m_pos;
	VECTOR m_posUp;

	VECTOR m_direction;
	VECTOR m_velocity;

};


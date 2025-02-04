#pragma once
#include "BossBase.h"
#include "util/Game.h"

#include <memory>

class AnimController;
class ActionTime;

class Player;

class BossSpeed : public BossBase
{
public:

	BossSpeed();
	virtual ~BossSpeed();

	void Initialize(std::shared_ptr<MyLib::Physics> physics) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;

	void Update(std::shared_ptr<MyLib::Physics> physics, Player& player, Game::e_PlayerAttackKind playerAttackKind);
	void Draw();

	const VECTOR& GetPosUp() const;

	const VECTOR& GetPosDown() const;
	void SetPosDown(const VECTOR pos);

	const float& GetHp() const { return m_hp; }

private:
	void Hit();

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void DashUpdate();
	void PlayerToDashUpdate();
	void HomePosDashUpdate();
	void PreliminaryAttack1Update();
	void PreliminaryAttack2Update();
	void PreliminaryAttack3Update();
	void Attack1Update();
	void Attack2Update();
	void Attack3Update();
	void AvoidUpdate();
	void AttackCoolTimeUpdate();
	void HitOneDamageUpdate();
	void HitTwoDamageUpdate();
	void DownUpdate();
	void DeadUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnDash();
	void OnPlayerToDash();
	void OnHomePosDash();
	void OnPreliminaryAttack1();
	void OnPreliminaryAttack2();
	void OnPreliminaryAttack3();
	void OnAttack1();
	void OnAttack2();
	void OnAttack3();
	void OnAvoid();
	void OnAttackCoolTime();
	void OnHitOneDamage();
	void OnHitTwoDamage();
	void OnDown();
	void OnDead();

private:

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Player> m_pPlayer;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(BossSpeed::*)();
	UpdateFunc_t m_updateFunc;


	VECTOR m_anglePos;

	int m_actionTime;
	bool m_isAttackAction;
	bool m_isAvoidAction;

	int m_attackType;
	int m_attackNum;

	float m_moveAngle;

	//攻撃判定の半径
	float m_attackRadius;

	float m_angle;
	float m_nextAngle;

	float m_hp;

	VECTOR m_playerPos;

	float m_bossToPlayerLength;
	float m_bossToHomePosLength;

	VECTOR m_homePos;

	//座標
	VECTOR m_pos;
	VECTOR m_posUp;

	VECTOR m_direction;
	VECTOR m_velocity;

};
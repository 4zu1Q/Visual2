﻿#pragma once
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

	void Update(std::shared_ptr<MyLib::Physics> physics, Player& player, Game::e_PlayerAttackKind playerAttackKind);

	void Draw();

	const VECTOR& GetPosDown() const;
	void SetPosDown(const VECTOR pos);

	const VECTOR& GetPosUp() const;

private:

	void Hit();

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void WalkUpdate();
	void DashUpdate();
	void PreliminaryAttack1Update();
	void PreliminaryAttack2Update();
	void PreliminaryAttack3Update();
	void AxeAttackUpdate();
	void TwoHandedAttackUpdate();
	void Attack3Update();
	void AttackCoolTimeUpdate();
	void HitOneDamageUpdate();
	void HitTwoDamageUpdate();
	void DownUpdate();
	void DeadUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnWalk();
	void OnDash();
	void OnPreliminaryAttack1();
	void OnPreliminaryAttack2();
	void OnPreliminaryAttack3();
	void OnAxeAttack();
	void OnTwoHandedAttack();
	void OnJumpAttack();
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
	using UpdateFunc_t = void(BossPower::*)();
	UpdateFunc_t m_updateFunc;

	//次の行動に移るための変数
	int m_actionTime;
	int m_actionKind;

	float m_angle;
	float m_nextAngle;


	VECTOR m_playerPos;

	float m_length;

	//座標
	VECTOR m_posUp;

	VECTOR m_direction;
	VECTOR m_velocity;

};


#pragma once
#include "BossBase.h"
#include <memory>

class AnimController;
class ActionTime;

class Player;

class BossShot : public BossBase
{
public:

	BossShot();
	virtual ~BossShot();

	void Initialize(std::shared_ptr<MyLib::Physics> physics) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;

	void Update(std::shared_ptr<MyLib::Physics> physics, Player& player);
	void Draw();

	const VECTOR& GetPosDown() const;
	void SetPosDown(const VECTOR pos);

	const float& GetHp() const { return m_hp; }

private:

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void WalkUpdate();
	void DashUpdate();
	void Attack1Update();
	void Attack2Update();
	void Attack3Update();
	void AvoidFrontUpdate();
	void AvoidBackUpdate();
	void AvoidRightUpdate();
	void AvoidLeftUpdate();
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
	void OnAvoidFront();
	void OnAvoidBack();
	void OnAvoidRight();
	void OnAvoidLeft();
	void OnAttackCoolTime();
	void OnDown();
	void OnDead();

private:

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Player> m_pPlayer;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(BossShot::*)();
	UpdateFunc_t m_updateFunc;

	//モデルハンドル
	//int m_modelH;

	//タイマー関連の変数
	//std::shared_ptr<ActionTime> m_pOnWlakTime;
	//std::shared_ptr<ActionTime> m_pOnDashTime;
	//std::shared_ptr<ActionTime> m_pOnIdleTime;
	//std::shared_ptr<ActionTime> m_pOnDownTime;
	//std::shared_ptr<ActionTime> m_pOnAttackTime;

	int m_attackCoolTime;
	int m_actionTime;
	bool m_isAttack;
	bool m_isAvoid;

	int m_attackKind;


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


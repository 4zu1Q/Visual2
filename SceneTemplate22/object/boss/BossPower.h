#pragma once
#include "BossBase.h"

#include <memory>

class AnimController;
class ActionTime;

class Player;

class BossPower :
	public BossBase
{
public:
	BossPower();
	virtual ~BossPower();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

private:

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void WalkUpdate();
	void Attack1Update();
	void Attack2Update();
	void Attack3Update();
	void DownUpdate();
	void DeadUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnWalk();
	void OnAttack1();
	void OnAttack2();
	void OnAttack3();
	void OnDown();
	void OnDead();

private:

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Player> m_pPlayer;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(BossPower::*)(/*引数書く*/);
	UpdateFunc_t m_updaFunc;

	//モデルハンドル
	int m_modelH;

	//タイマー関連の変数
	std::shared_ptr<ActionTime> m_pOnWlakTime;
	std::shared_ptr<ActionTime> m_pOnDashTime;
	std::shared_ptr<ActionTime> m_pOnIdleTime;
	std::shared_ptr<ActionTime> m_pOnDownTime;
	std::shared_ptr<ActionTime> m_pOnAttackTime;

	//当たり判定の半径
	float m_radius;
	//攻撃判定の半径
	float m_attackRadius;

	float m_angle;


	//座標
	VECTOR m_posDown;
	VECTOR m_posUp;

	VECTOR m_direction;
	VECTOR m_velocity;

};


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
	void PreliminaryAttack1Update();
	void PreliminaryAttack2Update();
	void PreliminaryAttack3Update();
	void Attack1Update();
	void Attack2Update();
	void Attack3Update();
	void DashFrontUpdate();
	void DashBackUpdate();
	void DashRightUpdate();
	void DashLeftUpdate();
	void AttackCoolTimeUpdate();
	void HitOneDamageUpdate();
	void HitTwoDamageUpdate();
	void DownUpdate();
	void DeadUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnPreliminaryAttack1();
	void OnPreliminaryAttack2();
	void OnPreliminaryAttack3();
	void OnAttack1();
	void OnAttack2();
	void OnAttack3();
	void OnDashFront();
	void OnDashBack();
	void OnDashRight();
	void OnDashLeft();
	void OnAttackCoolTime();
	void OnHitOneDamage();
	void OnHitTwoDamage();
	void OnDown();
	void OnDead();

private:

	//ステージ名
	enum class e_AvoidKind
	{
		kFront,	
		kBack,
		kRight,
		kLeft,
	};

private:

	int m_weaponModelH;

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Player> m_pPlayer;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(BossShot::*)();
	UpdateFunc_t m_updateFunc;

	//ボスが移動する場所との長さを入れる変数
	float m_dashFrontLength;
	float m_dashBackLength;
	float m_dashRightLength;
	float m_avoidLeftLength;

	VECTOR m_dashFrontPos;
	VECTOR m_dashBackPos;
	VECTOR m_dashRightPos;
	VECTOR m_dashLeftPos;

	//アタッチする座標
	VECTOR m_attachFramePos;
	int m_modelRightFrame;	//アタッチするモデルの右手のフレーム

	int m_actionTime;
	int m_actionKind;
	bool m_isAttack;
	bool m_isAvoid;

	e_AvoidKind m_avoidKind;


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


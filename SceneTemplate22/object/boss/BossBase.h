#pragma once
#include "DxLib.h"
#include "object/CharaBase.h"
#include "util/Game.h"

class Player;

class BossBase : public CharaBase
{
public:
	BossBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~BossBase();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();


	void HitUpdate(VECTOR attackXPos,VECTOR attackYPos,VECTOR shockPos, float attackXRadius,float attackYRadius,float shockRadius, bool isAttack);

	//ボスの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }
	
	//ボスのヒット座標
	const VECTOR& GetHitPos() const { return m_hitPos; }
	void SetHitPos(const VECTOR hitPos) { m_hitPos = hitPos; }

	//ボスの攻撃座標(武器座標も同じ座標)
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }
	//ボスの衝撃波座標
	const VECTOR& GetShockPos() const { return m_shockAttackPos; }
	void SetShockPos(const VECTOR shockAttackPos) { m_shockAttackPos = shockAttackPos; }

	//ボスのショット座標
	//const VECTOR& GetShotPos() const { return m_pos; }
	//void SetShotPos(const VECTOR pos) { m_pos = pos; }

	//ボスのHPを取得
	const float& GetHp() const { return m_hp; }
	void SetHp(const float hp) { m_hp = hp; }

	//ボスの攻撃半径
	const float& GetHitRadius() const { return m_hitRadius; }
	void SetHitRadius(const float normalAttackRadius) { m_hitRadius = normalAttackRadius; }
	//ボスの攻撃半径
	const float& GetAttackRadius() const { return m_normalAttackRadius; }
	void SetAttackRadius(const float normalAttackRadius) { m_normalAttackRadius = normalAttackRadius; }
	//ボスの武器半径
	const float& GetWeaponRadius() const { return m_weaponAttackRadius; }
	void SetWeaponRadius(const float weaponAttackRadius) { m_weaponAttackRadius = weaponAttackRadius; }
	//ボスの衝撃半径
	const float& GetShockRadius() const { return m_shockRadius; }
	void SetShockRadius(const float shockAttackPos) { m_shockRadius = shockAttackPos; }
	//ボスのショットの半径
	const float& GetShotRadius() const { return m_shotRadius; }
	void SetShotRadius(const float shotAttackPos) { m_shotRadius = shotAttackPos; }

	const bool& GetIsClear() const { return m_isClear; }

	const Game::e_BossAttackKind& GetAttackKind() const { return m_attackKind; }

	void PlayerLockOn(Player& player);

	/// <summary>
	/// アタックしたかどうか
	/// </summary>
	/// <returns>攻撃生成の時に使う</returns>
	const bool& GetIsAttack() const { return m_isAttack; }
	void SetIsAttack(const bool isAttack) { m_isAttack = isAttack; }

	/// <summary>
	/// 衝撃波を出したかどうか
	/// </summary>
	/// <returns>衝撃波生成の時に使う</returns>
	const bool& GetIsShock() const { return m_isShock; }
	void SetIsShock(const bool isShock) { m_isShock = isShock; }

	//攻撃判定
	bool IsAttackXHit();
	bool IsAttackYHit();
	bool IsShockAttackHit();


protected:

	//HP
	float m_hp;

	//モデルハンドル
	//int m_modelH;

	//ボスが倒されたかどうかのフラグ
	bool m_isClear;

	//座標
	VECTOR m_pos;
	VECTOR m_hitPos;
	VECTOR m_attackPos;
	VECTOR m_shockAttackPos;

	VECTOR m_attackDir;

	//プレイヤーの攻撃座標と半径
	VECTOR m_playerAttackXPos;
	VECTOR m_playerAttackYPos;
	VECTOR m_playerShockPos;
	float m_playerAttackXRadius;
	float m_playerAttackYRadius;
	float m_playerShockRadius;
	bool m_isPlayerAttack;

	int m_preliminaryActionFrame;

	//ボスがプレイヤーの攻撃にヒットする当たり判定の半径
	float m_hitRadius;
	//ボスの通常攻撃半径
	float m_normalAttackRadius;
	//ボスの武器の攻撃半径
	float m_weaponAttackRadius;
	//ボスの衝撃波攻撃半径
	float m_shockRadius;
	//ボスのショット攻撃半径
	float m_shotRadius;

	//攻撃判定
	bool m_isAttack;
	bool m_isShock;

	bool m_isHit;
	bool m_isPlayerFace;

	int m_attackFrame;
	int m_damageFrame;

	//ダウン判定
	bool m_isDown;

	//ダウンカウントダウン
	int m_downCountDown;

	Game::e_BossAttackKind m_attackKind;
	Game::e_PlayerAttackKind m_playerAttackKind;
	e_PlayerKind m_playerKind;
};


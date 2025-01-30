#pragma once
#include "DxLib.h"
#include "object/CharaBase.h"

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

	//ボスの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//ボスのHPを取得
	const float& GetHp() const { return m_hp; }
	void SetHp(const float hp) { m_hp = hp; }

	const bool& GetIsClear() const { return m_isClear; }

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

	//ダウン判定
	bool m_isDown;

	//ダウンカウントダウン
	int m_downCountDown;

};


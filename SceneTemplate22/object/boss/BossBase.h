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
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	const bool& GetIsClear() const { return m_isClear; }

	void PlayerLockOn(Player& player);

protected:

	//HP
	int m_hp;

	//モデルハンドル
	//int m_modelH;

	//ボスが倒されたかどうかのフラグ
	bool m_isClear;

	//座標
	VECTOR m_pos;
	VECTOR m_attackPos;


};


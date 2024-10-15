#pragma once
#include "DxLib.h"

class BossBase
{
public:
	BossBase();
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

private:

	int m_hp;




	VECTOR m_pos;
	VECTOR m_attackPos;



};


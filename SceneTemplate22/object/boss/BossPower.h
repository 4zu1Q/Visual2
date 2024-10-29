#pragma once
#include "BossBase.h"
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

	//当たり判定の半径
	float m_radius;
	//攻撃判定の半径
	float m_attackRadius;

	//座標
	VECTOR m_pos;



};


#pragma once

#include "util/Vec2.h"

#include <memory>

//class Player;
class BossPower;
//class BossBase;

class HpBar
{
public:

	enum class e_BossType : int
	{

	};


public:

	HpBar();
	virtual ~HpBar();

	void Update(int boss);
	void Draw();

private:

	//std::shared_ptr<Player> m_pPlayer;
	//std::shared_ptr<BossBase> m_pBoss;

	int m_bossHp;
	int m_bossHpH;
};


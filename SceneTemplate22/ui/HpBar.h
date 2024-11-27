#pragma once

#include "util/Vec2.h"

#include <memory>

class Player;
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

	void Update(Player& player);
	void Draw();

private:

	//std::shared_ptr<Player> m_pPlayer;
	//std::shared_ptr<BossBase> m_pBoss;

	float m_playerHp;
	float m_playerMp;
	float m_playerStamina;

	float m_playerPrevMp;
	float m_playerPrevStamina;

	bool m_isPlayerStamina;

	int m_playerHpH;
	int m_playerHpLostH;

	int m_bossHp;
	int m_bossHpH;
};


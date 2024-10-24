﻿#pragma once

#include "util/Vec2.h"

#include <memory>

class Player;
class BossBase;

class HpBar
{
public:

	enum class e_BossType : int
	{

	};


public:

	HpBar();
	virtual ~HpBar();

	void Initialize();
	void Finalize();

	void Update(Player& player);
	void Draw();

private:

	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<BossBase> m_pBoss;

	float m_playerHp;
	int m_bossHp;

	int m_playerHpH;
	int m_bossHpH;
};


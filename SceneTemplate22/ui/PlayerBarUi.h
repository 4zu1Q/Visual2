#pragma once

#include "util/Vec2.h"


class Player;
class PlayerBarUi
{
public:
	PlayerBarUi();
	virtual ~PlayerBarUi();

	void Update(Player& player);
	void Draw();

private:

	float m_playerHp;
	float m_playerMp;
	float m_playerStamina;

	//float m_playerPrevMp;
	//float m_playerPrevStamina;

	bool m_isPlayerStamina;

	int m_playerHpH;
	int m_playerHpLostH;

};


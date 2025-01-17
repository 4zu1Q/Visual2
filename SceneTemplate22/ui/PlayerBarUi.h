#pragma once

#include "util/Vec2.h"

#include <vector>

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

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	bool m_isPlayerStamina;

};


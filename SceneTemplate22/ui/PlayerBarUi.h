#pragma once

#include "util/Vec2.h"

#include <vector>
#include <array>

class Player;
class PlayerBarUi
{
public:
	PlayerBarUi();
	virtual ~PlayerBarUi();

	void Update(Player& player);
	void Draw();

private:

	int m_stamina;

	float m_playerHp;
	float m_playerMp;
	float m_playerStamina;

	std::vector<int> m_handles;

	//スタミナゲージハンドル
	//std::array<int, 3> m_staminaGaugeHandle{};

	bool m_isPlayerStamina;

	double m_percent;
	double m_percentGreenGauge;

};


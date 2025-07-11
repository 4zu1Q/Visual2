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

	//プレイヤーのステータス
	float m_playerHp;
	float m_playerMp;
	float m_playerStamina;
	float m_isPlayerMP;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	//スタミナを使い切った場合
	bool m_isPlayerStamina;

	//カーソルアニメーション
	float m_selectAnimation;

	double m_percent;
	double m_percentGreenGauge;

};


#include "PlayerBarUi.h"
#include "object/player/Player.h"

namespace
{
	const char* const kHpFileName = "Data/Image/Hp.png";
	const char* const kHpLostFileName = "Data/Image/HpLost.png";

	//最大値
	constexpr float kMaxHp = 10.0f;
	constexpr float kMaxMp = 300.0f;
	constexpr float kMaxStamina = 300.0f;

	//スタミナの減るスピード
	constexpr float kStaminaDiminishSpeed = 0.5f;
	constexpr float kStaminaIncreaseSpeed = 0.4f;

	//MPの減る値
	constexpr float kMpDiminishNum = 30.0f;
	constexpr float kMpIncreaseNum = 40.0f;

	//座標
	//HP座標
	const Vec2 kHpPos = { 34.0f , 15.0f };


}

PlayerBarUi::PlayerBarUi():
	m_playerHp(0.0f),
	m_playerMp(0.0f),
	m_playerStamina(0.0f),
	m_isPlayerStamina(false),
	m_playerHpH(LoadGraph(kHpFileName)),
	m_playerHpLostH(LoadGraph(kHpLostFileName))
{

}

PlayerBarUi::~PlayerBarUi()
{
	DeleteGraph(m_playerHpH);
	DeleteGraph(m_playerHpLostH);
}

void PlayerBarUi::Update(Player& player)
{
	m_playerHp = player.GetHp();
	m_playerMp = player.GetMp();
	m_playerStamina = player.GetStamina();
	m_isPlayerStamina = player.GetIsStamina();
}

void PlayerBarUi::Draw()
{
	//変動しないバー
	DrawBox(34, 55, 34 + kMaxMp, 70, 0x000000, true);
	DrawBox(34, 75, 34 + kMaxStamina, 90, 0x000000, true);

	//少し遅れて変動するバー


	//変動するバー
	DrawBox(34, 55, 34 + m_playerMp, 70, 0x00bfff, true);

	if (!m_isPlayerStamina)
	{
		DrawBox(34, 75, 34 + m_playerStamina, 90, 0x00ff00, true);
	}
	else
	{
		DrawBox(34, 75, 34 + m_playerStamina, 90, 0xffa500, true);
	}

	//枠
	//DrawBox(32, 10, 374, 50, 0xffffff, false);
	DrawBox(34, 55, 334, 70, 0xffffff, false);
	DrawBox(34, 75, 334, 90, 0xffffff, false);

	//プレイヤーのロストした時のHP
	for (int i = 1; i <= static_cast<int>(kMaxHp); i++)
	{
		DrawGraph(kHpPos.x * i, kHpPos.y, m_playerHpLostH, true);
	}
	//プレイヤーのHP
	for (int i = 1; i <= static_cast<int>(m_playerHp); i++)
	{
		DrawGraph(kHpPos.x * i, kHpPos.y, m_playerHpH, true);
	}
}

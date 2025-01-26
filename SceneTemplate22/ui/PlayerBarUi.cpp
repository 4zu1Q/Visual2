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

	const Vec2 kMpLeftBarPos = { 60.0f , 60.0f };
	const Vec2 kMpRightBarPos = { 60.0f , 75.0f };
	const Vec2 kStaminaLeftBarPos = { 60.0f , 89.0f };
	const Vec2 kStaminaRightBarPos = { 60.0f , 104.0f };

	const Vec2 kMpBarPos = { 20.0f , 40.0f };
	const Vec2 kStaminaBarPos = { 20.0f , 70.0f };

	//ハンドル名
	std::array<const char*, 3>kFileName =
	{
		"Data/Image/StaminaGaugeGreen.png",
		"Data/Image/StaminaGaugeRed.png",
		"Data/Image/StaminaGaugeBlack.png"
	};

	//使う画像の種類
	enum e_Ui
	{
		kHpH,
		kHpLostH,
		kStaminaBarH,
		kMpBarH,
		kStGreenCircleH,
		kStRedCircleH,
		kStBlackCircleH,
	};

	//スタミナゲージの座標
	constexpr float kStaminaGaugePosX = 80.0f;
	constexpr float kStaminaGaugePosY = 120.0f;

	constexpr float kMaxPercent = 20.0f;

}

PlayerBarUi::PlayerBarUi():
	m_playerHp(0.0f),
	m_playerMp(0.0f),
	m_playerStamina(0.0f),
	m_isPlayerStamina(false)
{
	m_percent = 100.0f;
	m_percentGreenGauge = 0.0f;

	m_handles.push_back(LoadGraph("Data/Image/Hp.png"));
	m_handles.push_back(LoadGraph("Data/Image/HpLost.png"));
	m_handles.push_back(LoadGraph("Data/Image/BarFlameSt.png"));
	m_handles.push_back(LoadGraph("Data/Image/BarFlameMp.png"));
	m_handles.push_back(LoadGraph("Data/Image/StaminaGaugeGreen.png"));
	m_handles.push_back(LoadGraph("Data/Image/StaminaGaugeRed.png"));
	m_handles.push_back(LoadGraph("Data/Image/StaminaGaugeBlack.png"));

	//画像ロード
	//for (int i = 0; i < m_staminaGaugeHandle.size(); i++)
	//{
	//	m_staminaGaugeHandle[i] = LoadGraph(kFileName[i]);
	//}

}

PlayerBarUi::~PlayerBarUi()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	//画像削除
	//for (int i = 0; i < m_staminaGaugeHandle.size(); i++)
	//{
	//	DeleteGraph(m_staminaGaugeHandle[i]);
	//}

	m_handles.clear();

}

void PlayerBarUi::Update(Player& player)
{
	m_playerHp = player.GetHp();
	m_playerMp = player.GetMp();
	m_playerStamina = player.GetStamina();
	m_isPlayerStamina = player.GetIsStamina();

	//スタミナの残り％を計算
	m_percent = static_cast<double>(m_stamina) /
		static_cast<double>(kMaxStamina) * kMaxPercent;

}

void PlayerBarUi::Draw()
{
	//スタミナゲージの位置
	DrawBox(kMpLeftBarPos.x, kMpLeftBarPos.y, kMpRightBarPos.x + kMaxMp, kMpRightBarPos.y, 0x000000, true);
	DrawBox(kStaminaLeftBarPos.x, kStaminaLeftBarPos.y, kStaminaRightBarPos.x + kMaxStamina, kStaminaRightBarPos.y, 0x000000, true);

	//変動するバー
	DrawBox(kMpLeftBarPos.x, kMpLeftBarPos.y, kMpRightBarPos.x + m_playerMp, kMpRightBarPos.y, 0x00bfff, true);

	if (!m_isPlayerStamina)
	{
		DrawBox(kStaminaLeftBarPos.x, kStaminaLeftBarPos.y, kStaminaRightBarPos.x + m_playerStamina, kStaminaRightBarPos.y, 0x00ff00, true);
	}
	else
	{
		DrawBox(kStaminaLeftBarPos.x, kStaminaLeftBarPos.y, kStaminaRightBarPos.x + m_playerStamina, kStaminaRightBarPos.y, 0xffa500, true);
	}

	//プレイヤーのロストした時のHP
	for (int i = 1; i <= static_cast<int>(kMaxHp); i++)
	{
		DrawGraph(kHpPos.x * i, kHpPos.y, m_handles[kHpLostH], true);
	}
	//プレイヤーのHP
	for (int i = 1; i <= static_cast<int>(m_playerHp); i++)
	{
		DrawGraph(kHpPos.x * i, kHpPos.y, m_handles[kHpH], true);
	}

	DrawGraph(kMpBarPos.x, kMpBarPos.y, m_handles[kMpBarH], true);
	DrawGraph(kStaminaBarPos.x, kStaminaBarPos.y, m_handles[kStaminaBarH], true);
}

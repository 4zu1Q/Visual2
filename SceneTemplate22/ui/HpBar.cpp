#include "HpBar.h"
#include "DxLib.h"

#include "object/player/Player.h"
#include "object/boss/BossBase.h"

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
}

HpBar::HpBar() :
	m_playerHp(0.0f),
	m_playerMp(0.0f),
	m_playerStamina(0.0f),
	m_isPlayerStamina(false),
	m_playerHpH(LoadGraph(kHpFileName)),
	m_playerHpLostH(LoadGraph(kHpLostFileName)),
	m_bossHp(0)
{
	//m_pPlayer = std::make_shared<Player>();
	//m_pBoss = std::make_shared<BossBase>();

}

HpBar::~HpBar()
{
	DeleteGraph(m_playerHpH);

}

void HpBar::Update(Player& player)
{

}

void HpBar::Draw()
{


}

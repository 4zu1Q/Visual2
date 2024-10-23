#include "HpBar.h"
#include "DxLib.h"

#include "object/player/Player.h"
#include "object/boss/BossBase.h"

namespace
{

}

HpBar::HpBar():
	m_playerHp(0),
	m_bossHp(0)
{
	m_pPlayer = std::make_shared<Player>();
	m_pBoss = std::make_shared<BossBase>();

}

HpBar::~HpBar()
{

}

void HpBar::Initialize()
{

}

void HpBar::Finalize()
{

}

void HpBar::Update(Player& player)
{

	m_playerHp = player.GetHp();
	m_bossHp = m_pBoss->GetHp();


}

void HpBar::Draw()
{
	for (int i = 1; i <= static_cast<int>(m_playerHp); i++)
	{
		DrawCircle(18 * i, 32, 8, 0xff00ff, true);
	}

	//DrawBox(10, 48, 10 + 300, 48 + 16, 0xffff00, true);

}

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
	//m_pBoss = std::make_shared<BossBase>();

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


}

void HpBar::Draw()
{

	//HPの仮位置
	//スタミナの仮位置
	//魔力の仮位置
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	DrawBox(20, 90, 420, 110, 0x00ff00, true);
	DrawBox(20, 120, 420, 140, 0xff0000, true);
	DrawBox(20, 20, 420, 80, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	//枠
	DrawBox(20, 20, 420, 80, 0xffffff, false);
	DrawBox(20, 90, 420, 110, 0xffffff, false);
	DrawBox(20, 120, 420, 140, 0xffffff, false);


	for (int i = 1; i <= static_cast<int>(m_playerHp); i++)
	{
		DrawCircle(40 * i, 50, 17, 0xffffff, true);
		DrawCircle(40 * i, 50, 16, 0xff00ff, true);
	}


}

#include "HpBar.h"
#include "DxLib.h"

#include "object/player/Player.h"
#include "object/boss/BossBase.h"

namespace
{
	const char* const kHpFileName = "Data/Image/Hp02.png";

}

HpBar::HpBar():
	m_playerHp(0),
	m_playerHpH(LoadGraph(kHpFileName)),
	m_bossHp(0)
{
	m_pPlayer = std::make_shared<Player>();
	//m_pBoss = std::make_shared<BossBase>();

}

HpBar::~HpBar()
{
	DeleteGraph(m_playerHpH);

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
	//DrawBox(32, 10, 374, 50, 0x000000, true);
	DrawBox(34, 55, 334, 70, 0x00ff00, true);
	DrawBox(34, 75, 334, 90, 0xff0000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	
	//枠
	//DrawBox(32, 10, 374, 50, 0xffffff, false);
	DrawBox(34, 55, 334, 70, 0xffffff, false);
	DrawBox(34, 75, 334, 90, 0xffffff, false);


	for (int i = 1; i <= static_cast<int>(m_playerHp); i++)
	{
		//DrawCircle(40 * i, 50, 17, 0xffffff, true);
		//DrawCircle(40 * i, 50, 16, 0xff00ff, true);

		DrawGraph(34 * i, 15, m_playerHpH, true);
	}


}

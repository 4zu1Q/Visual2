#include "HpBar.h"
#include "DxLib.h"

#include "object/boss/BossPower.h"

namespace
{
	const char* const kHpFileName = "Data/Image/Hp.png";
	const char* const kHpLostFileName = "Data/Image/HpLost.png";

	//最大値
	constexpr float kMaxHp = 400.0f;


}

HpBar::HpBar() :
	m_bossHpH(-1),
	m_bossHp(kMaxHp)
{

}

HpBar::~HpBar()
{

}

void HpBar::Update(int boss)
{
	m_bossHp = boss;
}

void HpBar::Draw()
{
	DrawBox(440, 82, 440 + kMaxHp, 90, 0x000000, true);
	DrawBox(440, 82, 440 + m_bossHp, 90, 0xff6347, true);
}

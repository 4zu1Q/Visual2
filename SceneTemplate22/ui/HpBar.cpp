#include "HpBar.h"
#include "DxLib.h"

#include "object/boss/BossPower.h"

namespace
{
	const char* const kHpFileName = "Data/Image/Hp.png";
	const char* const kHpLostFileName = "Data/Image/HpLost.png";

	const char* const kPowerNameFileName = "Data/Image/BossPowerName.png";
	const char* const kSpeedNameFileName = "Data/Image/PowerBossName1.png";
	const char* const kShotNameFileName = "Data/Image/PowerBossName1.png";
	const char* const kRastNameFileName = "Data/Image/PowerBossName1.png";

	//使う画像の種類
	enum e_Ui
	{
		kPowerBossNameH,
		kSpeedBossNameH,
		kShotBossNameH,
		kRastBossNameH,
	};

	const Vec2 kNamePos = { 540.0f , 30.0f };

	//最大値
	constexpr float kMaxHp = 400.0f;

}

HpBar::HpBar() :
	m_bossHpH(-1),
	m_bossHp(kMaxHp)
{

	m_handles.push_back(LoadGraph(kPowerNameFileName));
	m_handles.push_back(LoadGraph(kSpeedNameFileName));
	m_handles.push_back(LoadGraph(kShotNameFileName));
	m_handles.push_back(LoadGraph(kRastNameFileName));

}

HpBar::~HpBar()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
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

void HpBar::DrawBossName(Game::e_BossKind bossKind)
{
	if (bossKind == Game::e_BossKind::kPower)
	{
		DrawGraph(kNamePos.x, kNamePos.y, m_handles[kPowerBossNameH], true);
	}
	else if (bossKind == Game::e_BossKind::kSpeed)
	{
		DrawGraph(kNamePos.x, kNamePos.y, m_handles[kSpeedBossNameH], true);
	}
	else if (bossKind == Game::e_BossKind::kSpeed)
	{
		DrawGraph(kNamePos.x, kNamePos.y, m_handles[kShotBossNameH], true);
	}
	else if (bossKind == Game::e_BossKind::kSpeed)
	{
		DrawGraph(kNamePos.x, kNamePos.y, m_handles[kRastBossNameH], true);
	}

}

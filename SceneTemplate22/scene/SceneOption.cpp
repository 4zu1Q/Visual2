#include "DxLib.h"

#include "SceneOption.h"
#include "SceneManager.h"

#include "util/Game.h"
#include "util/Pad.h"


namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//BGMやSEなどのテキストポジション
	constexpr int kTextPosY[4] = { 180, 280, 380, 480 };

	constexpr int kPushWaitCount = 30;

	//オプションの背景アルファ値
	constexpr int kAlpha = 200;
	
}

SceneOption::SceneOption(SceneManager& manager) :
	SceneBase(manager),
	m_pushCount(0),
	m_bgmScale(50),
	m_seScale(50),
	m_sensitivityScale(50),
	m_isFullScreen(false)
{
	m_nowItem = e_Item::kBgm;

	FadeInSkip();

}

SceneOption::~SceneOption()
{

}

void SceneOption::Update()
{
	Pad::Update();
	UpdateFade();

	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		m_pManager.PopScene();
	}

}

void SceneOption::Draw()
{
	DrawBlackFade();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG

	DrawFormatString(0, 0, 0xffffff, "Scene Option");

	DrawFormatString(0, 16, 0xffffff, "Item : %d", m_nowItem);

	DrawFormatString(32, 32, 0xffffff, "BGM");
	DrawFormatString(32, 48, 0xffffff, "SE");
	DrawFormatString(32, 64, 0xffffff, "Sensitivity");
	DrawFormatString(32, 80, 0xffffff, "FullScreen");

	DrawBox(128, 32, 128 + m_bgmScale, 45, 0xff00ff, true);
	DrawBox(128, 48, 128 + m_seScale, 61, 0xff00ff, true);
	DrawBox(128, 64, 128 + m_sensitivityScale, 77, 0xff00ff, true);
	
#endif

}

void SceneOption::BgmUpdate()
{
	Pad::Update();
	
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_nowItem = e_Item::kSe;
		m_updateFunc = &SceneOption::SeUpdate;
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_sensitivityScale++;
		}
	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_bgmScale--;
		}
	}

}

void SceneOption::SeUpdate()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_nowItem = e_Item::kBgm;
		m_updateFunc = &SceneOption::BgmUpdate;
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_seScale++;
		}
	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_seScale--;
		}
	}
}

void SceneOption::SensitivityUpdate()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_nowItem = e_Item::kSe;
		m_updateFunc = &SceneOption::SeUpdate;
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		m_nowItem = e_Item::kFullScreen;
		m_updateFunc = &SceneOption::FullScreenUpdate;
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_sensitivityScale++;
		}
	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_sensitivityScale--;
		}
	}
}

void SceneOption::FullScreenUpdate()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		ChangeWindowMode(m_isFullScreen);
		m_isFullScreen = !m_isFullScreen;
	}
}

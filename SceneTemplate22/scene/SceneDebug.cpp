#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/SceneDebug.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelect.h"
#include "scene/SceneGamePlay.h"
#include "scene/SceneGameClear.h"
#include "scene/SceneGameOver.h"
#include "scene/SceneOption.h"
#include "scene/ScenePause.h"


#include "util/Pad.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;
}

SceneDebug::SceneDebug(SceneManager& manager) :
	SceneBase(manager),
	m_pushCount(0),
	m_isPush(false)
{
	m_sceneTrans = e_SceneTrans::kDebug;
}

SceneDebug::~SceneDebug()
{

}

void SceneDebug::Update()
{

	Pad::Update();
	UpdateFade();

	//
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		if (!m_isPush)
		{
			if (m_sceneTrans != e_SceneTrans::kDebug)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kDebug)
			{
				m_sceneTrans = e_SceneTrans::kPause;
			}
		}
		else
		{
			m_pushCount++;

			if (m_pushCount > 0)
			{
				if (m_sceneTrans != e_SceneTrans::kDebug)
				{
					m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				}
				else if (m_sceneTrans == e_SceneTrans::kDebug)
				{
					m_sceneTrans = e_SceneTrans::kPause;
				}
				m_pushCount = 0;
			}
		}

	}
	else
	{
		m_isPush = false;
	}

	//
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_sceneTrans != e_SceneTrans::kPause)
		{
			m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
		}
		else if (m_sceneTrans == e_SceneTrans::kPause)
		{
			m_sceneTrans = e_SceneTrans::kDebug;
		}
	}

	//
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		if (m_sceneTrans == e_SceneTrans::kDebug)
		{
			m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kTitle)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kSelect)
		{
			m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kGamePlay)
		{
			m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, Game::e_BossKind::kPower, Game::e_StageKind::kGamePlay));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kGameClear)
		{
			m_pManager.ChangeScene(std::make_shared<SceneGameClear>(m_pManager));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kGameOver)
		{
			m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager, Game::e_BossKind::kPower));
			return;
		}

		if (m_sceneTrans == e_SceneTrans::kOption)
		{
			m_pManager.PushScene(std::make_shared<SceneOption>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kPause)
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

}

void SceneDebug::Draw()
{
	DrawFade(0x000000);
	DrawString(0, 0, "Scene Debug", 0xffffff, false);

	DrawFormatString(kTextX * 0.5f, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kDebug) * kTextIntervalY, 0xffffff, "Scene Debug");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Scene Title");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Scene Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGamePlay) * kTextIntervalY, 0xffffff, "Scene Game Play");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGameClear) * kTextIntervalY, 0xffffff, "Scene Game Clear");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGameOver) * kTextIntervalY, 0xffffff, "Scene Game Over");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Scene Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kPause) * kTextIntervalY, 0xffffff, "Scene Pause");
}

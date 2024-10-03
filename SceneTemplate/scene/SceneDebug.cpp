#include "DxLib.h"

#include "SceneManager.h"
#include "SceneDebug.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneGamePlay.h"
#include "SceneGameClear.h"
#include "SceneGameOver.h"
#include "SceneOption.h"


#include "util/Pad.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;
}

SceneDebug::SceneDebug(SceneManager& manager) :
	SceneBase(manager)
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
		if (m_sceneTrans != e_SceneTrans::kDebug)
		{
			m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
		}
	}

	//
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_sceneTrans != e_SceneTrans::kOption)
		{
			m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
		}
	}

	//
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		if (m_sceneTrans == e_SceneTrans::kDebug)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kTitle)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kSelect)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kGamePlay)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kGameClear)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kGameOver)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

		if (m_sceneTrans == e_SceneTrans::kOption)
		{
			m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
		}

	}

}

void SceneDebug::Draw()
{
	DrawFade();
	DrawString(0, 0, "Scene Debug", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kDebug) * kTextIntervalY, 0xffffff, "Scene Debug");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Scene Title");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Scene Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGamePlay) * kTextIntervalY, 0xffffff, "Scene Game Play");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGameClear) * kTextIntervalY, 0xffffff, "Scene Game Clear");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGameOver) * kTextIntervalY, 0xffffff, "Scene Game Over");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Scene Option");
}

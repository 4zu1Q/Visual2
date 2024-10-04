#include "DxLib.h"

#include "util/Pad.h"

#include "SceneSelect.h"
#include "SceneGamePlay.h"
#include "SceneManager.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;
}


SceneSelect::SceneSelect(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kPowerTypeBoss;
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Update()
{
	Pad::Update();
	UpdateFade();


	if (!m_isToNextScene)
	{
		//
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kPowerTypeBoss)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
		}

		//
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kRastBoss)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
		}

		//
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
			if (m_sceneTrans == e_SceneTrans::kSpeedTypeBoss)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
			if (m_sceneTrans == e_SceneTrans::kShooterTypeBoss)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
			if (m_sceneTrans == e_SceneTrans::kHookShotTypeBoss)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
			if (m_sceneTrans == e_SceneTrans::kRastBoss)
			{
				m_pManager.PushScene(std::make_shared<SceneSelect>(m_pManager));
				return;
			}
		}
	}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}
			
			if (m_sceneTrans == e_SceneTrans::kSpeedTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}
			
			if (m_sceneTrans == e_SceneTrans::kShooterTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}

			if (m_sceneTrans == e_SceneTrans::kHookShotTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}

			if (m_sceneTrans == e_SceneTrans::kRastBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			}
		}
	}

}

void SceneSelect::Draw()
{

#ifdef _DEBUG

	DrawString(0, 0, "Scene Select", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kPowerTypeBoss) * kTextIntervalY, 0xffffff, "Power Boss");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSpeedTypeBoss) * kTextIntervalY, 0xffffff, "Speed Boss");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kShooterTypeBoss) * kTextIntervalY, 0xffffff, "Shooter Boss");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kHookShotTypeBoss) * kTextIntervalY, 0xffffff, "HookShot Boss");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kRastBoss) * kTextIntervalY, 0xffffff, "Rast Boss");

#endif

	DrawFade();
}

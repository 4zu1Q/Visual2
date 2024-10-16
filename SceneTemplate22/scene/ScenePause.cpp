#include "DxLib.h"

#include "SceneManager.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "SceneOption.h"

#include "util/Game.h"
#include "util/Pad.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//ポーズの背景アルファ値
	constexpr int kAlpha = 200;
}

ScenePause::ScenePause(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kRestart;
	FadeInSkip();

}

ScenePause::~ScenePause()
{

}

void ScenePause::Update()
{
	//パッドを更新
	Pad::Update();

	//フェードを更新
	UpdateFade();

	if (!m_isToNextScene)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kRestart)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kRestart)
			{
				m_pManager.PopScene();
			}
			if (m_sceneTrans == e_SceneTrans::kOption)
			{
				m_pManager.PushScene(std::make_shared<SceneOption>(m_pManager));
			}
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_isToNextScene = true;
				StartFadeOut();	//フェードアウト開始
			}
		}

		//スタートボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PopScene();
		}
	}

	//シーンフラグが立った場合
	if (m_isToNextScene)
	{
		//フェードアウトのフラグがたっていない場合
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_pManager.ChangeAndClearScene(std::make_shared<SceneTitle>(m_pManager));
				return;
			}
		}
	}
}

void ScenePause::Draw()
{
	DrawFade();

#ifdef _DEBUG

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawString(0, 0, "Scene Pause", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kRestart) * kTextIntervalY, 0xffffff, "Restart");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");

#endif

}

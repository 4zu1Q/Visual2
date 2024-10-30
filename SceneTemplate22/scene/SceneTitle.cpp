#include "DxLib.h"

#include "util/Pad.h"
#include "util/Game.h"

#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneOption.h"


namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//タイトルロゴのポジション
	constexpr VECTOR kTitleLogoPos = { 40.0f , 40.0f };
	
	//スタートのポジション
	constexpr VECTOR kStartPos = { 900.0f , 320.0f };
	//オプションのポジション
	constexpr VECTOR kOptionPos = { 900.0f , 440.0f };
	//やめるのポジション
	constexpr VECTOR kQuitPos = { 900.0f , 560.0f };
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSelect;
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Update()
{
	Pad::Update();
	UpdateFade();

	
	//if (!m_isToNextScene)
	//{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				StartFadeOut();
				m_isToNextScene = true;

				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kOption)
			{
				m_pManager.PushScene(std::make_shared<SceneOption>(m_pManager));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kQuit)
			{
				DxLib_End();
			}
		}
	//}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
			}
		}
	}

}

void SceneTitle::Draw()
{
	//背景座標
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	//タイトル画面の座標
	DrawBox(40, 40, 740, 240, 0xff00ff, true);

	//スタート
	DrawBox(900, 320, 1260, 420, 0xffffff, true);
	//オプション
	DrawBox(900, 440, 1260, 540, 0xffffff, true);
	//エンド
	DrawBox(900, 560, 1260, 660, 0xffffff, true);


//#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Start");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

//#endif



	DrawFade();
}

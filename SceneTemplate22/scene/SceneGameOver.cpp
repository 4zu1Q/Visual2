#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneDebug.h"

#include "util/Pad.h"
#include "util/Game.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;
}

SceneGameOver::SceneGameOver(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSelect;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/GameOver.png"));
	m_handles.push_back(LoadGraph("Data/Image/PleasePressButton1.png"));
	m_handles.push_back(LoadGraph("Data/Image/NewGame2.png"));					//NewGame
	m_handles.push_back(LoadGraph("Data/Image/LoadGame2.png"));				//LoadGame
	m_handles.push_back(LoadGraph("Data/Image/Option2.png"));					//Option
	m_handles.push_back(LoadGraph("Data/Image/End2.png"));					//End
	m_handles.push_back(LoadGraph("Data/Image/Select2.png"));				//矢印
	m_handles.push_back(LoadGraph("Data/Image/Stamp.png"));
}

SceneGameOver::~SceneGameOver()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void SceneGameOver::Update()
{
	Pad::Update();
	UpdateFade();

#ifdef _DEBUG
	//デバッグに遷移する
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}
#endif

	//
	if (!m_isToNextScene)
	{
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
			if (m_sceneTrans != e_SceneTrans::kTitle)
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
			}

			if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				StartFadeOut();
				m_isToNextScene = true;
			}
		}
	}

	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
				return;
			}
		}
	}

}

void SceneGameOver::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x2e8b57, true);

#ifdef _DEBUG

	DrawString(0, 0, "Scene Game Over", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Title");

#endif


	DrawFade(0x000000);
}

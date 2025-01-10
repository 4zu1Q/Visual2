#include "DxLib.h"

#include"SceneManager.h"
#include "SceneGameClear.h"
#include "SceneSelect.h"
#include "SceneTitle.h"
#include "SceneDebug.h"

#include "util/SoundManager.h"
#include "util/Pad.h"
#include "util/Game.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//使う画像の種類
	enum e_Ui
	{
		kGameClearH,
		kSelectH,
		kTitleH,
		kPointerH,
	};
}

SceneGameClear::SceneGameClear(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSelect;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/GameClear.png"));
	m_handles.push_back(LoadGraph("Data/Image/Select.png"));				//Select
	m_handles.push_back(LoadGraph("Data/Image/Title.png"));					//Title
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));				//矢印
}

SceneGameClear::~SceneGameClear()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void SceneGameClear::Update()
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
	if (!m_isToNextScene)
	{
		//左を押した場合
		if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				SoundManager::GetInstance().PlaySe("selectSe");
			}
		}

		//右を押した場合
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			if (m_sceneTrans != e_SceneTrans::kTitle)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				SoundManager::GetInstance().PlaySe("selectSe");
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
				SoundManager::GetInstance().PlaySe("dectionSe");
			}

			if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				StartFadeOut();
				m_isToNextScene = true;
				SoundManager::GetInstance().PlaySe("dectionSe");
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

void SceneGameClear::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x2e8b57, true);

#ifdef _DEBUG




#endif

	//選択
	if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 430, m_handles[kSelect], true);
		//DrawGraph(550, 420, m_handles[kSelectH], true);
		DrawGraph(360, 610, m_handles[kPointerH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kTitle)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 490, m_handles[kSelect], true);
		//DrawGraph(550, 480, m_handles[kSelectH], true);
		DrawGraph(760, 610, m_handles[kPointerH], true);
	}

	DrawGraph(380, 100, m_handles[kGameClearH], true);

	//Select
	DrawGraph(400, 600, m_handles[kSelectH], true);
	//Title
	DrawGraph(800, 600, m_handles[kTitleH], true);

	DrawString(0, 0, "Scene Game Clear", 0xffffff, false);


	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Title");

	DrawFade(0xffffff);
}

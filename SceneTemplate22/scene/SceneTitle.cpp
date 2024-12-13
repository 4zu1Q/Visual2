#include "DxLib.h"

#include "util/Pad.h"
#include "util/Game.h"
#include "util/Vec2.h"

#include "SceneManager.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneGamePlay.h"
#include "SceneOption.h"
#include "SceneDebug.h"

#include "util/SoundManager.h"


namespace
{

	//使う画像の種類
	enum e_Ui
	{
		kLogo,
		kNewGame,
		kLoadGame,
		kOption,
		kQuit,
		kSelect,
	};

	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//タイトルロゴのポジション　※この型で画像などの移動を行っていく
	const Vec2 kTitleLogoPos = { 40.0f , 40.0f };
	
	//スタートのポジション
	constexpr VECTOR kStartPos = { 900.0f , 320.0f };
	//オプションのポジション
	constexpr VECTOR kLoadGamePos = { 900.0f , 440.0f };
	//やめるのポジション
	constexpr VECTOR kQuitPos = { 900.0f , 560.0f };
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSelect;

	//使用する画像を入れていく
	m_handles.push_back(LoadGraph("Data/Image/TitleLogo.png"));
	m_handles.push_back(LoadGraph("Data/Image/Start.png"));
	m_handles.push_back(LoadGraph("Data/Image/Start.png"));
	m_handles.push_back(LoadGraph("Data/Image/Option.png"));
	m_handles.push_back(LoadGraph("Data/Image/End.png"));
	m_handles.push_back(LoadGraph("Data/Image/RB_Push.png"));
	m_handles.push_back(LoadGraph("Data/Image/Stamp.png"));
	m_handles.push_back(LoadGraph("Data/Image/End.png"));

	//SoundManager::GetInstance().Load("")
}

SceneTitle::~SceneTitle()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	m_handles.clear();
}

void SceneTitle::Update()
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
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_sceneTrans = e_SceneTrans::kQuit;
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kQuit)
			{
				m_sceneTrans = e_SceneTrans::kSelect;
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
	}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				//m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
				return;
			}
		}
	}

}

void SceneTitle::Draw()
{
	//背景座標
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x2e8b57, true);

	//タイトル画面の座標
	//DrawBox(40, 40, 740, 240, 0xff00ff, true);
	DrawGraph(255, 200, m_handles[kLogo], true);

	//スタート
	//DrawBox(900, 320, 1260, 420, 0xffffff, true);
	DrawGraph(900, 320, m_handles[kNewGame], true);
	//オプション
	//DrawBox(900, 440, 1260, 540, 0xffffff, true);
	DrawGraph(900, 440, m_handles[kLoadGame], true);
	//エンド
	//DrawBox(900, 560, 1260, 660, 0xffffff, true);
	DrawGraph(900, 560, m_handles[kOption], true);


	DrawGraph(900, 680, m_handles[kQuit], true);


	if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		DrawGraph(820, 320, m_handles[kSelect], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kOption)
	{
		DrawGraph(820, 440, m_handles[kSelect], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kQuit)
	{
		DrawGraph(820, 560, m_handles[kSelect], true);
	}

	DrawGraph(900, 320, m_handles[kNewGame], true);

#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Start");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

#endif



	DrawFade(0x000000);
}

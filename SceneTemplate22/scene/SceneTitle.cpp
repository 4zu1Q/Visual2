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
		kPleasePress,
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
	
	//UIのポジション定数
	constexpr VECTOR kNewGamePos = { 900.0f , 320.0f };
	constexpr VECTOR kLoadGamePos = { 900.0f , 320.0f };
	constexpr VECTOR kOptionPos = { 900.0f , 440.0f };
	constexpr VECTOR EndPos = { 900.0f , 560.0f };
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kSelect;
	m_isStart = false;

	m_startTime = 0;

	//使用する画像を入れていく
	m_handles.push_back(LoadGraph("Data/Image/TitleLogo.png"));
	m_handles.push_back(LoadGraph("Data/Image/PleasePressButton1.png"));
	m_handles.push_back(LoadGraph("Data/Image/NewGame.png"));					//NewGame
	m_handles.push_back(LoadGraph("Data/Image/LoadGame.png"));				//LoadGame
	m_handles.push_back(LoadGraph("Data/Image/Option1.png"));					//Option
	m_handles.push_back(LoadGraph("Data/Image/End1.png"));					//End
	m_handles.push_back(LoadGraph("Data/Image/RB_Push.png"));				//矢印
	m_handles.push_back(LoadGraph("Data/Image/Stamp.png"));					

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

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isStart = true;
	}

	if (m_isStart)
	{
		m_startTime++;
	}
	
	if (!m_isToNextScene && m_isStart && m_startTime >= 180)
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

		//Bボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_2))
		{
			m_isStart = false;
			m_startTime = 0;
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

	if (!m_isStart)
	{
		//ロゴ
		DrawGraph(255, 200, m_handles[kLogo], true);
		DrawGraph(520, 420, m_handles[kPleasePress], true);

	}
	else if(m_isStart)
	{

		//ニューゲーム
		DrawGraph(440, 320, m_handles[kNewGame], true);
		//ロード
		DrawGraph(440, 380, m_handles[kLoadGame], true);
		//オプション
		DrawGraph(440, 440, m_handles[kOption], true);
		//エンド
		DrawGraph(440, 500, m_handles[kQuit], true);

		//選択
		if (m_sceneTrans == e_SceneTrans::kSelect)
		{
			DrawGraph(Game::kScreenWidthHalf - 150, 320, m_handles[kSelect], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kOption)
		{
			DrawGraph(Game::kScreenWidthHalf - 150, 440, m_handles[kSelect], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kQuit)
		{
			DrawGraph(Game::kScreenWidthHalf - 150, 560, m_handles[kSelect], true);
		}
	}

#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Start");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

#endif



	DrawFade(0x000000);
}

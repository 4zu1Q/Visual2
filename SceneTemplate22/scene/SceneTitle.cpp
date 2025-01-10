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
		kLogoH,
		kPleasePressH,
		kNewGameH,
		kLoadGameH,
		kOptionH,
		kQuitH,
		kSelectH,
		kPointerH,
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
	m_sceneTrans = e_SceneTrans::kNewGame;
	m_isStart = false;

	m_startTime = 0;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/TitleLogo.png"));
	m_handles.push_back(LoadGraph("Data/Image/PleasePressButton1.png"));
	m_handles.push_back(LoadGraph("Data/Image/NewGame2.png"));				//NewGame
	m_handles.push_back(LoadGraph("Data/Image/LoadGame2.png"));				//LoadGame
	m_handles.push_back(LoadGraph("Data/Image/Option.png"));				//Option
	m_handles.push_back(LoadGraph("Data/Image/End2.png"));					//End
	m_handles.push_back(LoadGraph("Data/Image/Select2.png"));				//矢印
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));					

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

	SoundManager::GetInstance().PlayBgm("titleBgm", true);


#ifdef _DEBUG

	//デバッグに遷移する
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		SoundManager::GetInstance().StopBgm("titleBgm");
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}

#endif

	if (Pad::IsTrigger(PAD_INPUT_1) && !m_isStart)
	{
		m_isStart = true;
		SoundManager::GetInstance().PlaySe("dectionSe");

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
			if (m_sceneTrans != e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kQuit;
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kQuit)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kNewGame;
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				StartFadeOut();
				m_isToNextScene = true;
			}

			if (m_sceneTrans == e_SceneTrans::kOption)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_pManager.PushScene(std::make_shared<SceneOption>(m_pManager));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kQuit)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
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
			if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				//m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
				SoundManager::GetInstance().StopBgm("titleBgm");
				//m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
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
		DrawGraph(255, 100, m_handles[kLogoH], true);
		DrawGraph(520, 420, m_handles[kPleasePressH], true);

	}
	else if(m_isStart)
	{

		//選択
		if (m_sceneTrans == e_SceneTrans::kNewGame)
		{
			//DrawGraph(Game::kScreenWidthHalf - 150, 430, m_handles[kSelect], true);
			//DrawGraph(550, 420, m_handles[kSelectH], true);
			DrawGraph(520, 430, m_handles[kPointerH], true);
		}
		if (m_sceneTrans == e_SceneTrans::kLoadGame)
		{
			//DrawGraph(Game::kScreenWidthHalf - 150, 490, m_handles[kSelect], true);
			//DrawGraph(550, 480, m_handles[kSelectH], true);
			DrawGraph(520, 490, m_handles[kPointerH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kOption)
		{
			//DrawGraph(Game::kScreenWidthHalf - 150, 550, m_handles[kSelect], true);
			//DrawGraph(550, 540, m_handles[kSelectH], true);
			DrawGraph(550, 550, m_handles[kPointerH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kQuit)
		{
			//DrawGraph(Game::kScreenWidthHalf - 150, 610, m_handles[kSelect], true);
			//DrawGraph(550, 600, m_handles[kSelectH], true);
			DrawGraph(570, 610, m_handles[kPointerH], true);
		}

		DrawGraph(255, 100, m_handles[kLogoH], true);
		//ニューゲーム
		DrawGraph(550, 420, m_handles[kNewGameH], true);
		//ロード
		DrawGraph(550, 480, m_handles[kLoadGameH], true);
		//オプション
		DrawGraph(550, 540, m_handles[kOptionH], true);
		//エンド
		DrawGraph(550, 600, m_handles[kQuitH], true);


	}

#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kNewGame) * kTextIntervalY, 0xffffff, "NewStart");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kLoadGame) * kTextIntervalY, 0xffffff, "LoadStart");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

#endif



	DrawFade(0x000000);
}

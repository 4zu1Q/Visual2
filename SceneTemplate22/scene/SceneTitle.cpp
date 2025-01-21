#include "DxLib.h"

#include "util/Pad.h"
#include "util/Game.h"
#include "util/Vec2.h"

#include "object/player/PlayerProduction.h"
#include "object/CameraProduction.h"
#include "object/stage/SkyDome.h"
#include "object/stage/TitleField.h"
#include "object/stage/Field.h"

#include "scene/SceneManager.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelect.h"
#include "scene/SceneGamePlay.h"
#include "scene/SceneOption.h"
#include "scene/SceneDebug.h"

#include "util/SoundManager.h"
#include "util/SaveDataManager.h"

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
	const Vec2 kTitleLogoPos = { 255 , 100 };
	
	//UIのポジション定数
	const Vec2 kNewGamePos = { 550.0f , 420.0f };
	const Vec2 kLoadGamePos = { 550.0f , 480.0f };
	const Vec2 kOptionPos = { 550.0f , 540.0f };
	const Vec2 kEndPos = { 550.0f , 600.0f };

	const Vec2 kAnyPreesButtonPos = { 520.0f , 420.0f };


	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kNewGame;
	m_isStart = false;
	m_isPlayer = true;
	m_startTime = 0;
	m_selectAnimation = 0.0f;

	m_isActionStart = false;
	m_isActionBack = false;

	m_pPlayerProduction = std::make_shared<PlayerProduction>();
	m_pCameraProduction = std::make_shared<CameraProduction>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pTitleField = std::make_shared<TitleField>();
	m_pField = std::make_shared<Field>(Game::e_StageKind::kTitle);

	m_pPlayerProduction->Initialize(Game::e_PlayerProduction::kTitle);
	m_pCameraProduction->Initialize(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kTitle);

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
	UpdateFadeGraph();

	m_pCameraProduction->Update();
	m_pPlayerProduction->Update(m_isActionStart, m_isActionBack);
	m_pSkyDome->Update();

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
	
	if (!m_isToNextScene && m_isStart && m_startTime >= 60)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				FadeGraphReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kQuit;
				FadeGraphReset();
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				FadeGraphReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kQuit)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kNewGame;
				FadeGraphReset();
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				SaveDataManager::GetInstance().Init();	//セーブデータの初期化
				m_isActionStart = true;
				m_isActionBack = false;
				StartFadeOut();
				m_isToNextScene = true;
			}

			if (m_sceneTrans == e_SceneTrans::kLoadGame)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				SaveDataManager::GetInstance().Load();	//セーブデータの読み込み
				m_isActionStart = true;
				m_isActionBack = false;
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
				SaveDataManager::GetInstance().Save(); //セーブデータの保存

				DxLib_End();
			}
		}

		//Bボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_2))
		{
			//m_isActionBack = true;
			m_isActionStart = false;
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
				SoundManager::GetInstance().StopBgm("titleBgm");
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager,Game::e_StageKind::kSelect));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kLoadGame)
			{
				SoundManager::GetInstance().StopBgm("titleBgm");
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager,Game::e_StageKind::kSelect));
				return;
			}
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;

}

void SceneTitle::Draw()
{
	//背景座標
	m_pCameraProduction->Draw();
	m_pPlayerProduction->Draw();
	m_pSkyDome->Draw();
	m_pTitleField->Draw();
	//m_pField->Draw();

	if (!m_isStart)
	{
		//ロゴ
		DrawGraph(kTitleLogoPos.x, kTitleLogoPos.y, m_handles[kLogoH], true);
		DrawFadeGraph(m_handles[kPleasePressH], kAnyPreesButtonPos);

	}
	else if(m_isStart)
	{

		//選択
		if (m_sceneTrans == e_SceneTrans::kNewGame)
		{
			DrawGraph(520 + m_selectAnimation, 430, m_handles[kPointerH], true);

			//ニューゲーム
			DrawFadeGraph(m_handles[kNewGameH], kNewGamePos);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		if (m_sceneTrans == e_SceneTrans::kLoadGame)
		{
			DrawGraph(520 + m_selectAnimation, 490, m_handles[kPointerH], true);

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawFadeGraph(m_handles[kLoadGameH], kLoadGamePos);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kOption)
		{
			DrawGraph(550 + m_selectAnimation, 550, m_handles[kPointerH], true);

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawFadeGraph(m_handles[kOptionH], kOptionPos);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kQuit)
		{
			DrawGraph(570 + m_selectAnimation, 610, m_handles[kPointerH], true);

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawFadeGraph(m_handles[kQuitH], kEndPos);
		}

		DrawGraph(255, 100, m_handles[kLogoH], true);



	}

#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	//DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kNewGame) * kTextIntervalY, 0xffffff, "NewStart");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kLoadGame) * kTextIntervalY, 0xffffff, "LoadStart");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

#endif



	DrawFade(0x000000);
}
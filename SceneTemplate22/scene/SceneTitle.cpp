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
		kLogoStartH,
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
	const Vec2 kTitleLogoStartPos = { 240 , 0 };
	const Vec2 kTitleLogoPos = { 300 , 100 };
	
	//UIのポジション定数
	const Vec2 kNewGamePos = { 540.0f , 410.0f };
	const Vec2 kLoadGamePos = { 540.0f , 480.0f };
	const Vec2 kOptionPos = { 540.0f , 550.0f };
	const Vec2 kEndPos = { 540.0f , 620.0f };

	const Vec2 kAnyPreesButtonPos = { 240.0f , 550.0f };

	//選択UIのポジション
	const Vec2 kNewGameSelectPos = { 520 , 425 };
	const Vec2 kLoadGameSelectPos = { 520 , 495 };
	const Vec2 kOptionSelectPos = { 540 , 565 };
	const Vec2 kEndSelectPos = { 510 , 635 };

	constexpr float kCursorSpeed = 0.05f;

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneTitle::SceneTitle(SceneManager& manager):
	SceneBase(manager)
{
	m_cursorPos = kNewGameSelectPos;
	m_targetCursorDownPos = kNewGameSelectPos;
	m_targetCursorUpPos = kNewGameSelectPos;

	m_titleLogoPos = kTitleLogoStartPos;


	m_sceneTrans = e_SceneTrans::kNewGame;
	m_isStart = false;
	m_isSkip = false;
	m_isPlayer = true;
	m_startTime = 0;
	m_titleLogoTime = 0;
	m_fadeGraphTime = 0;
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

	m_PlayerPos = m_pPlayerProduction->GetPos();

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/TitleLogoStart.png"));
	m_handles.push_back(LoadGraph("Data/Image/TitleLogo.png"));
	m_handles.push_back(LoadGraph("Data/Image/PleasePressButton.png"));
	m_handles.push_back(LoadGraph("Data/Image/NewGame.png"));				//NewGame
	m_handles.push_back(LoadGraph("Data/Image/LoadGame.png"));				//LoadGame
	m_handles.push_back(LoadGraph("Data/Image/Option.png"));				//Option
	m_handles.push_back(LoadGraph("Data/Image/End.png"));					//End
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
	UpdateFadeSelectGraph();
	UpdateFadeGraphTitleLogo();

	m_pCameraProduction->Update(m_PlayerPos, Game::e_PlayerProduction::kTitle);
	m_pPlayerProduction->Update(m_isActionStart, m_isSkip);
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
	if(!m_isSkip) m_titleLogoTime++;

	if (m_titleLogoTime >= 140)
	{
		m_isSkip = true;
	}

	if (Pad::IsTrigger(PAD_INPUT_1) && !m_isStart && m_isSkip)
	{
		m_isStart = true;
		SoundManager::GetInstance().PlaySe("dectionSe");
		m_fadeGraphTime = 0;
	}

	if (m_isStart)
	{
		m_startTime++;
	}

	if (m_sceneTrans == e_SceneTrans::kNewGame)
	{
		m_cursorPos = kNewGameSelectPos;

		m_targetCursorUpPos = kEndSelectPos;
		m_targetCursorDownPos = kLoadGameSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kLoadGame)
	{
		m_cursorPos = kLoadGameSelectPos;

		m_targetCursorUpPos = kNewGameSelectPos;
		m_targetCursorDownPos = kOptionSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kOption)
	{
		m_cursorPos = kOptionSelectPos;

		m_targetCursorUpPos = kLoadGameSelectPos;
		m_targetCursorDownPos = kEndSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kQuit)
	{
		m_cursorPos = kEndSelectPos;

		m_targetCursorUpPos = kOptionSelectPos;
		m_targetCursorDownPos = kNewGameSelectPos;
	}

	if (!m_isToNextScene && m_isStart && m_startTime >= 60)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kNewGame)
			{
				UpdateCursorUp();
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				FadeGraphSelectReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kNewGame)
			{
				UpdateCursorUp();
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kQuit;
				FadeGraphSelectReset();
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kQuit)
			{

				UpdateCursorDown();
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				FadeGraphSelectReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kQuit)
			{

				UpdateCursorDown();
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kNewGame;
				FadeGraphSelectReset();
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
			SoundManager::GetInstance().PlaySe("backSe");
			m_isActionStart = false;
			m_isStart = false;
			m_isSkip = false;
			m_titleLogoTime = 0;
			m_startTime = 0;
			FadeGraphTitleLogoReset();
			m_fadeGraphTime = 0;

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

	if (m_isSkip && m_isStart)
	{
		// スタート指示を点滅させる
		if (m_fadeGraphTime == 120)
		{
			m_fadeGraphTime++;
		}
		else if (m_fadeGraphTime % 2 == 0)
		{
			m_fadeGraphTime += 2;
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
	//m_pTitleField->Draw();
	m_pField->Draw();

	// フェードしながら描画
	int alpha = static_cast<int>(255 * ((float)m_fadeGraphTime / 120));


	if (!m_isStart)
	{
		//ロゴ
		//DrawGraph(m_titleLogoPos.x, m_titleLogoPos.y, m_handles[kLogoStartH], true);
		DrawFadeGraphTitleLogo(m_handles[kLogoStartH], m_titleLogoPos);

		if (m_isSkip)
		{
			// 画像の描画

			DrawFadeSelectGraph(m_handles[kPleasePressH], kAnyPreesButtonPos);
			DrawGraph(360 + m_selectAnimation, 585, m_handles[kPointerH], true);
			DrawTurnGraph(880 - m_selectAnimation, 585, m_handles[kPointerH], true);
			
		}

	}
	else if(m_isStart)
	{
		//カーソルの描画
		DrawCursor();

		//選択
		if (m_sceneTrans == e_SceneTrans::kNewGame)
		{
			//ニューゲーム
			DrawFadeSelectGraph(m_handles[kNewGameH], kNewGamePos);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		if (m_sceneTrans == e_SceneTrans::kLoadGame)
		{

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawFadeSelectGraph(m_handles[kLoadGameH], kLoadGamePos);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kOption)
		{

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawFadeSelectGraph(m_handles[kOptionH], kOptionPos);
			//エンド
			DrawGraph(kEndPos.x, kEndPos.y, m_handles[kQuitH], true);
		}
		else if (m_sceneTrans == e_SceneTrans::kQuit)
		{

			//ニューゲーム
			DrawGraph(kNewGamePos.x, kNewGamePos.y, m_handles[kNewGameH], true);
			//ロード
			DrawGraph(kLoadGamePos.x, kLoadGamePos.y, m_handles[kLoadGameH], true);
			//オプション
			DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
			//エンド
			DrawFadeSelectGraph(m_handles[kQuitH], kEndPos);
		}

		//タイトルロゴ
		DrawFadeGraphTitleLogo(m_handles[kLogoStartH], m_titleLogoPos);

	}

#ifdef _DEBUG

	DrawString(0, 0, "Scene Title", 0xffffff, false);


	DrawFormatString(0,16,0xffffff,"SkipFlag:%d",m_isSkip);

	//DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kNewGame) * kTextIntervalY, 0xffffff, "NewStart");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kLoadGame) * kTextIntervalY, 0xffffff, "LoadStart");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	//DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kQuit) * kTextIntervalY, 0xffffff, "Quit");

#endif



	DrawFade(0x000000);
}

void SceneTitle::DrawCursor()
{
	DrawGraph(m_cursorPos.x + m_selectAnimation , m_cursorPos.y, m_handles[kPointerH], true);
}

void SceneTitle::UpdateCursorUp()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorUpPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorUpPos.y - m_cursorPos.y) * kCursorSpeed;


}

void SceneTitle::UpdateCursorDown()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorDownPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorDownPos.y - m_cursorPos.y) * kCursorSpeed;


}

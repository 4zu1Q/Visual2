#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/ScenePause.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelect.h"
#include "scene/SceneOption.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/SaveDataManager.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//const char* const kPauseFilename = "Data/Image/Pause2.png";
	const Vec2 kPausePos = { 0.0f , 0.0f };
	const Vec2 kReStartPos = { 160.0f , 180.0f };
	const Vec2 kOptionPos = { 160.0f , 300.0f };
	const Vec2 kSelectPos = { 160.0f , 420.0f };
	const Vec2 kTitlePos = { 160.0f , 540.0f };
	const Vec2 kBackPos = { 0.0f , 685.0f };

	//選択UIのポジション
	const Vec2 kReStartSelectPos = { 110 , 195 };
	const Vec2 kOptionSelectPos = { 110 , 315 };
	const Vec2 kSelectSelectPos = { 110 , 430 };
	const Vec2 kTitleSelectPos = { 110 , 550 };

	//ポーズの背景アルファ値
	constexpr int kAlpha = 200;

	constexpr float kCursorSpeed = 0.05f;

	//使う画像の種類
	enum e_Ui
	{
		kPauseH,
		kReStartH,
		kOptionH,
		kSelectH,
		kTitleH,
		kBackH,
		kPointerH,
		kPauseBarH,
	};

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;

}

ScenePause::ScenePause(SceneManager& manager) :
	SceneBase(manager)
{
	m_cursorPos = kReStartSelectPos;
	m_targetCursorDownPos = kOptionSelectPos;
	m_targetCursorUpPos = kTitleSelectPos;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/Pause.png"));
	m_handles.push_back(LoadGraph("Data/Image/ReStart.png"));
	m_handles.push_back(LoadGraph("Data/Image/Option2.png"));
	m_handles.push_back(LoadGraph("Data/Image/Select.png"));
	m_handles.push_back(LoadGraph("Data/Image/Title.png"));
	m_handles.push_back(LoadGraph("Data/Image/Start_Back.png"));
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));
	m_handles.push_back(LoadGraph("Data/Image/PauseBar.png"));

	m_sceneTrans = e_SceneTrans::kRestart;
	FadeInSkip();
}

ScenePause::~ScenePause()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void ScenePause::Update()
{
	//パッドを更新
	Pad::Update();
	//フェードを更新
	UpdateFade();
	UpdateFadeSelectGraph();

	if (m_sceneTrans == e_SceneTrans::kRestart)
	{
		m_cursorPos = kReStartSelectPos;

		m_targetCursorUpPos = kTitleSelectPos;
		m_targetCursorDownPos = kOptionSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kOption)
	{
		m_cursorPos = kOptionSelectPos;

		m_targetCursorUpPos = kReStartSelectPos;
		m_targetCursorDownPos = kSelectSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		m_cursorPos = kSelectSelectPos;

		m_targetCursorUpPos = kOptionSelectPos;
		m_targetCursorDownPos = kTitleSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kTitle)
	{
		m_cursorPos = kTitleSelectPos;

		m_targetCursorUpPos = kSelectSelectPos;
		m_targetCursorDownPos = kReStartSelectPos;
	}

	if (!m_isToNextScene)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				FadeGraphSelectReset();
				UpdateCursorUp();
			}
			else if (m_sceneTrans == e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kTitle;
				FadeGraphSelectReset();
				UpdateCursorUp();
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kTitle)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				FadeGraphSelectReset();
				UpdateCursorDown();
			}
			else if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kRestart;
				FadeGraphSelectReset();
				UpdateCursorDown();
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_pManager.PopScene();
			}
			if (m_sceneTrans == e_SceneTrans::kOption)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_pManager.PushScene(std::make_shared<SceneOption>(m_pManager));
			}
			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_isToNextScene = true;
				StartFadeOut();	//フェードアウト開始
			}
			if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_isToNextScene = true;
				StartFadeOut();	//フェードアウト開始
				SaveDataManager::GetInstance().Save(); //セーブデータの保存
			}
		}

		//スタートボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			//バックSEを入れる
			SoundManager::GetInstance().PlaySe("backSe");
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
				m_pManager.ChangeAndClearScene(std::make_shared<SceneSelect>(m_pManager,Game::e_StageKind::kSelect));
				return;
			}
			else if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				m_pManager.ChangeAndClearScene(std::make_shared<SceneTitle>(m_pManager));
				return;
			}
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void ScenePause::Draw()
{
	//背景フェード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawGraph(0, 0, m_handles[kPauseBarH], true);

#ifdef _DEBUG

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kRestart) * kTextIntervalY, 0xffffff, "Restart");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Title");

	DrawString(0, 0, "Scene Pause", 0xffffff, false);

#endif

	DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);
	DrawGraph(kBackPos.x, kBackPos.y, m_handles[kBackH], true);

	//カーソルの描画
	DrawCursor();

	//選択
	if (m_sceneTrans == e_SceneTrans::kRestart)
	{

		
		DrawFadeSelectGraph(m_handles[kReStartH], kReStartPos);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kOption)
	{


		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawFadeSelectGraph(m_handles[kOptionH], kOptionPos);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kSelect)
	{

		
		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawFadeSelectGraph(m_handles[kSelectH], kSelectPos);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kTitle)
	{

		
		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		DrawFadeSelectGraph(m_handles[kTitleH], kTitlePos);
	}

	DrawFade(0x000000);

}

void ScenePause::DrawCursor()
{
	DrawGraph(m_cursorPos.x + m_selectAnimation, m_cursorPos.y, m_handles[kPointerH], true);
}

void ScenePause::UpdateCursorUp()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorUpPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorUpPos.y - m_cursorPos.y) * kCursorSpeed;

}

void ScenePause::UpdateCursorDown()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorDownPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorDownPos.y - m_cursorPos.y) * kCursorSpeed;

}

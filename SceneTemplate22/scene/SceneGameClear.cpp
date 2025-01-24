#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/SceneGameClear.h"
#include "scene/SceneSelect.h"
#include "scene/SceneTitle.h"
#include "scene/SceneDebug.h"

#include "object/player/PlayerProduction.h"
#include "object/CameraProduction.h"
#include "object/stage/SkyDome.h"
#include "object/stage/TitleField.h"

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

	const Vec2 kSelectSelectPos = { 360 , 615 };
	const Vec2 kTitleSelectPos = { 760 , 615 };

	const Vec2 kSelectPos = { 400.0f , 600.0f };
	const Vec2 kTitlePos = { 800.0f , 600.0f };


	constexpr float kCursorSpeed = 0.05f;

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneGameClear::SceneGameClear(SceneManager& manager) :
	SceneBase(manager)
{
	m_isActionStart = false;
	m_isActionBack = false;

	m_cursorPos = kSelectSelectPos;
	m_targetCursorPos = kTitleSelectPos;


	m_sceneTrans = e_SceneTrans::kSelect;

	m_pPlayerProduction = std::make_shared<PlayerProduction>();
	m_pCameraProduction = std::make_shared<CameraProduction>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pTitleField = std::make_shared<TitleField>();

	m_pPlayerProduction->Initialize(Game::e_PlayerProduction::kGameClear);
	m_pCameraProduction->Initialize(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kGameClear);

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
	UpdateFadeSelectGraph();

	m_pCameraProduction->Update(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kGameClear);
	m_pPlayerProduction->Update(m_isActionStart, m_isActionBack);
	m_pSkyDome->Update();

	if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		m_cursorPos = kSelectSelectPos;

		m_targetCursorPos = kTitleSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kTitle)
	{
		m_cursorPos = kTitleSelectPos;

		m_targetCursorPos = kSelectSelectPos;
	}

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
				FadeGraphSelectReset();
				UpdateCursorLeft();
			}
			else if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_sceneTrans = e_SceneTrans::kTitle;
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
				UpdateCursorLeft();
			}
		}

		//右を押した場合
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			if (m_sceneTrans != e_SceneTrans::kTitle)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
				UpdateCursorRight();
			}
			else if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				m_sceneTrans = e_SceneTrans::kSelect;
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
				UpdateCursorRight();
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
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
				return;
			}

			if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
				return;
			}
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneGameClear::Draw()
{
	//DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x2e8b57, true);
	m_pTitleField->Draw();

	//背景座標
	m_pCameraProduction->Draw();
	m_pPlayerProduction->Draw();
	m_pSkyDome->Draw();

#ifdef _DEBUG




#endif
	//
	DrawCursor();

	//選択
	if (m_sceneTrans == e_SceneTrans::kSelect)
	{

		//Select
		DrawFadeSelectGraph(m_handles[kSelectH], kSelectPos);
		//Title
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kTitle)
	{

		//Select
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		//Title
		DrawFadeSelectGraph(m_handles[kTitleH], kTitlePos);
	}

	DrawGraph(380, 100, m_handles[kGameClearH], true);


	DrawString(0, 0, "Scene Game Clear", 0xffffff, false);


	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Title");

	DrawFade(0xffffff);
}

void SceneGameClear::DrawCursor()
{
	DrawGraph(m_cursorPos.x + m_selectAnimation, m_cursorPos.y, m_handles[kPointerH], true);
}

void SceneGameClear::UpdateCursorRight()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorPos.x - m_cursorPos.x) * kCursorSpeed;
}

void SceneGameClear::UpdateCursorLeft()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorPos.x - m_cursorPos.x) * kCursorSpeed;
}

﻿#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/SceneGameOver.h"
#include "scene/SceneSelect.h"
#include "scene/SceneGamePlay.h"
#include "scene/SceneTitle.h"
#include "scene/SceneDebug.h"
#include "scene/SceneTutorial.h"

#include "object/player/PlayerProduction.h"
#include "object/CameraProduction.h"

#include "util/SoundManager.h"
#include "util/Pad.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//使う画像の種類
	enum e_Ui
	{
		kGameOverH,
		kSelectH,
		kTitleH,
		kGamePlayH,
		kPointerH,
		kGameOverBgH,
	};

	const Vec2 kGamePlaySelectPos = { 360 , 615 };
	const Vec2 kSelectSelectPos = { 760 , 615 };

	constexpr float kCursorSpeed = 0.05f;

	const Vec2 kSelectPos = { 800.0f , 600.0f };
	const Vec2 kGamePlayPos = { 400.0f , 600.0f };

	const Vec2 kGameOverPos = { 380.0f , 100.0f };
	const Vec2 kGameOverBgPos = { 0.0f , -10.0f };

	constexpr VECTOR kPlayerPowerTombPos = { -168.0f,-37.0f,624.0f };
	constexpr VECTOR kPlayerSpeedTombPos = { 979.0f,-37.0f,608.0f };
	constexpr VECTOR kPlayerShotTombPos = { 409.0f,-37.0f,363.0f };
	constexpr VECTOR kPlayerRassTombPos = { 402.0f,-37.0f,450.0f };

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneGameOver::SceneGameOver(SceneManager& manager, Game::e_BossKind bossKind) :
	SceneBase(manager),
	m_fadeTime(0)
{
	m_cursorPos = kGamePlaySelectPos;
	m_targetCursorPos = kSelectSelectPos;

	m_isToNextScene = false;

	m_isActionStart = false;
	m_isActionBack = false;

	m_sceneTrans = e_SceneTrans::kGamePlay;
	m_bossKind = bossKind;

	m_pPlayerProduction = std::make_shared<PlayerProduction>();
	m_pCameraProduction = std::make_shared<CameraProduction>();

	m_pPlayerProduction->Initialize(Game::e_PlayerProduction::kGameOver);
	m_pCameraProduction->Initialize(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kGameOver);

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/GameOver.png"));
	m_handles.push_back(LoadGraph("Data/Image/Select.png"));				//Select
	m_handles.push_back(LoadGraph("Data/Image/Title.png"));				//
	m_handles.push_back(LoadGraph("Data/Image/Retry.png"));					//Title
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));				//矢印
	m_handles.push_back(LoadGraph("Data/Image/GameOverBg.png"));				//

	SoundManager::GetInstance().PlayBgm("gameOverBgm", true);

}

SceneGameOver::~SceneGameOver()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	m_handles.clear();

	SoundManager::GetInstance().StopBgm("gameOverBgm");

}

void SceneGameOver::Update()
{
	Pad::Update();
	UpdateFade();
	UpdateFadeSelectGraph();

	m_pCameraProduction->Update(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kGameOver);
	m_pPlayerProduction->Update(m_isActionStart, m_isActionBack);


#ifdef _DEBUG
	//デバッグに遷移する
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}
#endif

	if (m_sceneTrans == e_SceneTrans::kGamePlay)
	{
		m_cursorPos = kGamePlaySelectPos;

		m_targetCursorPos = kSelectSelectPos;
	}
	else if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		m_cursorPos = kSelectSelectPos;

		m_targetCursorPos = kGamePlaySelectPos;
	}

	if (!m_isToNextScene)
	{
		//左を押した場合
		if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			if (m_sceneTrans != e_SceneTrans::kGamePlay)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kGamePlay)
			{
				m_sceneTrans = e_SceneTrans::kSelect;
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
				UpdateCursorLeft();
			}
		}

		//右を押した場合
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				m_sceneTrans = e_SceneTrans::kGamePlay;
				SoundManager::GetInstance().PlaySe("selectSe");
				FadeGraphSelectReset();
				UpdateCursorLeft();
			}
		}

		//Aボタンを押した場合
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			//enum変数が同じだった場合
			if (m_sceneTrans == e_SceneTrans::kGamePlay)
			{
				StartFadeOut();
				m_isToNextScene = true;
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_isActionStart = true;
			}

			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				StartFadeOut();
				m_isToNextScene = true;
				SoundManager::GetInstance().PlaySe("dectionSe");
				m_isActionStart = true;
			}
		}
	}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kGamePlay)
			{
				if (m_bossKind == Game::e_BossKind::kTutorial)
				{
					m_pManager.ChangeScene(std::make_shared<SceneTutorial>(m_pManager, Game::e_StageKind::kTutorial));
					return;
				}
				else
				{
					m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, m_bossKind, Game::e_StageKind::kGamePlay));
					return;
				}
			}

			if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				if (m_bossKind == Game::e_BossKind::kTutorial)
				{
					m_pManager.ChangeScene(std::make_shared<SceneTitle>(m_pManager));
					return;
				}
				else if (m_bossKind == Game::e_BossKind::kPower)
				{
					m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerPowerTombPos));
					return;
				}
				else if (m_bossKind == Game::e_BossKind::kSpeed)
				{
					m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerSpeedTombPos));
					return;
				}
				else if (m_bossKind == Game::e_BossKind::kShot)
				{
					m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerShotTombPos));
					return;
				}
				else if (m_bossKind == Game::e_BossKind::kRast)
				{
					m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerRassTombPos));
					return;
				}
			}
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneGameOver::Draw()
{
	//背景
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	m_pCameraProduction->Draw();
	m_pPlayerProduction->Draw();

	DrawCursor();

	//選択
	if (m_bossKind == Game::e_BossKind::kTutorial)
	{
		if (m_sceneTrans == e_SceneTrans::kGamePlay)
		{
			//GamePlay
			DrawFadeSelectGraph(m_handles[kGamePlayH], kGamePlayPos);
			//Select
			DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kTitleH], true);
		}

		if (m_sceneTrans == e_SceneTrans::kSelect)
		{
			//GamePlay
			DrawGraph(kGamePlayPos.x, kGamePlayPos.y, m_handles[kGamePlayH], true);
			//Select
			DrawFadeSelectGraph(m_handles[kTitleH], kSelectPos);
		}
	}
	else
	{
		if (m_sceneTrans == e_SceneTrans::kGamePlay)
		{
			//GamePlay
			DrawFadeSelectGraph(m_handles[kGamePlayH], kGamePlayPos);
			//Select
			DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		}

		if (m_sceneTrans == e_SceneTrans::kSelect)
		{
			//GamePlay
			DrawGraph(kGamePlayPos.x, kGamePlayPos.y, m_handles[kGamePlayH], true);
			//Select
			DrawFadeSelectGraph(m_handles[kSelectH], kSelectPos);
		}
	}


	DrawGraph(kGameOverBgPos.x, kGameOverBgPos.y + m_selectAnimation, m_handles[kGameOverBgH], true);
	DrawGraph(kGameOverPos.x, kGameOverPos.y, m_handles[kGameOverH], true);

	DrawFade(0x000000);

#ifdef _DEBUG
	DrawString(0, 0, "Scene Game Over", 0xffffff, false);
	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGamePlay) * kTextIntervalY, 0xffffff, "Retry");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");
#endif
}

void SceneGameOver::DrawCursor()
{
	DrawGraph(m_cursorPos.x + m_selectAnimation, m_cursorPos.y, m_handles[kPointerH], true);
}

void SceneGameOver::UpdateCursorRight()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorPos.x - m_cursorPos.x) * kCursorSpeed;
}

void SceneGameOver::UpdateCursorLeft()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorPos.x - m_cursorPos.x) * kCursorSpeed;
}
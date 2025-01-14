﻿#include "DxLib.h"

#include "SceneManager.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "SceneOption.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//const char* const kPauseFilename = "Data/Image/Pause2.png";
	const Vec2 kPausePos = { 0.0f , 0.0f };



	//ポーズの背景アルファ値
	constexpr int kAlpha = 200;

	//使う画像の種類
	enum e_Ui
	{
		kPauseH,
		kReStartH,
		kOptionH,
		kTitleH,
		kBackH,
		kPointerH,
	};

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

ScenePause::ScenePause(SceneManager& manager) :
	SceneBase(manager)
{
	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/Pause2.png"));
	m_handles.push_back(LoadGraph("Data/Image/ReStart.png"));
	m_handles.push_back(LoadGraph("Data/Image/Option2.png"));
	m_handles.push_back(LoadGraph("Data/Image/Title.png"));
	m_handles.push_back(LoadGraph("Data/Image/Start_Back.png"));
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));

	m_sceneTrans = e_SceneTrans::kRestart;
	FadeInSkip();
}

ScenePause::~ScenePause()
{

}

void ScenePause::Update()
{
	//パッドを更新
	Pad::Update();
	//フェードを更新
	UpdateFade();

	if (!m_isToNextScene)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kSelect;
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
			}
			else if (m_sceneTrans == e_SceneTrans::kSelect)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kRestart;
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
	//DrawBlackFade();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x00000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG





	DrawString(0, 0, "Scene Pause", 0xffffff, false);


#endif

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kRestart) * kTextIntervalY, 0xffffff, "Restart");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kOption) * kTextIntervalY, 0xffffff, "Option");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");


	DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);
	DrawGraph(100, 60, m_handles[kReStartH], true);
	DrawGraph(100, 180, m_handles[kOptionH], true);
	DrawGraph(100, 300, m_handles[kTitleH], true);
	DrawGraph(0, 660, m_handles[kBackH], true);

	//選択
	if (m_sceneTrans == e_SceneTrans::kRestart)
	{
		DrawGraph(50 + m_selectAnimation, 70, m_handles[kPointerH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kOption)
	{
		DrawGraph(50 + m_selectAnimation, 190, m_handles[kPointerH], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		DrawGraph(50 + m_selectAnimation, 310, m_handles[kPointerH], true);
	}

	DrawFade(0x000000);

}

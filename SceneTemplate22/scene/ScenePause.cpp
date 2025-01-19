#include "DxLib.h"

#include "SceneManager.h"
#include "ScenePause.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
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
	const Vec2 kReStartPos = { 160.0f , 120.0f };
	const Vec2 kOptionPos = { 160.0f , 240.0f };
	const Vec2 kSelectPos = { 160.0f , 360 };
	const Vec2 kTitlePos = { 160.0f , 480.0f };
	const Vec2 kBackPos = { 0.0f , 660.0f };



	//ポーズの背景アルファ値
	constexpr int kAlpha = 200;

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
	UpdateFadeGraph();

	if (!m_isToNextScene)
	{
		//上を押した場合
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{
			if (m_sceneTrans != e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				FadeGraphReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kRestart)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kSelect;
				FadeGraphReset();
			}
		}

		//下を押した場合
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_sceneTrans != e_SceneTrans::kTitle)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				FadeGraphReset();
			}
			else if (m_sceneTrans == e_SceneTrans::kTitle)
			{
				SoundManager::GetInstance().PlaySe("selectSe");
				m_sceneTrans = e_SceneTrans::kRestart;
				FadeGraphReset();
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
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kTitle) * kTextIntervalY, 0xffffff, "Title");



	DrawGraph(kBackPos.x, kBackPos.y, m_handles[kBackH], true);

	//選択
	if (m_sceneTrans == e_SceneTrans::kRestart)
	{
		DrawGraph(110 + m_selectAnimation, 130, m_handles[kPointerH], true);

		DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);
		
		DrawFadeGraph(m_handles[kReStartH], kReStartPos);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kTitleH], true);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kOption)
	{
		DrawGraph(110 + m_selectAnimation, 250, m_handles[kPointerH], true);

		DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);

		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawFadeGraph(m_handles[kOptionH], kOptionPos);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kTitleH], true);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		DrawGraph(110 + m_selectAnimation, 370, m_handles[kPointerH], true);

		DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);
		
		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawFadeGraph(m_handles[kSelectH], kSelectPos);
		DrawGraph(kTitlePos.x, kTitlePos.y, m_handles[kTitleH], true);
	}
	else if (m_sceneTrans == e_SceneTrans::kTitle)
	{
		DrawGraph(110 + m_selectAnimation, 490, m_handles[kPointerH], true);

		DrawGraph(kPausePos.x, kPausePos.y, m_handles[kPauseH], true);
		
		DrawGraph(kReStartPos.x, kReStartPos.y, m_handles[kReStartH], true);
		DrawGraph(kOptionPos.x, kOptionPos.y, m_handles[kOptionH], true);
		DrawGraph(kSelectPos.x, kSelectPos.y, m_handles[kSelectH], true);
		DrawFadeGraph(m_handles[kTitleH], kTitlePos);
	}

	DrawFade(0x000000);

}

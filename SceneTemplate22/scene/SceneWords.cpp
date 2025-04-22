#include "SceneWords.h"

#include "scene/SceneManager.h"
#include "scene/ScenePause.h"
#include "scene/SceneTitle.h"
#include "scene/SceneSelect.h"
#include "scene/SceneOption.h"

#include "util/Pad.h"
#include "util/Font.h"
#include "util/SoundManager.h"

namespace
{
	//フォントのパス
	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";
	//フォントサイズ	
	constexpr int kFontSize = 38;

	//チュートリアルのフレーム数
	constexpr int kTutorialFrame = 120;
	constexpr int kTutorialBossFrame = 180;

	//チュートリアルの説明を行う位置
	constexpr float kTutorialJumpPosZ = 620.0f;
	constexpr float kTutorialJumpClearPosZ = 506.0f;
	constexpr float kTutorialDashJumpPosZ = 317.0f;
	constexpr float kTutorialDashJumpClearPosZ = 88.0f;
	constexpr float kTutorialAttackXPosZ = -9.0f;
	constexpr float kTutorialAttackYPosZ = -242.0f;
	constexpr float kTutorialBossBattlePosZ = -455.0f;

	//チュートリアルのセリフの座標
	const Vec2 kWordsPos = { 130.0f , 50.0f };
	const Vec2 kMessageCursorPos = { 619.0f , 320.0f };

	//調整する座標
	const Vec2 kHitAdjustmentPos = { 25.0f , 40.0f };
	const Vec2 kHitAdjustmentUpPos = { 25.0f , 80.0f };

	constexpr float kSelectSpeed = 0.1f;
	constexpr float kSelectAnimationSize = 4.0f;

	enum e_Ui
	{
		kStart01H,
		kStart02H,
		kStart03H,
		kStart04H,
		kStart05H,
		kJump01H,
		kJumpClear01H,
		kDashJump01H,
		kDashJumpClear01H,
		kActionClear01H,
		kBattle01H,
		kBattle02H,
		kBattle03H,
		kBattleClear01H,
		kBattleClear02H,
		kMask01H,
		kMask02H,
		kCursorH,
		kSerihuH,
	};
}

SceneWords::SceneWords(SceneManager& manager, Game::e_TutorialProgress progress):
	SceneBase(manager),
	m_wordsNum(0),
	m_skipTime(0),
	m_selectAnimation(0.0f)
{
	m_tutorialProgress = progress;

	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start02.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start03.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start04.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start05.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Jump01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Jump02.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/DashJump01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/DashJump02.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/ActionClear01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Battle01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Battle02.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Battle03.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Battle04.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Battle05.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Mask01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Mask02.png"));
	m_handles.push_back(LoadGraph("Data/Image/MessageCursor.png"));
	m_handles.push_back(LoadGraph("Data/Image/TutorialSerihu1.png"));

	m_fontH = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", 32);


}

SceneWords::~SceneWords()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}
	m_handles.clear();

}

void SceneWords::Update()
{
	Pad::Update();
	UpdateFade();
	UpdateFadeSelectGraph();
	UpdateFadeGraphTitleLogo();

	//タイマーを使う
	m_skipTime++;

	if (m_skipTime > 60)
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			m_wordsNum++;	//プラス1
			m_skipTime = 0;	//スキップタイムをリセット
			SoundManager::GetInstance().PlaySe("dectionSe");
			FadeGraphTitleLogoReset();
		}

	}

	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialStart)
	{
		//メンバ関数ポインタでチュートリアルスタートの関数に移動する
		CloseWords(4);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJump)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJumpClear)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJump)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJumpClear)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialActionClear)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBoss)
	{
		CloseWords(3);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBossClear)
	{
		CloseWords(2);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialMask)
	{
		CloseWords(1);
	}

	//スタートボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		//バックSEを入れる
		SoundManager::GetInstance().PlaySe("backSe");
		m_pManager.PopScene();
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneWords::Draw()
{
	DrawGraph(kWordsPos.x, kWordsPos.y, m_handles[kSerihuH], true);

	DrawFormatString(0, 450, 0xffffff, "%d", m_wordsNum);

	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialStart)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart02H], kWordsPos);
		}
		else if (m_wordsNum == 2)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart03H], kWordsPos);
		}
		else if (m_wordsNum == 3)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart04H], kWordsPos);
		}
		else if (m_wordsNum == 4)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart05H], kWordsPos);
		}
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJump)
	{
		DrawFadeGraphTitleLogo(m_handles[kJump01H], kWordsPos);
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJumpClear)
	{
		DrawFadeGraphTitleLogo(m_handles[kJumpClear01H], kWordsPos);
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJump)
	{
		DrawFadeGraphTitleLogo(m_handles[kDashJump01H], kWordsPos);
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJumpClear)
	{
		DrawFadeGraphTitleLogo(m_handles[kDashJumpClear01H], kWordsPos);
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialActionClear)
	{
		DrawFadeGraphTitleLogo(m_handles[kActionClear01H], kWordsPos);
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBoss)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle02H], kWordsPos);
		}
		else if (m_wordsNum == 2)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle03H], kWordsPos);
		}

	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBossClear)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattleClear01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattleClear02H], kWordsPos);
		}
	}
	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialMask)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kMask01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kMask02H], kWordsPos);
		}
	}

	//タイムが以上になったら表示
	if (m_skipTime > 60)
	{
		DrawGraph(kMessageCursorPos.x, kMessageCursorPos.y + m_selectAnimation, m_handles[kCursorH], true);
	}
}

void SceneWords::CloseWords(int wordsNum)
{
	if (m_wordsNum >= wordsNum)
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			m_wordsNum = 0;
			m_pManager.PopScene();
		}
	}
}


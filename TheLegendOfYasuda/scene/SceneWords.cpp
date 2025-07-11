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
	const Vec2 kOperationPos= { 310.0f , 100.0f };
	const Vec2 kWordsCursorPos = { 619.0f , 320.0f };
	const Vec2 kOperationCursorPos = { 619.0f , 520.0f };

	const Vec2 kMoveFramePos = { 485.0f , 270.0f };
	const Vec2 kCameraRightFramePos = { 708.0f , 390.0f };
	const Vec2 kCameraLeftFramePos = { 453.0f , 390.0f };
	const Vec2 kJumpFramePos = { 627.0f , 310.0f };
	const Vec2 kDashFramePos = { 722.0f , 320.0f };
	const Vec2 kAttackRightFramePos = { 695.0f , 305.0f };
	const Vec2 kAttackLeftFramePos = { 447.0f , 272.0f };
	const Vec2 kLockOnFramePos = { 592.0f , 282.0f };

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
		kStart06H,
		kJump01H,
		kJumpClear01H,
		kJumpClear02H,
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
		kTutorialMoveH,
		kTutorialCameraH,
		kTutorialJumpH,
		kTutorialDashH,
		kTutorialAttackH,
		kTutorialLockOnH,
		kTutorialMaskH,
		kTutorialButtonFrameH,
		kTutorialStickFrameH,
	};
}

SceneWords::SceneWords(SceneManager& manager, Game::e_TutorialProgress progress):
	SceneBase(manager),
	m_wordsNum(0),
	m_skipTime(0),
	m_selectAnimation(0.0f)
{
	m_tutorialProgress = progress;
	m_isBgDraw = false;

	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start02.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start03.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start04.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start05.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Start06.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Jump01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/JumpClear01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/Dash01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/DashJump01.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/DashJumpClear01.png"));
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
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/MoveTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/CameraTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/JumpTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/DashTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/AttackTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/LockOnTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/MaskTutorial.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/ButtonFrame.png"));
	m_handles.push_back(LoadGraph("Data/Image/Tutorial/StickFrame.png"));

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


	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialStart)
	{
		//メンバ関数ポインタでチュートリアルスタートの関数に移動する
		CloseWords(7);
		TutorialBgFlag(6);
		TutorialSe(5);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJump)
	{
		CloseWords(1);
		TutorialBgFlag(1);
		TutorialSe(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJumpClear)
	{
		CloseWords(2);
		TutorialBgFlag(2);
		TutorialSe(1);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJump)
	{
		CloseWords(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJumpClear)
	{
		CloseWords(1);
		TutorialBgFlag(1);
		TutorialSe(0);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBoss)
	{
		CloseWords(4);
		TutorialBgFlag(4);
		TutorialSe(3);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBossClear)
	{
		CloseWords(1);
	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialMask)
	{
		CloseWords(1);
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneWords::Draw()
{
	if (!m_isBgDraw)
	{
		DrawGraph(kWordsPos.x, kWordsPos.y, m_handles[kSerihuH], true);
	}

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
		else if (m_wordsNum == 5)
		{
			DrawFadeGraphTitleLogo(m_handles[kStart06H], kWordsPos);
		}
		else if (m_wordsNum == 6)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialMoveH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialStickFrameH], kMoveFramePos);
		}
		else if (m_wordsNum == 7)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialCameraH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialStickFrameH], kCameraRightFramePos);
			DrawFadeSelectGraph(m_handles[kTutorialStickFrameH], kCameraLeftFramePos);
		}
	}


	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJump)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kJump01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialJumpH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialButtonFrameH], kJumpFramePos);
		}
	}


	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJumpClear)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kJumpClear01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kJumpClear02H], kWordsPos);
		}
		else if (m_wordsNum == 2)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialDashH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialButtonFrameH], kDashFramePos);
		}
	}


	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJump)
	{
		DrawFadeGraphTitleLogo(m_handles[kDashJump01H], kWordsPos);
	}


	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJumpClear)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kDashJumpClear01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialAttackH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialButtonFrameH], kAttackRightFramePos);
			DrawFadeSelectGraph(m_handles[kTutorialButtonFrameH], kAttackLeftFramePos);
		}
	}

	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBoss)
	{
		if (m_wordsNum == 0)
		{
			DrawFadeGraphTitleLogo(m_handles[kActionClear01H], kWordsPos);
		}
		else if (m_wordsNum == 1)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle01H], kWordsPos);
		}
		else if (m_wordsNum == 2)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle02H], kWordsPos);
		}
		else if (m_wordsNum == 3)
		{
			DrawFadeGraphTitleLogo(m_handles[kBattle03H], kWordsPos);
		}
		else if (m_wordsNum == 4)
		{
			DrawFadeGraphTitleLogo(m_handles[kTutorialLockOnH], kOperationPos);
			DrawFadeSelectGraph(m_handles[kTutorialStickFrameH], kLockOnFramePos);
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
		if (!m_isBgDraw)
		{
			DrawGraph(kWordsCursorPos.x, kWordsCursorPos.y + m_selectAnimation, m_handles[kCursorH], true);
		}
		else
		{
			DrawGraph(kOperationCursorPos.x, kOperationCursorPos.y + m_selectAnimation, m_handles[kCursorH], true);
		}
	}
}

void SceneWords::CloseWords(int wordsNum)
{

	if (m_skipTime > 60)
	{
		if (m_wordsNum == wordsNum)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				m_wordsNum = 0;
				SoundManager::GetInstance().PlaySe("dectionSe");
				FadeGraphTitleLogoReset();
				m_pManager.PopScene();
			}
		}
		else if (m_wordsNum != wordsNum)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				m_wordsNum++;	//プラス1
				m_skipTime = 0;	//スキップタイムをリセット
				SoundManager::GetInstance().PlaySe("dectionSe");
				FadeGraphTitleLogoReset();
			}
		}
	}

}

void SceneWords::TutorialSe(int wordsNum)
{
	if (m_wordsNum == wordsNum)
	{
		SoundManager::GetInstance().PlaySe("tutorialSe");
	}
}

void SceneWords::TutorialBgFlag(int wordsNum)
{
	if (m_wordsNum == wordsNum)
	{
		m_isBgDraw = true;
	}
}


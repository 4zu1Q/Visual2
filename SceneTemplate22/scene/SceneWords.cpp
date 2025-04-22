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

	//エフェクトを出すフレーム数
	constexpr int kEffectFrame = 190;
	//チュートリアルのフレーム数
	constexpr int kTutorialFrame = 120;
	constexpr int kTutorialBossFrame = 180;

	//初期位置
	constexpr VECTOR kInitPos = { 400.0f,-35.0f,740.0f };
	constexpr VECTOR kLookPos = { -100.0f,30.0f,500.0f };

	//チュートリアルの説明を行う位置
	constexpr float kTutorialJumpPosZ = 620.0f;
	constexpr float kTutorialJumpClearPosZ = 506.0f;
	constexpr float kTutorialDashJumpPosZ = 317.0f;
	constexpr float kTutorialDashJumpClearPosZ = 88.0f;
	constexpr float kTutorialAttackXPosZ = -9.0f;
	constexpr float kTutorialAttackYPosZ = -242.0f;
	constexpr float kTutorialBossBattlePosZ = -455.0f;

	//ボスに行く部屋に表示される画像の座標
	const Vec2 kHitPos = { 440.0f , 480.0f };
	const Vec2 kHitBossPos = { 340.0f , 30.0f };
	const Vec2 kHitTextPos = { 740.0f , 230.0f };
	const Vec2 kHitText2Pos = { 743.0f , 230.0f };
	const Vec2 kHitItemPos = { 790.0f , 160.0f };
	const Vec2 kHitStarPos = { 790.0f , 220.0f };

	//調整する座標
	const Vec2 kHitAdjustmentPos = { 25.0f , 40.0f };
	const Vec2 kHitAdjustmentUpPos = { 25.0f , 80.0f };

	//プレイヤーの最初の位置
	constexpr VECTOR kPlayerPos = { 383.0f,-410.0f,670.0f };
	constexpr VECTOR kSelectPlayerPos = { 400.0f,-35.0f,740.0f };

	constexpr int kShadowMapSize = 16384;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = { -10000.0f, -500.0f, -10000.0f };		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = { 10000.0f, 0.0f, 10000.0f };	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = { 0.0f, -5.0f, 0.0f };					// ライト方向

	constexpr float kShadowColor = 0.7f;
	constexpr float kShadowAlpha = 0.3f;

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
	m_skipTime(0)
{
	//m_tutorialProgress = progress;

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
	m_handles.push_back(LoadGraph("Data/Image/messageCursor.png"));
	m_handles.push_back(LoadGraph("Data/Image/TutorialSerihu.png"));

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
		}

	}

	if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialStart)
	{
		//メンバ関数ポインタでチュートリアルスタートの関数に移動する
		Next();	
		CloseWords(4);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJump)
	{
		Next();
		CloseWords(0);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialJumpClear)
	{
		Next();
		CloseWords(0);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJump)
	{
		Next();
		CloseWords(0);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialDashJumpClear)
	{
		Next();
		CloseWords(0);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialActionClear)
	{
		Next();
		CloseWords(0);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBoss)
	{
		Next();
		CloseWords(2);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialBossClear)
	{
		Next();
		CloseWords(1);

	}
	else if (m_tutorialProgress == Game::e_TutorialProgress::kTutorialMask)
	{
		Next();
		CloseWords(1);

	}

	//スタートボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		//バックSEを入れる
		SoundManager::GetInstance().PlaySe("backSe");
		m_pManager.PopScene();
	}
}

void SceneWords::Draw()
{
	DrawGraph(130, 50, m_handles[kSerihuH], true);

	switch (m_tutorialProgress)
	{

	case Game::e_TutorialProgress::kTutorialStart:
		
		//Aボタンを押したら次のセリフに行く
		if (m_wordsNum == 0)
		{
			DrawGraph(130, 50, m_handles[kStart01H], true);
		}
		else if (m_wordsNum == 1)
		{
			DrawGraph(130, 50, m_handles[kStart02H], true);
		}
		else if (m_wordsNum == 2)
		{
			DrawGraph(130, 50, m_handles[kStart03H], true);
		}
		else if (m_wordsNum == 3)
		{
			DrawGraph(130, 50, m_handles[kStart04H], true);
		}
		else if (m_wordsNum == 4)
		{
			DrawGraph(130, 50, m_handles[kStart05H], true);
		}

		//switch (m_wordsNum)
		//{
		//case 0:
		//	DrawGraph(130, 50, m_handles[kStart01H], true);
		//	break;
		//case 1:
		//	DrawGraph(130, 50, m_handles[kStart02H], true);
		//	break;
		//case 2:
		//	DrawGraph(130, 50, m_handles[kStart03H], true);
		//	break;
		//case 3:
		//	DrawGraph(130, 50, m_handles[kStart04H], true);
		//	break;
		//case 4:
		//	DrawGraph(130, 50, m_handles[kStart05H], true);
		//	break;
		//default:
		//	break;
		//}

		break;
	case Game::e_TutorialProgress::kTutorialJump:
		DrawGraph(130, 50, m_handles[kJump01H], true);

		break;
	case Game::e_TutorialProgress::kTutorialJumpClear:
		DrawGraph(130, 50, m_handles[kJumpClear01H], true);

		break;
	case Game::e_TutorialProgress::kTutorialDashJump:
		DrawGraph(130, 50, m_handles[kDashJump01H], true);

		break;
	case Game::e_TutorialProgress::kTutorialDashJumpClear:
		DrawGraph(130, 50, m_handles[kDashJumpClear01H], true);

		break;
	case Game::e_TutorialProgress::kTutorialActionClear:
		DrawGraph(130, 50, m_handles[kActionClear01H], true);

		break;
	case Game::e_TutorialProgress::kTutorialBoss:

		switch (m_wordsNum)
		{
		case 0:
			DrawGraph(130, 50, m_handles[kBattle01H], true);
			break;
		case 1:
			DrawGraph(130, 50, m_handles[kBattle02H], true);
			break;
		case 2:
			DrawGraph(130, 50, m_handles[kBattle03H], true);
			break;
		default:
			break;
		}

		break;
	case Game::e_TutorialProgress::kTutorialBossClear:

		switch (m_wordsNum)
		{
		case 0:
			DrawGraph(130, 50, m_handles[kBattleClear01H], true);
			break;
		case 1:
			DrawGraph(130, 50, m_handles[kBattleClear02H], true);
			break;
		}

		break;
	case Game::e_TutorialProgress::kTutorialMask:

		switch (m_wordsNum)
		{
		case 0:
			DrawGraph(130, 50, m_handles[kMask01H], true);
			break;
		case 1:
			DrawGraph(130, 50, m_handles[kMask02H], true);
			break;
		}

		break;
	default:
		break;
	}

	//タイムが以上になったら表示
	if (m_skipTime > 60)
	{
		DrawGraph(130, 50, m_handles[kCursorH], true);
	}

	//DrawFormatStringToHandle(300, 100, 0xffffff, m_fontH, "TEST");

}



void SceneWords::Next()
{

}

void SceneWords::CloseWords(int wordsNum)
{
	if (m_wordsNum == wordsNum)
	{
		m_pManager.PopScene();
	}
}


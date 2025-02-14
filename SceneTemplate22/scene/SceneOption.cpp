#include "DxLib.h"

#include "scene/SceneOption.h"
#include "scene/SceneManager.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/Setting.h"
#include "util/Font.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	constexpr int kMaxVolume = 100;

	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";

	//フォントサイズ	
	constexpr int kFontSize = 34;

	//BGMやSEなどのテキストポジション
	constexpr int kTextPosY[4] = { 180, 280, 380, 480 };

	constexpr int kPushWaitCount = 30;

	//グラフの座標
	const Vec2 kBgmPos = { 160.0f , 180.0f };
	const Vec2 kSePos = { 160.0f , 300.0f };
	const Vec2 kSensitivityPos = { 160.0f , 420.0f };
	const Vec2 kFlipSideUpPos = { 160.0f , 540.0f };
	const Vec2 kFullScreenPos = { 160.0f , 660.0f };

	//選択UIのポジション
	const Vec2 kBgmSelectPos = { 110 , 195 };
	const Vec2 kSeSelectPos = { 110 , 315 };
	const Vec2 kSensitivitySelectPos = { 110 , 430 };
	const Vec2 kFlipSideUpSelectPos = { 110 , 550 };
	const Vec2 kFullScreenSelectPos = { 110 , 670 };
	
	const Vec2 kTextBgmPosFront = { 1030 , 170 };
	const Vec2 kTextBgmPosBack = { 1033 , 170 };

	const Vec2 kTextSePosFront = { 1030 , 290 };
	const Vec2 kTextSePosBack = { 1033 , 290 };

	const Vec2 kTextSensitivityPosFront = { 1030 , 410 };
	const Vec2 kTextSensitivityPosBack = { 1033 , 410 };

	const Vec2 kBBackPos = { 1160 , 680 };
	
	const Vec2 kBgmBarPos = { 550 , 170 };
	const Vec2 kSeBarPos = { 550 , 290 };
	const Vec2 kSensitivityBarPos = { 550 , 410 };
	const Vec2 kFlipSideUpMathPos = { 550 , 530 };
	const Vec2 kFullScreenMathPos = { 550 , 650 };
	
	const Vec2 kPointBgmPos = { 550 , 174 };
	const Vec2 kPointSePos = { 550 , 294 };
	const Vec2 kPointSensitivityPos = { 550 , 414 };

	const Vec2 kFlipSideUpCheckPos = { 550 , 530 };
	const Vec2 kFullScreenCheckPos = { 550 , 650 };


	constexpr float kCursorSpeed = 0.05f;

	//オプションの背景アルファ値
	constexpr int kAlpha = 220;

	//使う画像の種類
	enum e_Ui
	{
		kBgmH,
		kSeH,
		kSensitivityH,
		kFlipSideUpH,
		kFullScreenH,
		kBarH,
		kMathH,
		kPointH,
		kCheckH,
		kPointerH,
		kBackH,
		kOptionBarH,
	};

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneOption::SceneOption(SceneManager& manager) :
	SceneBase(manager),
	m_pushCount(0),
	m_bgmScale(50),
	m_seScale(50),
	m_sensitivityScale(50),
	m_isFullScreen(false),
	m_isFlipSideUp(false)
{
	m_isToNextScene = false;

	m_cursorPos = kBgmSelectPos;
	m_targetCursorDownPos = kSeSelectPos;
	m_targetCursorUpPos = kFullScreenSelectPos;

	m_nowItem = e_Item::kBgm;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/Bgm2.png"));
	m_handles.push_back(LoadGraph("Data/Image/Se2.png"));				
	m_handles.push_back(LoadGraph("Data/Image/Sensitivity.png"));		
	m_handles.push_back(LoadGraph("Data/Image/FlipSideUp.png"));
	m_handles.push_back(LoadGraph("Data/Image/FullScreen.png"));		
	m_handles.push_back(LoadGraph("Data/Image/Bar.png"));
	m_handles.push_back(LoadGraph("Data/Image/FullScreenMath.png"));
	m_handles.push_back(LoadGraph("Data/Image/Point.png"));
	m_handles.push_back(LoadGraph("Data/Image/Check.png"));
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));
	m_handles.push_back(LoadGraph("Data/Image/B_Back.png"));
	m_handles.push_back(LoadGraph("Data/Image/OptionBar.png"));

	m_fontHandle = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", kFontSize);


	FadeInSkip();

	m_bgmScale = static_cast<int>(Setting::GetInstance().GetBGMVolume() * 100);
	m_seScale = static_cast<int>(Setting::GetInstance().GetSEVolume() * 100);
	m_sensitivityScale = static_cast<int>(Setting::GetInstance().GetSensitivity() * 100);
	m_isFlipSideUp = !Setting::GetInstance().GetIsFlipSideUp();
	m_isFullScreen = !Setting::GetInstance().GetIsFullScreen();

	// メンバ関数ポインタの初期化
	m_updateFunc = &SceneOption::BgmUpdate;
}

SceneOption::~SceneOption()
{

	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void SceneOption::Update()
{
	(this->*m_updateFunc)();

	Pad::Update();
	UpdateFade();
	UpdateFadeSelectGraph();

	if (m_nowItem == e_Item::kBgm)
	{
		m_cursorPos = kBgmSelectPos;

		m_targetCursorUpPos = kFullScreenSelectPos;
		m_targetCursorDownPos = kSeSelectPos;
	}
	else if (m_nowItem == e_Item::kSe)
	{
		m_cursorPos = kSeSelectPos;

		m_targetCursorUpPos = kBgmSelectPos;
		m_targetCursorDownPos = kSensitivitySelectPos;
	}
	else if (m_nowItem == e_Item::kSensitivity)
	{
		m_cursorPos = kSensitivitySelectPos;

		m_targetCursorUpPos = kSeSelectPos;
		m_targetCursorDownPos = kFlipSideUpSelectPos;
	}
	else if (m_nowItem == e_Item::kFlipSideUp)
	{
		m_cursorPos = kFlipSideUpSelectPos;

		m_targetCursorUpPos = kSensitivitySelectPos;
		m_targetCursorDownPos = kFullScreenSelectPos;
	}
	else if (m_nowItem == e_Item::kFullScreen)
	{
		m_cursorPos = kFullScreenSelectPos;

		m_targetCursorUpPos = kFlipSideUpSelectPos;
		m_targetCursorDownPos = kBgmSelectPos;
	}

	if (!Pad::IsPress(PAD_INPUT_RIGHT) && !Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_pushCount = 0;
	}

	//Bボタンを押したら前のシーンに戻る
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		SoundManager::GetInstance().PlaySe("backSe");
		Setting::GetInstance().SetBGMVolume(static_cast<float>(m_bgmScale) / 100);
		Setting::GetInstance().SetSEVolume(static_cast<float>(m_seScale) / 100);
		Setting::GetInstance().SetSensitivity(static_cast<float>(m_sensitivityScale) / 100);
		Setting::GetInstance().SetIsFlipSideUp(m_isFlipSideUp);
		Setting::GetInstance().SetIsFullScreen(m_isFullScreen);

		Setting::GetInstance().Save();
		m_pManager.PopScene();
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneOption::Draw()
{
	DrawFade(0x000000);

	//背景フェード
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawGraph(0, 0, m_handles[kOptionBarH], true);

	DrawGraph(kBgmBarPos.x, kBgmBarPos.y, m_handles[kBarH], true);
	DrawGraph(kSeBarPos.x, kSeBarPos.y, m_handles[kBarH], true);
	DrawGraph(kSensitivityBarPos.x, kSensitivityBarPos.y, m_handles[kBarH], true);
	DrawGraph(kFlipSideUpCheckPos.x, kFlipSideUpCheckPos.y, m_handles[kMathH], true);
	DrawGraph(kFullScreenMathPos.x, kFullScreenMathPos.y, m_handles[kMathH], true);

	DrawGraph(kPointBgmPos.x + m_bgmScale * 3.68, kPointBgmPos.y, m_handles[kPointH], true);
	DrawGraph(kPointSePos.x + m_seScale * 3.68, kPointSePos.y, m_handles[kPointH], true);
	DrawGraph(kPointSensitivityPos.x + m_sensitivityScale * 3.68, kPointSensitivityPos.y, m_handles[kPointH], true);

	if (m_isFlipSideUp)
	{
		DrawGraph(kFlipSideUpCheckPos.x, kFlipSideUpCheckPos.y, m_handles[kCheckH], true);
	}

	if (m_isFullScreen)
	{
		DrawGraph(kFullScreenCheckPos.x, kFullScreenCheckPos.y, m_handles[kCheckH], true);
	}

	DrawGraph(kBBackPos.x, kBBackPos.y, m_handles[kBackH], true);

	DrawFormatStringToHandle(kTextBgmPosBack.x, kTextBgmPosBack.y, 0xa9a9a9, m_fontHandle, "%d", m_bgmScale);
	DrawFormatStringToHandle(kTextSePosBack.x, kTextSePosBack.y, 0xa9a9a9, m_fontHandle, "%d", m_seScale);
	DrawFormatStringToHandle(kTextSensitivityPosBack.x, kTextSensitivityPosBack.y, 0xa9a9a9, m_fontHandle, "%d", m_sensitivityScale);

	DrawFormatStringToHandle(kTextBgmPosFront.x, kTextBgmPosFront.y, 0xffffff, m_fontHandle, "%d", m_bgmScale);
	DrawFormatStringToHandle(kTextSePosFront.x, kTextSePosFront.y, 0xffffff, m_fontHandle, "%d", m_seScale);
	DrawFormatStringToHandle(kTextSensitivityPosFront.x, kTextSensitivityPosFront.y, 0xffffff, m_fontHandle, "%d", m_sensitivityScale);

	//カーソルの描画
	DrawCursor();

	//選択
	if (m_nowItem == e_Item::kBgm)
	{
		//BGM
		DrawFadeSelectGraph(m_handles[kBgmH], kBgmPos);
		//SE
		DrawGraph(kSePos.x, kSePos.y, m_handles[kSeH], true);
		//Sensitivity
		DrawGraph(kSensitivityPos.x, kSensitivityPos.y, m_handles[kSensitivityH], true);
		//FlipSideUp
		DrawGraph(kFlipSideUpPos.x, kFlipSideUpPos.y, m_handles[kFlipSideUpH], true);
		//FullScreen
		DrawGraph(kFullScreenPos.x, kFullScreenPos.y, m_handles[kFullScreenH], true);
	}
	if (m_nowItem == e_Item::kSe)
	{
		//BGM
		DrawGraph(kBgmPos.x, kBgmPos.y, m_handles[kBgmH], true);
		//SE
		DrawFadeSelectGraph(m_handles[kSeH], kSePos);
		//Sensitivity
		DrawGraph(kSensitivityPos.x, kSensitivityPos.y, m_handles[kSensitivityH], true);
		//FlipSideUp
		DrawGraph(kFlipSideUpPos.x, kFlipSideUpPos.y, m_handles[kFlipSideUpH], true);
		//FullScreen
		DrawGraph(kFullScreenPos.x, kFullScreenPos.y, m_handles[kFullScreenH], true);
	}
	else if (m_nowItem == e_Item::kSensitivity)
	{
		//BGM
		DrawGraph(kBgmPos.x, kBgmPos.y, m_handles[kBgmH], true);
		//SE
		DrawGraph(kSePos.x, kSePos.y, m_handles[kSeH], true);
		//Sensitivity
		DrawFadeSelectGraph(m_handles[kSensitivityH], kSensitivityPos);
		//FlipSideUp
		DrawGraph(kFlipSideUpPos.x, kFlipSideUpPos.y, m_handles[kFlipSideUpH], true);
		//FullScreen
		DrawGraph(kFullScreenPos.x, kFullScreenPos.y, m_handles[kFullScreenH], true);
	}
	else if (m_nowItem == e_Item::kFlipSideUp)
	{
		//BGM
		DrawGraph(kBgmPos.x, kBgmPos.y, m_handles[kBgmH], true);
		//SE
		DrawGraph(kSePos.x, kSePos.y, m_handles[kSeH], true);
		//Sensitivity
		DrawGraph(kSensitivityPos.x, kSensitivityPos.y, m_handles[kSensitivityH], true);
		//FlipSideUp
		DrawFadeSelectGraph(m_handles[kFlipSideUpH], kFlipSideUpPos);
		//FullScreen
		DrawGraph(kFullScreenPos.x, kFullScreenPos.y, m_handles[kFullScreenH], true);
	}
	else if (m_nowItem == e_Item::kFullScreen)
	{
		//BGM
		DrawGraph(kBgmPos.x, kBgmPos.y, m_handles[kBgmH], true);
		//SE
		DrawGraph(kSePos.x, kSePos.y, m_handles[kSeH], true);
		//Sensitivity
		DrawGraph(kSensitivityPos.x, kSensitivityPos.y, m_handles[kSensitivityH], true);
		//FlipSideUp
		DrawGraph(kFlipSideUpPos.x, kFlipSideUpPos.y, m_handles[kFlipSideUpH], true);
		//FullScreen
		DrawFadeSelectGraph(m_handles[kFullScreenH], kFullScreenPos);
	}

#ifdef _DEBUG

	DrawFormatString(0, 0, 0xffffff, "Scene Option");

	DrawFormatString(0, 160, 0xffffff, "Item : %d", m_nowItem);

	DrawFormatString(32, 32, 0xffffff, "BGM");
	DrawFormatString(32, 48, 0xffffff, "SE");
	DrawFormatString(32, 64, 0xffffff, "Sensitivity");
	DrawFormatString(32, 80, 0xffffff, "FullScreen");

	DrawBox(128, 32, 128 + m_bgmScale, 45, 0xff00ff, true);
	DrawBox(128, 48, 128 + m_seScale, 61, 0xff00ff, true);
	DrawBox(128, 64, 128 + m_sensitivityScale, 77, 0xff00ff, true);
	
	DrawBox(128, 32, 128 + kMaxVolume, 45, 0xffffff, false);
	DrawBox(128, 48, 128 + kMaxVolume, 61, 0xffffff, false);
	DrawBox(128, 64, 128 + kMaxVolume, 77, 0xffffff, false);
#endif

}

void SceneOption::BgmUpdate()
{
	
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kFullScreen;
		m_updateFunc = &SceneOption::FullScreenUpdate;
		FadeGraphSelectReset();
		UpdateCursorUp();
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSe;
		m_updateFunc = &SceneOption::SeUpdate;
		FadeGraphSelectReset();
		UpdateCursorDown();
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_bgmScale++;

			if (m_bgmScale > 100)
			{
				m_bgmScale = 100;
			}
			SoundManager::GetInstance().ChangeBGMVolume(static_cast<float>(m_bgmScale) / 100);
		}
		m_pushCount++;
	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_bgmScale--;

			if (m_bgmScale < 0)
			{
				m_bgmScale = 0;
			}
			SoundManager::GetInstance().ChangeBGMVolume(static_cast<float>(m_bgmScale) / 100);
		}
		m_pushCount++;
	}

}

void SceneOption::SeUpdate()
{

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kBgm;
		m_updateFunc = &SceneOption::BgmUpdate;
		FadeGraphSelectReset();
		UpdateCursorUp();
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
		FadeGraphSelectReset();
		UpdateCursorDown();
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_seScale++;

			if (m_seScale > 100)
			{
				m_seScale = 100;
			}
			SoundManager::GetInstance().ChangeSEVolume(static_cast<float>(m_seScale) / 100);
		}
		m_pushCount++;
	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_seScale--;

			if (m_seScale < 0)
			{
				m_seScale = 0;
			}
			SoundManager::GetInstance().ChangeSEVolume(static_cast<float>(m_seScale) / 100);
		}
		m_pushCount++;
	}
}

void SceneOption::SensitivityUpdate()
{

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSe;
		m_updateFunc = &SceneOption::SeUpdate;
		FadeGraphSelectReset();
		UpdateCursorUp();
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kFlipSideUp;
		m_updateFunc = &SceneOption::FlipSideUpUpdate;
		FadeGraphSelectReset();
		UpdateCursorDown();
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_sensitivityScale++;
			if (m_sensitivityScale > 100)
			{
				m_sensitivityScale = 100;
			}
		}
		m_pushCount++;

	}

	if (Pad::IsPress(PAD_INPUT_LEFT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			m_sensitivityScale--;
			if (m_sensitivityScale < 1)
			{
				m_sensitivityScale = 1;
			}
		}
		m_pushCount++;
	}
}

void SceneOption::FlipSideUpUpdate()
{
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
		FadeGraphSelectReset();
		UpdateCursorUp();
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kFullScreen;
		m_updateFunc = &SceneOption::FullScreenUpdate;
		FadeGraphSelectReset();
		UpdateCursorDown();
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		SoundManager::GetInstance().PlaySe("dectionSe");
		m_isFlipSideUp = !m_isFlipSideUp;
	}
}

void SceneOption::FullScreenUpdate()
{

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kFlipSideUp;
		m_updateFunc = &SceneOption::FlipSideUpUpdate;
		FadeGraphSelectReset();
		UpdateCursorUp();
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kBgm;
		m_updateFunc = &SceneOption::BgmUpdate;
		FadeGraphSelectReset();
		UpdateCursorDown();
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		SoundManager::GetInstance().PlaySe("dectionSe");
		ChangeWindowMode(m_isFullScreen);
		m_isFullScreen = !m_isFullScreen;
	}
}

void SceneOption::DrawCursor()
{
	DrawGraph(m_cursorPos.x + m_selectAnimation, m_cursorPos.y, m_handles[kPointerH], true);
}

void SceneOption::UpdateCursorUp()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorUpPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorUpPos.y - m_cursorPos.y) * kCursorSpeed;
}

void SceneOption::UpdateCursorDown()
{
	// 線形補間でカーソルの位置を更新
	m_cursorPos.x += (m_targetCursorDownPos.x - m_cursorPos.x) * kCursorSpeed;
	m_cursorPos.y += (m_targetCursorDownPos.y - m_cursorPos.y) * kCursorSpeed;
}

#include "DxLib.h"

#include "SceneOption.h"
#include "SceneManager.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/Setting.h"


namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	constexpr int kMaxVolume = 100;

	//BGMやSEなどのテキストポジション
	constexpr int kTextPosY[4] = { 180, 280, 380, 480 };

	constexpr int kPushWaitCount = 30;

	//オプションの背景アルファ値
	constexpr int kAlpha = 200;

	//使う画像の種類
	enum e_Ui
	{
		kBgmH,
		kSeH,
		kSensitivityH,
		kFullScreenH,
		kBarH,
		kMathH,
		kPointH,
		kCheckH,
		kSelectH,
		kBackH,
	};
	
}

SceneOption::SceneOption(SceneManager& manager) :
	SceneBase(manager),
	m_pushCount(0),
	m_bgmScale(50),
	m_seScale(50),
	m_sensitivityScale(50),
	m_isFullScreen(false)
{
	m_nowItem = e_Item::kBgm;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/Bgm2.png"));
	m_handles.push_back(LoadGraph("Data/Image/Se2.png"));				
	m_handles.push_back(LoadGraph("Data/Image/Sensitivity2.png"));		
	m_handles.push_back(LoadGraph("Data/Image/FullScreen2.png"));		
	m_handles.push_back(LoadGraph("Data/Image/Bar.png"));
	m_handles.push_back(LoadGraph("Data/Image/Math.png"));
	m_handles.push_back(LoadGraph("Data/Image/Point.png"));
	m_handles.push_back(LoadGraph("Data/Image/Check.png"));
	m_handles.push_back(LoadGraph("Data/Image/Select.png"));
	m_handles.push_back(LoadGraph("Data/Image/Back.png"));

	FadeInSkip();

	m_bgmScale = static_cast<int>(Setting::GetInstance().GetBGMVolume() * 100);
	m_seScale = static_cast<int>(Setting::GetInstance().GetSEVolume() * 100);
	m_sensitivityScale = static_cast<int>(Setting::GetInstance().GetSensitivity() * 100);
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

	if (!Pad::IsPress(PAD_INPUT_RIGHT) && !Pad::IsPress(PAD_INPUT_LEFT))
	{
		m_pushCount = 0;
	}

	//Bボタンを押したら前のシーンに戻る
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		Setting::GetInstance().SetBGMVolume(static_cast<float>(m_bgmScale) / 100);
		Setting::GetInstance().SetSEVolume(static_cast<float>(m_seScale) / 100);
		Setting::GetInstance().SetSensitivity(static_cast<float>(m_sensitivityScale) / 100);
		Setting::GetInstance().SetIsFullScreen(m_isFullScreen);

		Setting::GetInstance().Save();
		m_pManager.PopScene();
	}

}

void SceneOption::Draw()
{
	DrawFade(0x000000);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kAlpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//BGM
	DrawGraph(150, 60, m_handles[kBgmH], true);
	//SE
	DrawGraph(150, 180, m_handles[kSeH], true);
	//Sensitivity
	DrawGraph(150, 300, m_handles[kSensitivityH], true);
	//FullScreen
	DrawGraph(150, 420, m_handles[kFullScreenH], true);

	DrawGraph(450, 60, m_handles[kBarH], true);
	DrawGraph(450, 180, m_handles[kBarH], true);
	DrawGraph(450, 300, m_handles[kBarH], true);
	DrawGraph(450, 420, m_handles[kMathH], true);

	DrawGraph(450 + m_bgmScale * 3.68, 64, m_handles[kPointH], true);
	DrawGraph(450 + m_seScale * 3.68, 184, m_handles[kPointH], true);
	DrawGraph(450 + m_sensitivityScale * 3.68, 304, m_handles[kPointH], true);

	if (m_isFullScreen)
	{
		DrawGraph(450, 424, m_handles[kCheckH], true);
	}

	DrawGraph(0, 660, m_handles[kBackH], true);

	DrawFormatString(900, 75, 0xffffff, "%d", m_bgmScale);
	DrawFormatString(900, 195, 0xffffff, "%d", m_seScale);
	DrawFormatString(900, 315, 0xffffff, "%d", m_sensitivityScale);

	//選択
	if (m_nowItem == e_Item::kBgm)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 430, m_handles[kSelect], true);
		DrawGraph(100, 70, m_handles[kSelectH], true);
	}
	if (m_nowItem == e_Item::kSe)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 490, m_handles[kSelect], true);
		DrawGraph(100, 190, m_handles[kSelectH], true);
	}
	else if (m_nowItem == e_Item::kSensitivity)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 550, m_handles[kSelect], true);
		DrawGraph(100, 310, m_handles[kSelectH], true);
	}
	else if (m_nowItem == e_Item::kFullScreen)
	{
		//DrawGraph(Game::kScreenWidthHalf - 150, 610, m_handles[kSelect], true);
		DrawGraph(100, 430, m_handles[kSelectH], true);
	}

#ifdef _DEBUG

	DrawFormatString(0, 0, 0xffffff, "Scene Option");

	DrawFormatString(0, 16, 0xffffff, "Item : %d", m_nowItem);

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
	
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSe;
		m_updateFunc = &SceneOption::SeUpdate;
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
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
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
	}

	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kFullScreen;
		m_updateFunc = &SceneOption::FullScreenUpdate;
	}

	if (Pad::IsPress(PAD_INPUT_RIGHT))
	{
		if (m_pushCount == 0 || m_pushCount > kPushWaitCount)
		{
			SoundManager::GetInstance().PlaySe("selectSe");
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
			SoundManager::GetInstance().PlaySe("selectSe");
			m_sensitivityScale--;
			if (m_sensitivityScale < 1)
			{
				m_sensitivityScale = 1;
			}
		}
		m_pushCount++;
	}
}

void SceneOption::FullScreenUpdate()
{

	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		SoundManager::GetInstance().PlaySe("selectSe");
		m_nowItem = e_Item::kSensitivity;
		m_updateFunc = &SceneOption::SensitivityUpdate;
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		SoundManager::GetInstance().PlaySe("dectionSe");
		ChangeWindowMode(m_isFullScreen);
		m_isFullScreen = !m_isFullScreen;
	}
}

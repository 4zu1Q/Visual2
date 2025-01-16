#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGameOver.h"
#include "SceneSelect.h"
#include "SceneGamePlay.h"
#include "SceneTitle.h"
#include "SceneDebug.h"

#include "object/player/PlayerProduction.h"
#include "object/CameraProduction.h"

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
		kGameOverH,
		kSelectH,
		kTitleH,
		kPointerH,
	};

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;
}

SceneGameOver::SceneGameOver(SceneManager& manager) :
	SceneBase(manager),
	m_fadeTime(0)
{
	m_isActionStart = false;
	m_isActionBack = false;

	m_sceneTrans = e_SceneTrans::kGamePlay;

	m_pPlayerProduction = std::make_shared<PlayerProduction>();
	m_pCameraProduction = std::make_shared<CameraProduction>();

	m_pPlayerProduction->Initialize(Game::e_PlayerProduction::kGameOver);
	m_pCameraProduction->Initialize(m_pPlayerProduction->GetPos(), Game::e_PlayerProduction::kGameOver);

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/GameOver.png"));
	m_handles.push_back(LoadGraph("Data/Image/Select.png"));				//Select
	m_handles.push_back(LoadGraph("Data/Image/Retry.png"));					//Title
	m_handles.push_back(LoadGraph("Data/Image/Pointer.png"));				//矢印
}

SceneGameOver::~SceneGameOver()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();
}

void SceneGameOver::Update()
{
	Pad::Update();
	UpdateFade();

	m_pCameraProduction->Update();
	m_pPlayerProduction->Update(m_isActionStart, m_isActionBack);

#ifdef _DEBUG
	//デバッグに遷移する
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}
#endif

	//
	if (!m_isToNextScene)
	{
		//左を押した場合
		if (Pad::IsTrigger(PAD_INPUT_LEFT))
		{
			if (m_sceneTrans != e_SceneTrans::kGamePlay)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
				SoundManager::GetInstance().PlaySe("selectSe");
			}
		}

		//右を押した場合
		if (Pad::IsTrigger(PAD_INPUT_RIGHT))
		{
			if (m_sceneTrans != e_SceneTrans::kSelect)
			{
				m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
				SoundManager::GetInstance().PlaySe("selectSe");
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

	//if (m_isToNextScene)
	//{
	//	m_fadeTime++;

	//	if (m_fadeTime > 60)
	//	{
	//	}
	//}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		//if (m_fadeTime > 60)
		{
			if (!IsFadingOut())
			{
				if (m_sceneTrans == e_SceneTrans::kGamePlay)
				{
					m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, Game::e_BossKind::kPower, Game::e_StageKind::kGamePlay));
					return;
				}

				if (m_sceneTrans == e_SceneTrans::kSelect)
				{
					m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
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
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);

	m_pCameraProduction->Draw();
	m_pPlayerProduction->Draw();

#ifdef _DEBUG

#endif

	//選択
	if (m_sceneTrans == e_SceneTrans::kGamePlay)
	{
		DrawGraph(360 + m_selectAnimation, 615, m_handles[kPointerH], true);
	}
	if (m_sceneTrans == e_SceneTrans::kSelect)
	{
		DrawGraph(760 + m_selectAnimation, 615, m_handles[kPointerH], true);
	}

	DrawGraph(380, 100, m_handles[kGameOverH], true);

	//Select
	DrawGraph(800, 600, m_handles[kSelectH], true);
	//GamePlay
	DrawGraph(400, 600, m_handles[kTitleH], true);

	DrawString(0, 0, "Scene Game Over", 0xffffff, false);

	DrawFormatString(kTextX / 2, kTextBlankSpaceY + static_cast<int>(m_sceneTrans) * kTextIntervalY, 0xff0000, "→");

	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kGamePlay) * kTextIntervalY, 0xffffff, "Retry");
	DrawFormatString(kTextX, kTextBlankSpaceY + static_cast<int>(e_SceneTrans::kSelect) * kTextIntervalY, 0xffffff, "Select");

	DrawFade(0x000000);
}

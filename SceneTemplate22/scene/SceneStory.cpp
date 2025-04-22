#include "scene/SceneStory.h"

#include "scene/SceneManager.h"
#include "scene/SceneSelect.h"
#include "scene/SceneTutorial.h"

#include "util/Vec2.h"
#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"

namespace
{

	//ポーズの背景アルファ値
	constexpr int kAlpha = 200;
	
	constexpr int kTimeRimit = 120;

	constexpr float kCursorSpeed = 0.05f;

	//使う画像の種類
	enum e_Ui
	{
		kScene1H,
		kScene2H,
		kButtonH,
	};

	constexpr float kSelectSpeed = 0.06f;
	constexpr float kSelectAnimationSize = 4.0f;

	const Vec2 kPos = { 0 , 0 };
	const Vec2 kButtonPos = { 950 , 660 };


}

SceneStory::SceneStory(SceneManager& manager) :
	SceneBase(manager)
{
	m_isNext = false;
	m_isSkip = false;

	m_isToNextScene = false;

	m_storyTime = 0;
	m_pressTime = 0;

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/Scene1.png"));
	m_handles.push_back(LoadGraph("Data/Image/Scene2.png"));
	m_handles.push_back(LoadGraph("Data/Image/StoryButton.png"));

	m_sceneTrans = e_Scene::kScene1;

	// メンバ関数ポインタの初期化
	m_updateFunc = &SceneStory::Update01;
}

SceneStory::~SceneStory()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	m_handles.clear();
}

void SceneStory::Update()
{
	//アップデート
	(this->*m_updateFunc)();

	//パッドを更新
	Pad::Update();
	UpdateFade();
	UpdateFadeGraphTitleLogo();

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void SceneStory::Draw()
{

	switch (m_sceneTrans)
	{
	case 0:
		//最初の話
		DrawFadeGraphTitleLogo(m_handles[kScene1H], kPos);
		DrawFadeGraphTitleLogo(m_handles[kButtonH], kButtonPos);
		DrawString(0, 16, "1", 0xffffff, false);


		break;
	case 1:
		DrawGraph(kPos.x, kPos.y, m_handles[kScene1H], true);
		DrawFadeGraphTitleLogo(m_handles[kScene2H], kPos);
		DrawFadeGraphTitleLogo(m_handles[kButtonH], kButtonPos);
		DrawString(0, 16, "2", 0xffffff, false);


		break;
	default:
		break;
	}

	DrawString(0, 0, "Scene Story", 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Time:%d", m_storyTime);
	DrawFormatString(0, 48, 0xffffff, "Skip:%d", m_isSkip);
	DrawFormatString(0, 64, 0xffffff, "Next:%d", m_isNext);
	DrawFormatString(0, 80, 0xffffff, "pressTime:%d", m_pressTime);

	DrawFade(0x000000);
}

void SceneStory::Update01()
{
	Next();
}

void SceneStory::Update02()
{
	Next();

	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			//m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerPos));
			m_pManager.ChangeScene(std::make_shared<SceneTutorial>(m_pManager, Game::e_StageKind::kTutorial));
			return;
		}
	}
}

void SceneStory::OnUpdate02()
{
	FadeGraphTitleLogoReset();
	m_isNext = false;
	m_isSkip = false;
	m_storyTime = 0;

	m_sceneTrans = e_Scene::kScene2;
	m_updateFunc = &SceneStory::Update02;
}

void SceneStory::Next()
{
	m_storyTime++;

	if (m_storyTime > kTimeRimit)
	{
		m_isNext = true;
	}

	if (Pad::IsTrigger(PAD_INPUT_1) && m_isNext)
	{
		switch (m_sceneTrans)
		{

		case 0:
			//最初の話
			OnUpdate02();

			break;
		case 1:
			StartFadeOut();
			m_isToNextScene = true;

			break;
		default:
			break;
		}
	}

	if (Pad::IsPress(PAD_INPUT_2))
	{
		m_pressTime++;
		if (m_pressTime > 120)
		{
			m_isSkip = true;
		}
	}

	if (m_isSkip)
	{
		//m_isNext = true;
		//FadeGraphTitleLogoDraw();

		StartFadeOut();
		m_isToNextScene = true;
	}
}




#include "SceneTitle.h"
#include "DxLib.h"
#include "Pad.h"

#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

SceneTitle::SceneTitle():
	m_frameCount(0)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

#ifdef _DEBUG

	//è„ï˚å¸ÇâüÇµÇΩÇ∆Ç´
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{
		if (m_select == kSceneSelect)
		{
			m_select = kSceneLose;
		}
		else if (m_select == kScenePlaying)
		{
			m_select = kSceneSelect;
		}
		else if (m_select == kSceneWin)
		{
			m_select = kScenePlaying;
		}
		else if (m_select == kSceneLose)
		{
			m_select = kSceneWin;
		}
	}

	//â∫ï˚å¸ÇâüÇµÇΩÇ∆Ç´
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{
		if (m_select == kSceneSelect)
		{
			m_select = kScenePlaying;
		}
		else if (m_select == kScenePlaying)
		{
			m_select = kSceneWin;
		}
		else if (m_select == kSceneWin)
		{
			m_select = kSceneLose;
		}
		else if (m_select == kSceneLose)
		{
			m_select = kSceneSelect;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		if (m_select == kSceneSelect)
		{
		}
		else if (m_select == kScenePlaying)
		{
			return std::make_shared<ScenePlaying>();
		}
		else if (m_select == kSceneWin)
		{
			return std::make_shared<SceneLose>();
		}
		else if (m_select == kSceneLose)
		{
			return std::make_shared<SceneLose>();
		}

	}

#endif


	return shared_from_this();
}

void SceneTitle::Draw()
{


	if (m_select == kSceneSelect)
	{
		DrawString(0, 16, "Å®", 0xff00ff, false);
	}
	else if (m_select == kScenePlaying)
	{
		DrawString(0, 32, "Å®", 0xff00ff, false);
	}
	else if (m_select == kSceneWin)
	{
		DrawString(0, 48, "Å®", 0xff00ff, false);
	}
	else if (m_select == kSceneLose)
	{
		DrawString(0, 64, "Å®", 0xff00ff, false);
	}

	DrawString(16, 16, "Scene Select", 0x0000ff, false);
	DrawString(16, 32, "Scene Playing", 0x0000ff, false);
	DrawString(16, 48, "Scene Win", 0x0000ff, false);
	DrawString(16, 64, "Scene Lose", 0x0000ff, false);


	DrawString(0, 0, "Scene Title", 0xffffff, false);
}

void SceneTitle::End()
{
}

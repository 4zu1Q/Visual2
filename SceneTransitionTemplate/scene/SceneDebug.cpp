#include "SceneDebug.h"

#include "SceneDebug.h"
#include "SceneTitle.h"
#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

#include "DxLib.h"
#include "Pad.h"

SceneDebug::SceneDebug() :
	m_select(kSceneDebug)
{
}

SceneDebug::~SceneDebug()
{
}

void SceneDebug::Init()
{
}

std::shared_ptr<SceneBase> SceneDebug::Update()
{
	Pad::Update();
	bool isCommand = false;


#ifdef _DEBUG

	//åàíËÇµÇΩÇÁìÆÇ©ÇπÇ»Ç≠Ç≥ÇπÇÈ
	if (isCommand) return shared_from_this();

	//è„ï˚å¸ÇâüÇµÇΩÇ∆Ç´
	if (Pad::IsTrigger(PAD_INPUT_UP))
	{


		if (m_select == kSceneDebug)
		{
			m_select = kSceneLose;
		}
		else if (m_select == kSceneTitle)
		{
			m_select = kSceneDebug;
		}
		else if (m_select == kScenePlaying)
		{
			m_select = kSceneTitle;
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
		if (m_select == kSceneDebug)
		{
			m_select = kSceneTitle;
		}
		else if (m_select == kSceneTitle)
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
			m_select = kSceneDebug;
		}
	}

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		if (m_select == kSceneDebug)
		{
			isCommand = true;
			return std::make_shared<SceneDebug>();
		}
		else if (m_select == kSceneTitle)
		{
			isCommand = true;
			return std::make_shared<SceneTitle>();
		}
		else if (m_select == kScenePlaying)
		{
			isCommand = true;
			return std::make_shared<ScenePlaying>();
		}
		else if (m_select == kSceneWin)
		{
			isCommand = true;
			return std::make_shared<SceneLose>();
		}
		else if (m_select == kSceneLose)
		{
			isCommand = true;
			return std::make_shared<SceneLose>();
		}

	}

#endif

	return shared_from_this();
}

void SceneDebug::Draw()
{
	if (m_select == kSceneDebug)
	{
		DrawString(0, 16, "Å®", 0xff00ff, false);
	}
	else if (m_select == kSceneTitle)
	{
		DrawString(0, 32, "Å®", 0xff00ff, false);
	}
	else if (m_select == kScenePlaying)
	{
		DrawString(0, 48, "Å®", 0xff00ff, false);
	}
	else if (m_select == kSceneWin)
	{
		DrawString(0, 64, "Å®", 0xff00ff, false);
	}
	else if (m_select == kSceneLose)
	{
		DrawString(0, 80, "Å®", 0xff00ff, false);
	}


	DrawString(16, 16, "Scene Debug", 0xff0000, false);
	DrawString(16, 32, "Scene Title", 0xff0000, false);
	DrawString(16, 48, "Scene Playing", 0xff0000, false);
	DrawString(16, 64, "Scene Win", 0xff0000, false);
	DrawString(16, 80, "Scene Lose", 0xff0000, false);

	DrawString(0, 0, "Scene Debug", 0xffffff, false);

	DrawFormatString(110, 0, 0xffffff, "m_select:%d", m_select);
}

void SceneDebug::End()
{
}
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
	bool isCommand = false;

#ifdef _DEBUG

	//���肵���瓮�����Ȃ�������
	if (isCommand) return shared_from_this();

	//��������������Ƃ�
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

	//���������������Ƃ�
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
			isCommand = true;
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

void SceneTitle::Draw()
{


	if (m_select == kSceneSelect)
	{
		DrawString(0, 16, "��", 0xff00ff, false);
	}
	else if (m_select == kScenePlaying)
	{
		DrawString(0, 32, "��", 0xff00ff, false);
	}
	else if (m_select == kSceneWin)
	{
		DrawString(0, 48, "��", 0xff00ff, false);
	}
	else if (m_select == kSceneLose)
	{
		DrawString(0, 64, "��", 0xff00ff, false);
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

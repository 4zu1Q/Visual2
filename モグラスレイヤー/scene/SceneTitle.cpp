#include "SceneTitle.h"
#include "DxLib.h"
#include "Pad.h"

#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

SceneTitle::SceneTitle():
	m_frameCount(0),
	m_isInterval(false)
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

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isInterval = true;
	}

	if (m_isInterval)
	{
		m_frameCount++;

		UpdateFade();

		if (120 < m_frameCount)
		{
			return std::make_shared<ScenePlaying>();
		}
	}


	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawFade();

	DrawString(0, 0, "Scene Title", 0xffffff, false);
}

void SceneTitle::End()
{
}

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
	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawString(0, 0, "Scene Title", 0xffffff, false);
}

void SceneTitle::End()
{
}

#include "SceneTitle.h"
#include "DxLib.h"

#include "ScenePlaying.h"

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
	m_frameCount++;

	if (m_frameCount > 120)
	{
		//ScenePlayingに切り替え
		//ScenePlayingのメモリを確保してそのポインタを返す
		return std::make_shared<ScenePlaying>();

	}

	return shared_from_this();
}

void SceneTitle::Draw()
{
	DrawBox(100, 100, 200, 200, 0xffffff, false);
	DrawCircle(400, 200, 128, 0xfff000, true);

	DrawFormatString(0, 16, 0xffffff, "Frame:%d", m_frameCount);
	DrawString(0, 0, "Scene Title", 0xffffff, false);
}

void SceneTitle::End()
{
}

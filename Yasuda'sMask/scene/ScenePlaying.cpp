#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "DxLib.h"

#include "Pad.h"

ScenePlaying::ScenePlaying()
{
}

ScenePlaying::~ScenePlaying()
{
}

void ScenePlaying::Init()
{
}

std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		//SceneTitleのメモリを確保してそのポインタを返す
		return std::make_shared<SceneTitle>();
	}

	return shared_from_this();
}

void ScenePlaying::Draw()
{
	DrawString(0, 0, "Scene Playing", 0xffffff, false);
}

void ScenePlaying::End()
{
}

#include "SceneLose.h"
#include "DxLib.h"

SceneLose::SceneLose()
{
}

SceneLose::~SceneLose()
{
}

void SceneLose::Init()
{
}

std::shared_ptr<SceneBase> SceneLose::Update()
{
	return shared_from_this();
}

void SceneLose::Draw()
{
	DrawString(0, 0, "Scene Win", 0xffffff, false);
}

void SceneLose::End()
{
}
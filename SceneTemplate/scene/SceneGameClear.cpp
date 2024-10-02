#include "SceneGameClear.h"
#include "DxLib.h"

SceneGameClear::SceneGameClear()
{
}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Init()
{
}

void SceneGameClear::End()
{
}

std::shared_ptr<SceneBase> SceneGameClear::Update()
{
	return std::shared_ptr<SceneBase>();
}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "Scene Game Clear", 0xffffff, false);
}

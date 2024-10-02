#include "DxLib.h"
#include "SceneSelect.h"

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

void SceneSelect::End()
{
}

std::shared_ptr<SceneBase> SceneSelect::Update()
{
	return std::shared_ptr<SceneBase>();
}

void SceneSelect::Draw()
{
	DrawString(0, 0, "Scene Select", 0xffffff, false);
}

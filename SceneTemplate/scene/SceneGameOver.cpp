#include "SceneGameOver.h"
#include "DxLib.h"

SceneGameOver::SceneGameOver()
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
}

void SceneGameOver::End()
{
}

std::shared_ptr<SceneBase> SceneGameOver::Update()
{
	return std::shared_ptr<SceneBase>();
}

void SceneGameOver::Draw()
{
	DrawString(0, 0, "Scene Game Over", 0xffffff, false);
}

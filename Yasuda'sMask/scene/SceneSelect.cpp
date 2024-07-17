#include "SceneSelect.h"
#include "DxLib.h"

SceneSelect::SceneSelect()
{
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Init()
{
}

std::shared_ptr<SceneBase> SceneSelect::Update()
{

	return shared_from_this();
}

void SceneSelect::Draw()
{
	DrawString(0, 0, "Scene Select", 0xffffff, false);
}

void SceneSelect::End()
{
}

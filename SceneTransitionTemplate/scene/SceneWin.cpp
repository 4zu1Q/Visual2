#include "SceneWin.h"
#include "DxLib.h"

SceneWin::SceneWin()
{
}

SceneWin::~SceneWin()
{
}

void SceneWin::Init()
{
}

std::shared_ptr<SceneBase> SceneWin::Update()
{
	return shared_from_this();
}

void SceneWin::Draw()
{
	DrawString(0, 0, "Scene Win", 0xffffff, false);
}

void SceneWin::End()
{
}
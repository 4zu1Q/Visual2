#include "DxLib.h"

#include"SceneManager.h"
#include "SceneGameClear.h"

SceneGameClear::SceneGameClear(SceneManager& manager) :
	SceneBase(manager)
{

}

SceneGameClear::~SceneGameClear()
{
}

void SceneGameClear::Update()
{

}

void SceneGameClear::Draw()
{
	DrawString(0, 0, "Scene Game Clear", 0xffffff, false);
}

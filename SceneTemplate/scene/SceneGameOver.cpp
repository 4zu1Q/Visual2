#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGameOver.h"


SceneGameOver::SceneGameOver(SceneManager& manager) :
	SceneBase(manager)
{

}

SceneGameOver::~SceneGameOver()
{

}

void SceneGameOver::Update()
{
}

void SceneGameOver::Draw()
{
	DrawString(0, 0, "Scene Game Over", 0xffffff, false);
}

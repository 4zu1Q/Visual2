#include "SceneGamePlay.h"
#include "DxLib.h"

SceneGamePlay::SceneGamePlay()
{
}

SceneGamePlay::~SceneGamePlay()
{
}

void SceneGamePlay::Init()
{
}

void SceneGamePlay::End()
{
}

std::shared_ptr<SceneBase> SceneGamePlay::Update()
{
	return std::shared_ptr<SceneBase>();
}

void SceneGamePlay::Draw()
{
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

}

void DrawGrid()
{
	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-‚Ì•ûŒü‚ª•ª‚©‚è‚â‚·‚¢‚æ‚¤‚É•\Ž¦‚ð’Ç‰Á‚·‚é
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
}
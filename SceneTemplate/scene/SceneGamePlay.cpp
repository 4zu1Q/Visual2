#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "ScenePause.h"

#include "util/Pad.h"

namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;
}

SceneGamePlay::SceneGamePlay(SceneManager& manager) :
	SceneBase(manager),
	m_cameraAngle(0.0f)
{
}

SceneGamePlay::~SceneGamePlay()
{

}


void SceneGamePlay::Update()
{
	UpdateFade();
	Pad::Update();

#ifdef _DEBUG

	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
	{
		m_cameraAngle += 0.05f;
	}
	if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT))
	{
		m_cameraAngle -= 0.05f;
	}

	SetCameraNearFar(1.0f, 180.0f);
	VECTOR cameraPos;
	cameraPos.x = cosf(m_cameraAngle) * kCameraDist;
	cameraPos.y = kCameraHeight;
	cameraPos.z = sinf(m_cameraAngle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));

#endif 
	
	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}
}

void SceneGamePlay::Draw()
{
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

	DrawGrid();
	DrawFade();
}

void SceneGamePlay::DrawGrid()
{
	for (int x = -350; x <= 350; x += 5)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -350), VGet(static_cast<float>(x), 0, 350), 0xffff00);
	}
	for (int z = -350; z <= 350; z += 5)
	{
		DrawLine3D(VGet(-350, 0, static_cast<float>(z)), VGet(350, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
}
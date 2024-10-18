#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "ScenePause.h"

#include "object/player/PlayerBase.h"
#include "object/SkyDome.h"

#include "ui/HpBar.h"
#include "ui/FaceUi.h"

#include "util/Pad.h"

namespace
{
	constexpr float kCameraDist = 56;
	constexpr float kCameraHeight = 32;

	constexpr float kMaxSpeedN = 0.9f;

	constexpr float kAnalogRangeMin = 0.1f;
	constexpr float kAnalogRangeMax = 0.8f;

	constexpr float kAnalogInputMax = 1000.0f;
}

SceneGamePlay::SceneGamePlay(SceneManager& manager) :
	SceneBase(manager),
#ifdef _DEBUG
	m_analogX(0),
	m_analogZ(0),
	m_radius(5),
	m_angle(0.0f),
#endif 
	m_isFadingOut(false),
	m_cameraAngle(0.0f)
{
	m_pPlayer = std::make_shared<PlayerBase>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pHpBar = std::make_shared<HpBar>();
	m_pFaceUi = std::make_shared<FaceUi>();

	m_playerPos = VGet(0, 0, 0);
	m_cameraPos = VGet(0, 0, 0);

	m_pPlayer->Initialize();
}

SceneGamePlay::~SceneGamePlay()
{
	
}


void SceneGamePlay::Update()
{
	if (m_isFadingOut)
	{
		if (IsFadingOut())
		{
			SceneBase::StartFadeIn();
			m_isFadingOut = false;
		}
	}

	UpdateFade();
	Pad::Update();

	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	m_pSkyDome->Update();
	m_pPlayer->Update();
	m_pFaceUi->Update();
	m_pHpBar->Update(*m_pPlayer);

#ifdef _DEBUG

	//アナログスティックを使って移動
	int analogX = 0;
	int analogY = 0;

	GetJoypadAnalogInputRight(&analogX, &analogY, DX_INPUT_PAD1);

	if (analogX >= 10)
	{
		m_cameraAngle -= 0.05f;

	}

	if (analogX <= -10)
	{
		m_cameraAngle += 0.05f;

	}

	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
	//{
	//	m_cameraAngle += 0.05f;
	//}
	//if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
	//{
	//	m_cameraAngle -= 0.05f;
	//}

	SetCameraNearFar(0.1f, 1180.0f);
	VECTOR cameraPos;
	cameraPos.x = cosf(m_cameraAngle) * kCameraDist;
	cameraPos.y = 20;
	cameraPos.z = sinf(m_cameraAngle) * kCameraDist;

	SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));

#endif 
	

}

void SceneGamePlay::Draw()
{
	DrawGrid();
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

	m_pSkyDome->Draw();
	m_pPlayer->Draw();
	m_pFaceUi->Draw(*m_pPlayer);
	m_pHpBar->Draw();

	DrawFade();
	
}

void SceneGamePlay::StartFadeOut()
{
	SceneBase::StartFadeOut();
	m_isFadingOut = true;
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
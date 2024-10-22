#include "DxLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "ScenePause.h"

#include "object/player/PlayerBase.h"
#include "object/SkyDome.h"
#include "object/Camera.h"

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
	m_pCamera = std::make_shared<Camera>();
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

	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	m_pCamera->Update(*m_pPlayer);
	m_pSkyDome->Update();
	m_pPlayer->Update();
	m_pFaceUi->Update();
	m_pHpBar->Update(*m_pPlayer);	

}

void SceneGamePlay::Draw()
{
	DrawGrid();
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

	m_pCamera->Draw();
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

}
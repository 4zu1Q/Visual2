#include "DxLib.h"

#include "myLib/MyLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "SceneGameOver.h"
#include "ScenePause.h"
#include "SceneDebug.h"

#include "object/player/Player.h"
#include "object/boss/BossPower.h"
#include "object/boss/BossSpeed.h"
#include "object/stage/SkyDome.h"
#include "object/Camera.h"

#include "ui/HpBar.h"
#include "ui/PlayerBarUi.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"

//#include "ui/GamePlayUi.h"

#include "util/Pad.h"

namespace
{
	//初期位置
	constexpr VECTOR kInitPos = { 20.0f,5.0f,-100.0f };
}

SceneGamePlay::SceneGamePlay(SceneManager& manager) :
	SceneBase(manager),
	m_isMpHit(false),
	m_isHpHit(false),
#ifdef _DEBUG
	m_analogX(0),
	m_analogZ(0),
	m_radius(5),
	m_angle(0.0f),
#endif 
	m_gameOverTime(0),
	m_cameraAngle(0.0f)
{
	m_pPlayer = std::make_shared<Player>();

	m_pHpBarUi = std::make_shared<HpBar>();
	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();

	m_pBossPower = std::make_shared<BossPower>();
	m_pBossSpeed = std::make_shared<BossSpeed>();

	m_pSkyDome = std::make_shared<SkyDome>();
	m_pField = std::make_shared<Field>();

	m_pPhysics = std::make_shared<MyLib::Physics>();

	m_pCamera = std::make_shared<Camera>();

	m_playerPos = VGet(30, 0, 20);
	m_cameraPos = VGet(0, 0, 0);

	//初期位置をセット
	m_pPlayer->Initialize(m_pPhysics, kInitPos);
	m_pBossPower->Initialize(m_pPhysics);
	m_pBossSpeed->Initialize(m_pPhysics);
	m_pCamera->Initialize();

	m_pField->Initialize();
}

SceneGamePlay::~SceneGamePlay()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pBossPower->Finalize(m_pPhysics);
	m_pBossSpeed->Finalize(m_pPhysics);
}


void SceneGamePlay::Update()
{
	Pad::Update();
	UpdateFade();

#ifdef _DEBUG
	MyLib::DebugDraw::Draw();
#endif

#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

#ifdef _DEBUG
	//デバッグに遷移する
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}
#endif

	if (m_pPlayer->GetIsGameOver())
	{
		m_gameOverTime++;
	}

	if (m_gameOverTime > 240)
	{
		m_isToNextScene = true;
		StartFadeOut();
	}


	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	m_pSkyDome->Update();
	m_pBossPower->Update(m_pPhysics, *m_pPlayer);
	m_pBossSpeed->Update(m_pPhysics, *m_pPlayer);

	m_pCamera->Update(m_pField->GetModelHandle(), m_pPlayer->GetPos());
	m_pCamera->SetPlayerPos(m_pPlayer->GetPosUp());

	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());
	m_pPlayer->Update(m_pPhysics);

	m_pPhysics->Update();

	m_pFaceUi->Update();
	////m_pHpBarUi->Update(*m_pBossPower);
	m_pPlayerBarUi->Update(*m_pPlayer);


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		//なんか入らん後でやる
		if (!IsFadingOut())
		{
			m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager));
			return;
		}
	}

}

void SceneGamePlay::Draw()
{

	m_pCamera->Draw();

	m_pField->Draw();
	m_pSkyDome->Draw();

	m_pBossPower->Draw();
	m_pBossSpeed->Draw();
	m_pPlayer->Draw();

	m_pHpBarUi->Draw();
	m_pPlayerBarUi->Draw();
	m_pButtonUi->Draw(*m_pPlayer);
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);
	//m_pGamePlayUi->Draw(*m_pPlayer);

	DrawFade(0x000000);

#ifdef _DEBUG
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

#endif // DEBUG


}

//void SceneGamePlay::StartFadeOut()
//{
//	SceneBase::StartFadeOut();
//	m_isFadingOut = true;
//}

#include "DxLib.h"

#include "SceneManager.h"
#include "SceneSelect.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneGamePlay.h"
#include "SceneDebug.h"

#include "object/player/Player.h"
#include "object/player/PlayerWeapon.h"
#include "object/Camera.h"
#include "object/boss/BossShot.h"

#include "object/item/ItemHp.h"
#include "object/item/ItemMp.h"

#include "object/stage/Field.h"
#include "object/stage/SkyDome.h"
#include "object/stage/Tomb.h"


#include "ui/HpBar.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"
#include "ui/PlayerBarUi.h"

#include "util/Pad.h"

#include "myLib/Physics.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	constexpr float kCameraNear = 5.0f;
	constexpr float kCameraFar = 5000.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };
}


SceneSelect::SceneSelect(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kPowerTypeBoss;

	m_pPlayer = std::make_shared<Player>();
	m_pPlayerWeapon = std::make_shared<PlayerWeapon>();
	m_pCamera = std::make_shared<Camera>();

	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();

	m_pBossShot = std::make_shared<BossShot>();

	m_pField = std::make_shared<Field>();
	m_pSkyDome = std::make_shared<SkyDome>();

	m_pItemHp = std::make_shared<ItemHp>();
	m_pItemMp = std::make_shared<ItemMp>();

	m_pPhysics = std::make_shared<MyLib::Physics>();


	m_playerPos = VGet(0, 0, -120);
	m_cameraPos = VGet(0, 0, 0);

	m_pPlayer->Initialize(m_pPhysics, kInitPos, *m_pPlayerWeapon);
	m_pCamera->Initialize();
	m_pItemHp->Initialize(m_pPhysics);
	m_pItemMp->Initialize(m_pPhysics);
	m_pBossShot->Initialize(m_pPhysics);
	m_pField->Initialize();

	SetCameraNearFar(kCameraNear, kCameraFar);
}

SceneSelect::~SceneSelect()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pItemHp->Finalize(m_pPhysics);
	m_pItemMp->Finalize(m_pPhysics);
	m_pBossShot->Finalize(m_pPhysics);
}

void SceneSelect::Update()
{

#ifdef _DEBUG
	MyLib::DebugDraw::Draw();
#endif

#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

	Pad::Update();
	UpdateFade();

#ifdef _DEBUG
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}

	if (Pad::IsPress(PAD_INPUT_1) && Pad::IsPress(PAD_INPUT_2))
	{
		m_isToNextScene = true;
	}
#endif

	//if (m_isFadingOut)
	//{
	//	if (IsFadingOut())
	//	{
	//		SceneBase::StartFadeIn();
	//		m_isFadingOut = false;
	//	}
	//}

	//UpdateFade();

	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	m_pSkyDome->Update();
	m_pItemHp->Update(m_pPhysics);
	m_pItemMp->Update(m_pPhysics);

	m_pBossShot->Update(m_pPhysics, *m_pPlayer);
	//m_pCamera->Update(m_pField->GetModelHandle());
	//m_pCamera->SetPlayerPos(m_pPlayer->GetPosUp());
	m_pCamera->DebugUpdate(m_pPlayer->GetPosUp());

	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());
	m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon);

	m_pPhysics->Update();

	/*フレームにアタッチするための更新処理*/
	//Physicsの後に入れておかないと補正の影響でワンテンポ遅れる
	m_pPlayerWeapon->SwordUpdate();
	m_pPlayerWeapon->AxeUpdate();
	m_pPlayerWeapon->DaggerUpdate();
	m_pPlayerWeapon->MagicWandUpdate();
	m_pPlayerWeapon->LongSwordUpdate();

	m_pFaceUi->Update();
	m_pPlayerBarUi->Update(*m_pPlayer);

	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
				return;
			}
		}
	}

}

void SceneSelect::Draw()
{
	m_pPlayer->Draw(*m_pPlayerWeapon);

	m_pSkyDome->Draw();
	m_pField->Draw();

	m_pItemHp->Draw();
	m_pItemMp->Draw();

	m_pBossShot->Draw();
	m_pPlayerBarUi->Draw();
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);
	m_pButtonUi->Draw(*m_pPlayer);


#ifdef _DEBUG

	DrawString(0, 0, "Scene Select", 0xffffff, false);

#endif

	DrawFormatString(900, 280, 0xffffff, " PlayerHp : %f ", m_pPlayer->GetHp());


	if (!m_isFadeColor)
	{
		DrawFade(0x000000);
	}
	else
	{
		DrawFade(0xffffff);
	}

}

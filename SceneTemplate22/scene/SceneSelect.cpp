#include "DxLib.h"

#include "SceneManager.h"
#include "SceneSelect.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneGamePlay.h"

#include "object/player/Player.h"
#include "object/Camera.h"

#include "object/item/ItemHp.h"
#include "object/item/ItemMp.h"

#include "object/stage/Field.h"
#include "object/stage/SkyDome.h"
#include "object/stage/Tomb.h"


#include "ui/HpBar.h"
#include "ui/FaceUi.h"

#include "util/Pad.h"

#include "myLib/Physics.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };
}


SceneSelect::SceneSelect(SceneManager& manager) :
	SceneBase(manager)
{
	m_sceneTrans = e_SceneTrans::kPowerTypeBoss;

	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pHpBar = std::make_shared<HpBar>();
	m_pFaceUi = std::make_shared<FaceUi>();

	m_pField = std::make_shared<Field>();
	m_pSkyDome = std::make_shared<SkyDome>();

	m_pItemHp = std::make_shared<ItemHp>();
	m_pItemMp = std::make_shared<ItemMp>();

	m_pPhysics = std::make_shared<MyLib::Physics>();


	m_playerPos = VGet(0, 0, -120);
	m_cameraPos = VGet(0, 0, 0);

	m_pPlayer->Initialize(m_pPhysics, kInitPos);
	m_pItemHp->Initialize(m_pPhysics);
	m_pItemMp->Initialize(m_pPhysics);
	m_pField->Initialize();

}

SceneSelect::~SceneSelect()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pItemHp->Finalize(m_pPhysics);
	m_pItemMp->Finalize(m_pPhysics);
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


	//カメラのアングルをセットする
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

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
	m_pPlayer->Update(m_pPhysics);
	m_pItemHp->Update(m_pPhysics);
	m_pItemMp->Update(m_pPhysics);

	m_pPhysics->Update();

	m_pFaceUi->Update();
	m_pHpBar->Update(*m_pPlayer);

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
	m_pCamera->Draw();
	m_pPlayer->Draw();
	m_pSkyDome->Draw();
	m_pField->Draw();

	m_pItemHp->Draw();
	m_pItemMp->Draw();

	m_pFaceUi->Draw(*m_pPlayer);
	m_pHpBar->Draw();

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

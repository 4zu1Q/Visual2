#include "DxLib.h"

#include "myLib/MyLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "ScenePause.h"

#include "object/player/Player.h"
#include "object/boss/BossPower.h"
#include "object/item/ItemBase.h"
#include "object/SkyDome.h"
#include "object/Camera.h"

#include "ui/HpBar.h"
#include "ui/FaceUi.h"

#include "util/Pad.h"

namespace
{

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
	m_isFadingOut(false),
	m_cameraAngle(0.0f)
{
	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pHpBar = std::make_shared<HpBar>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pItem = std::make_shared<ItemBase>();
	m_pBoss = std::make_shared<BossPower>();


	m_playerPos = VGet(30, 0, 20);
	m_cameraPos = VGet(0, 0, 0);

	//初期位置をセット
	m_pPlayer->Initialize(m_pPhysics);
	m_pBoss->Initialize();

}

SceneGamePlay::~SceneGamePlay()
{
	m_pPlayer->Finalize(m_pPhysics);
}


void SceneGamePlay::Update()
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
	m_pBoss->Update();
	m_pItem->Update();

	m_pPhysics->Update();

	m_pFaceUi->Update();
	m_pHpBar->Update(*m_pPlayer);


	//プレイヤーとアイテムの当たり判定のフラグを代入
	m_isMpHit = m_pItem->ItemHit(m_pPlayer);

	


}

void SceneGamePlay::Draw()
{
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);

	m_pCamera->Draw();
	m_pSkyDome->Draw();
	m_pPlayer->Draw();
	m_pBoss->Draw();
	m_pFaceUi->Draw(*m_pPlayer);
	m_pHpBar->Draw();

	m_pItem->Draw(m_pPlayer);

	DrawFade(0xffffff);
	
}

//void SceneGamePlay::StartFadeOut()
//{
//	SceneBase::StartFadeOut();
//	m_isFadingOut = true;
//}

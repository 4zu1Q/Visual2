#include "DxLib.h"

#include "SceneManager.h"
#include "SceneSelect.h"
#include "ScenePause.h"
#include "SceneOption.h"
#include "SceneGamePlay.h"

#include "object/player/Player.h"
#include "object/item/ItemBase.h"	//後々消す

#include "object/item/ItemHp.h"
#include "object/item/ItemMp.h"

#include "object/SkyDome.h"
#include "object/Camera.h"
#include "object/Tomb.h"

#include "ui/HpBar.h"
#include "ui/FaceUi.h"

#include "util/Pad.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;
}


SceneSelect::SceneSelect(SceneManager& manager) :
	SceneBase(manager),
	m_isMpHit(false),
	m_isHpHit(false),
	m_isTombHitP(false),
	m_isTombHitS(false),
	m_isTombHitR(false)
{
	m_sceneTrans = e_SceneTrans::kPowerTypeBoss;

	m_pPlayer = std::make_shared<Player>();
	m_pCamera = std::make_shared<Camera>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pHpBar = std::make_shared<HpBar>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pItem = std::make_shared<ItemBase>();

	m_pItemHp = std::make_shared<ItemHp>();
	m_pItemMp = std::make_shared<ItemMp>();

	m_pTomb = std::make_shared<Tomb>();


	m_playerPos = VGet(0, 0, -120);
	m_cameraPos = VGet(0, 0, 0);

	m_pPlayer->Initialize(m_playerPos);
}

SceneSelect::~SceneSelect()
{
}

void SceneSelect::Update()
{
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
	m_pPlayer->Update();
	m_pFaceUi->Update();
	m_pHpBar->Update(*m_pPlayer);

	m_pItem->Update();
	m_pItemMp->Update();
	m_pItemHp->Update();
	m_pTomb->Update();

	//プレイヤーとアイテムの当たり判定のフラグを代入
	m_isMpHit = m_pItemMp->MpHit(m_pPlayer);
	m_isHpHit = m_pItemHp->HpHit(m_pPlayer);

	m_isTombHitP = m_pTomb->TombPHit(m_pPlayer);
	m_isTombHitS = m_pTomb->TombSHit(m_pPlayer);
	m_isTombHitR = m_pTomb->TombRHit(m_pPlayer);



	//プレイヤーとアイテムが当たった場合

	if (m_isMpHit)
	{
		printfDx("Mp");
	}

	if (m_isHpHit)
	{
		printfDx("Hp");
	}

	//プレイヤーと墓が当たった場合

	if (Pad::IsTrigger(PAD_INPUT_1) && m_isTombHitP)
	{
		printfDx("パワーボス");
		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		return;
	}

	if (Pad::IsTrigger(PAD_INPUT_1) && m_isTombHitS)
	{
		printfDx("スピードボス");
		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		return;
	}

	if (Pad::IsTrigger(PAD_INPUT_1) && m_isTombHitR)
	{
		printfDx("ラスボス");
		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		return;
	}

	//if (!m_isToNextScene)
	//{
		////上に移動した場合
		//if (Pad::IsTrigger(PAD_INPUT_UP))
		//{
		//	if (m_sceneTrans != e_SceneTrans::kPowerTypeBoss)
		//	{
		//		m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) - 1);
		//	}
		//}

		////下に移動した場合
		//if (Pad::IsTrigger(PAD_INPUT_DOWN))
		//{
		//	if (m_sceneTrans != e_SceneTrans::kRastBoss)
		//	{
		//		m_sceneTrans = static_cast<e_SceneTrans>(static_cast<int>(m_sceneTrans) + 1);
		//	}
		//}

		////決定ボタンを押した場合
		//if (Pad::IsTrigger(PAD_INPUT_1))
		//{
		//	if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
		//	{
		//		StartFadeOut();
		//		m_isToNextScene = true;

		//		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		//		return;
		//	}
		//	if (m_sceneTrans == e_SceneTrans::kSpeedTypeBoss)
		//	{
		//		StartFadeOut();
		//		m_isToNextScene = true;

		//		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		//		return;
		//	}
		//	if (m_sceneTrans == e_SceneTrans::kShooterTypeBoss)
		//	{
		//		StartFadeOut();
		//		m_isToNextScene = true;

		//		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		//		return;
		//	}
		//	if (m_sceneTrans == e_SceneTrans::kRastBoss)
		//	{
		//		StartFadeOut();
		//		m_isToNextScene = true;

		//		m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager));
		//		return;
		//	}
		//}
	//}


	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			//if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			//	return;
			//}
			//
			//if (m_sceneTrans == e_SceneTrans::kSpeedTypeBoss)
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			//	return;
			//}
			//
			//if (m_sceneTrans == e_SceneTrans::kShooterTypeBoss)
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			//	return;
			//}

			//if (m_sceneTrans == e_SceneTrans::kRastBoss)
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager));
			//	return;
			//}
		}
	}

}

void SceneSelect::Draw()
{
	m_pCamera->Draw();
	m_pSkyDome->Draw();
	m_pPlayer->Draw();
	m_pFaceUi->Draw(*m_pPlayer);
	m_pHpBar->Draw();

	m_pItem->Draw(m_pPlayer);
	m_pItemHp->Draw();
	m_pItemMp->Draw();
	m_pTomb->Draw();

//#ifdef _DEBUG

	DrawString(0, 0, "Scene Select", 0xffffff, false);

//#endif

	DrawBlackFade();
}

#include "DxLib.h"

#include "myLib/MyLib.h"

#include "SceneManager.h"
#include "SceneGamePlay.h"
#include "SceneGameOver.h"
#include "ScenePause.h"
#include "SceneDebug.h"

#include "object/player/Player.h"
#include "object/player/PlayerWeapon.h"
#include "object/boss/BossPower.h"
#include "object/boss/BossSpeed.h"
#include "object/boss/BossShot.h"
#include "object/Camera.h"
#include "object/Camera2.h"

#include "ui/HpBar.h"
#include "ui/PlayerBarUi.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"

//#include "ui/GamePlayUi.h"

#include "util/Pad.h"
#include "util/SoundManager.h"

namespace
{

	//初期位置
	constexpr VECTOR kInitPos = { 20.0f,15.0f,-100.0f };

}

SceneGamePlay::SceneGamePlay(SceneManager& manager , Game::e_BossKind bosskind , Game::e_StageKind stageKind) :
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
	m_selectTime(0),
	m_cameraAngle(0.0f)
{
	m_bossKind = e_BossKind::kPower;

	m_pPlayer = std::make_shared<Player>();
	m_pPlayerWeapon = std::make_shared<PlayerWeapon>();

	m_pHpBarUi = std::make_shared<HpBar>();
	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();

	m_pBossPower = std::make_shared<BossPower>();
	m_pBossSpeed = std::make_shared<BossSpeed>();
	m_pBossShot = std::make_shared<BossShot>();

	m_pField = std::make_shared<Field>(stageKind);

	m_pPhysics = std::make_shared<MyLib::Physics>(stageKind);

	m_pCamera = std::make_shared<Camera>();
	m_pCamera2 = std::make_shared<Camera2>();

	m_playerPos = VGet(30, 0, 20);
	m_cameraPos = VGet(0, 0, 0);

	m_bossKind = bosskind;

	//初期位置をセット
	m_pPlayer->Initialize(m_pPhysics, kInitPos, *m_pPlayerWeapon);

	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pBossPower->Initialize(m_pPhysics);
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pBossSpeed->Initialize(m_pPhysics);
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pBossShot->Initialize(m_pPhysics);
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		m_pBossShot->Initialize(m_pPhysics);
	}

	m_pCamera2->Initialize(m_pPlayer->GetPos());

	m_pField->Initialize();

}

SceneGamePlay::~SceneGamePlay()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pBossPower->Finalize(m_pPhysics);
	m_pBossSpeed->Finalize(m_pPhysics);
	m_pBossShot->Finalize(m_pPhysics);
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

	//ボスのHPが0になった場合
	if (m_pBossPower->GetHp() > 0)
	{
		m_selectTime++;
	}

	//プレイヤーのゲームオーバーフラグがtrueの場合
	if (m_pPlayer->GetIsGameOver())
	{
		m_gameOverTime++;
	}
	

	//ゲームオーバー時間が過ぎたら
	if (m_gameOverTime > 240)
	{
		m_isToNextScene = true;
		StartFadeOut();

		//なんか入らん後でやる
		//if (!IsFadingOut())
		{
			m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager));
			return;
		}
	}


	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			SoundManager::GetInstance().PlaySe("pouseSe");
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}


	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pBossPower->Update(m_pPhysics, *m_pPlayer);
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pBossSpeed->Update(m_pPhysics, *m_pPlayer);
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pBossShot->Update(m_pPhysics, *m_pPlayer);
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		//m_pBossRast->Update(m_pPhysics, *m_pPlayer);
	}

	m_pPlayer->SetCameraDirection(m_pCamera2->GetDirection());
	m_pCamera2->Update(m_pPlayer->GetPos(), m_pField->GetModelHandle());

	m_pPlayer->Update(m_pPhysics,*m_pPlayerWeapon,m_pCamera->GetCameraAngleX());

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

	}

}

void SceneGamePlay::Draw()
{

	m_pField->Draw();

	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pBossPower->Draw();
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pBossSpeed->Draw();
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pBossShot->Draw();
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		//m_pBossRast->Draw();
	}

	m_pPlayer->Draw(*m_pPlayerWeapon);

	m_pHpBarUi->Draw();
	m_pPlayerBarUi->Draw();
	m_pButtonUi->Draw(*m_pPlayer);
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);

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

#include "DxLib.h"

#include "myLib/MyLib.h"

#include "scene/SceneManager.h"
#include "scene/SceneGamePlay.h"
#include "scene/SceneGameOver.h"
#include "scene/SceneGameClear.h"
#include "scene/ScenePause.h"
#include "scene/SceneSelect.h"
#include "scene/SceneDebug.h"

#include "object/player/Player.h"
#include "object/player/PlayerWeapon.h"
#include "object/boss/BossPower.h"
#include "object/boss/BossSpeed.h"
#include "object/boss/BossShot.h"
#include "object/boss/BossRast.h"
#include "object/Camera.h"
#include "object/Camera2.h"
#include "object/stage/Tomb.h"

#include "ui/HpBar.h"
#include "ui/PlayerBarUi.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"

//#include "ui/GamePlayUi.h"

#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/SaveDataManager.h"
#include "util/EffectManager.h"

namespace
{

	//初期位置
	constexpr VECTOR kInitPos = { 20.0f,15.0f,-100.0f };
	constexpr VECTOR kLightUpPos = { -15.0f,15.0f, -27.0f };
	constexpr VECTOR kLightDownPos = { -15.0f,15.0f, -27.0f };

	const Vec2 kHitPos = { 440.0f , 480.0f };

	//使う画像の種類
	enum e_Ui
	{
		kStageTransH,
	};
}

SceneGamePlay::SceneGamePlay(SceneManager& manager, Game::e_BossKind bosskind, Game::e_StageKind stageKind) :
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
	m_gameClearTime(0),
	m_selectTime(0),
	m_cameraAngle(0.0f)
{
	m_isToNextScene = false;

	m_isCameraLockOn = true;

	m_isPowerTriangl = false;
	m_isSpeedTriangl = false;
	m_isShotTriangl = false;

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
	m_pBossRast = std::make_shared<BossRast>();

	m_pField = std::make_shared<Field>(stageKind);
	m_pTomb = std::make_shared<Tomb>();

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
		m_pPlayer->BossLook(m_pBossPower->GetPosDown());
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pBossSpeed->Initialize(m_pPhysics);
		m_pPlayer->BossLook(m_pBossSpeed->GetPosDown());
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pBossShot->Initialize(m_pPhysics);
		m_pPlayer->BossLook(m_pBossShot->GetPosDown());
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		m_pBossRast->Initialize(m_pPhysics);
		m_pPlayer->BossLook(m_pBossRast->GetPosDown());
	}

	m_pCamera2->Initialize(m_pPlayer->GetPos());


	m_pTomb->Initialize();

	m_pField->Initialize();

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/StageButton.png"));


	//戦闘用BGMを最初に流す
	SoundManager::GetInstance().PlayBgm("battleBgm", true);

}

SceneGamePlay::~SceneGamePlay()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pBossPower->Finalize(m_pPhysics);
	m_pBossSpeed->Finalize(m_pPhysics);
	m_pBossShot->Finalize(m_pPhysics);
	m_pBossRast->Finalize(m_pPhysics);

	DeleteLightHandle(m_lightHandle);

	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	SoundManager::GetInstance().StopBgm("stageClearBgm");

	m_handles.clear();
}


void SceneGamePlay::Update()
{
	Pad::Update();
	UpdateFade();
	UpdateFadeSelectGraph();
	EffectManager::GetInstance().Update();

#ifdef _DEBUG
	MyLib::DebugDraw::Draw();
#endif

#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

	//ボスのHPバーのアップデート処理
	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pHpBarUi->Update(m_pBossPower->GetHp());
		
		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossPower->GetHitPos(), m_pBossPower->GetAttackPos(), m_pBossPower->GetAttackPos(), m_pBossPower->GetShockPos(), m_pBossPower->GetHitRadius(), m_pBossPower->GetAttackRadius(), m_pBossPower->GetWeaponRadius(), m_pBossPower->GetShockRadius(), m_pBossPower->GetIsAttack());
		m_pBossPower->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(), m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(), m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pHpBarUi->Update(m_pBossSpeed->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossSpeed->GetHitPos(),m_pBossSpeed->GetAttackPos(), m_pBossSpeed->GetAttackPos(), m_pBossSpeed->GetShockPos(), m_pBossSpeed->GetHitRadius(), m_pBossSpeed->GetAttackRadius(), m_pBossSpeed->GetWeaponRadius(), m_pBossSpeed->GetShockRadius(), m_pBossSpeed->GetIsAttack());
		m_pBossSpeed->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(), m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(), m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pHpBarUi->Update(m_pBossShot->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossShot->GetHitPos(), m_pBossShot->GetAttackPos(), m_pBossShot->GetAttackPos(), m_pBossShot->GetShockPos(), m_pBossShot->GetHitRadius(), m_pBossShot->GetAttackRadius(), m_pBossShot->GetWeaponRadius(), m_pBossShot->GetShockRadius(), m_pBossShot->GetIsAttack());
		m_pBossShot->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(), m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(), m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		m_pHpBarUi->Update(m_pBossRast->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossRast->GetHitPos(), m_pBossRast->GetAttackPos(), m_pBossRast->GetAttackPos(), m_pBossRast->GetShockPos(), m_pBossRast->GetHitRadius(), m_pBossRast->GetAttackRadius(), m_pBossRast->GetWeaponRadius(), m_pBossRast->GetShockRadius(), m_pBossRast->GetIsAttack());
		m_pBossRast->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(), m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(), m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());
	}

	//ゲームステージに移動するための当たり判定
	if(m_pBossPower->GetIsClear()) m_isPowerTriangl = m_pTomb->TrianglePowerHit(m_pPlayer);
	if(m_pBossSpeed->GetIsClear()) m_isSpeedTriangl = m_pTomb->TriangleSpeedHit(m_pPlayer);
	if(m_pBossShot->GetIsClear()) m_isShotTriangl = m_pTomb->TriangleShotHit(m_pPlayer);
	
	//プレイヤーのゲームオーバーフラグがtrueの場合
	if (m_pPlayer->GetIsGameOver())
	{
		m_isCameraLockOn = false;
		m_gameOverTime++;

		SoundManager::GetInstance().StopBgm("battleBgm");

		StartFadeOut();
		m_isToNextScene = true;
	}

	if (m_pBossRast->GetIsClear())
	{
		m_isCameraLockOn = false;
		m_gameClearTime++;
	}

	//ゲームオーバー時間が過ぎたら
	if (m_gameOverTime > 180)
	{
		m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager, m_bossKind));
		return;
	}

	if (!IsFading())
	{
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			SoundManager::GetInstance().PlaySe("pouseSe");
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	//パワーボスを倒した場合
	if (m_pBossPower->GetIsClear())
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		SoundManager::GetInstance().PlayBgm("stageClearBgm", true);
		m_isCameraLockOn = false;
		m_pTomb->Update(m_pBossPower->GetPosUp(), m_pBossSpeed->GetPosUp(), m_pBossShot->GetPosUp());
		//パワークリアトライアングルの当たり判定に入った場合
		if (m_isPowerTriangl)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				SoundManager::GetInstance().PlaySe("selectTransSe");
				SoundManager::GetInstance().StopBgm("stageClearBgm");
				//パワータイプの顔が使用できるようになる
				SaveDataManager::GetInstance().OnRelease(Game::e_PlayerKind::kPowerPlayer);
				StartFadeOut();
				m_isToNextScene = true;

			}
		}
	}
	//スピードボスを倒した場合
	if (m_pBossSpeed->GetIsClear())
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		SoundManager::GetInstance().PlayBgm("stageClearBgm", true);
		m_isCameraLockOn = false;
		m_pTomb->Update(m_pBossPower->GetPosUp(), m_pBossSpeed->GetPosUp(), m_pBossShot->GetPosUp());
		//スピードクリアトライアングルの当たり判定に入った場合
		if (m_isSpeedTriangl)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				SoundManager::GetInstance().PlaySe("selectTransSe");
				SoundManager::GetInstance().StopBgm("stageClearBgm");
				//スピードタイプの顔が使用できるようになる
				SaveDataManager::GetInstance().OnRelease(Game::e_PlayerKind::kSpeedPlayer);
				StartFadeOut();
				m_isToNextScene = true;

			}
		}
	}
	//ショットボスを倒した場合
	if (m_pBossShot->GetIsClear())
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		SoundManager::GetInstance().PlayBgm("stageClearBgm", true);
		m_isCameraLockOn = false;
		m_pTomb->Update(m_pBossPower->GetPosUp(), m_pBossSpeed->GetPosUp(), m_pBossShot->GetPosUp());
		//ショットクリアトライアングルの当たり判定に入った場合
		if (m_isShotTriangl)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				SoundManager::GetInstance().PlaySe("selectTransSe");
				SoundManager::GetInstance().StopBgm("stageClearBgm");
				//ショットタイプの顔が使用できるようになる
				SaveDataManager::GetInstance().OnRelease(Game::e_PlayerKind::kShotPlayer);
				StartFadeOut();
				m_isToNextScene = true;

			}
		}
	}

	//ラスボスを倒した場合
	if (m_gameClearTime > 240)
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		//ラスボスタイプの顔が使用できるようになる
		SaveDataManager::GetInstance().OnRelease(Game::e_PlayerKind::kRassPlayer);

		m_pManager.ChangeScene(std::make_shared<SceneGameClear>(m_pManager));
		return;

	}

	//ボスのアップデート処理
	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pBossPower->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pBossSpeed->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pBossShot->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		m_pBossRast->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());
	}

	m_pPlayer->SetCameraDirection(m_pCamera2->GetDirection());

	//ボス別々にロックオンするための処理
	if (m_bossKind == Game::e_BossKind::kPower)
	{
		m_pCamera2->Update(m_pPlayer->GetPos(), m_pBossPower->GetPosUp(), m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);
		m_pPlayer->Update(m_pPhysics,*m_pPlayerWeapon,m_pCamera->GetCameraAngleX(),m_pBossPower->GetPosDown(),m_pCamera2->GetIsLockOn(),m_pBossPower->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		m_pCamera2->Update(m_pPlayer->GetPos(), m_pBossSpeed->GetPosUp(), m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(), m_pBossSpeed->GetPosDown(), m_pCamera2->GetIsLockOn(),m_pBossSpeed->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		m_pCamera2->Update(m_pPlayer->GetPos(), m_pBossShot->GetPosUp(), m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(), m_pBossShot->GetPosDown(), m_pCamera2->GetIsLockOn(), m_pBossShot->GetAttackKind());
	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		m_pCamera2->Update(m_pPlayer->GetPos(), m_pBossRast->GetPosUp(), m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(), m_pBossRast->GetPosDown(), m_pCamera2->GetIsLockOn(), m_pBossRast->GetAttackKind());
	}

	m_pPhysics->Update();

	/*フレームにアタッチするための更新処理*/
	//Physicsの後に入れておかないと補正の影響でワンテンポ遅れる
	m_pPlayerWeapon->SwordUpdate();

	m_pFaceUi->Update();
	m_pPlayerBarUi->Update(*m_pPlayer);

	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			//フェードさせるやつ
			//if (m_pPlayer->GetIsGameOver())
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager, m_bossKind));
			//	return;
			//}

			if (m_pBossPower->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
				return;
			}

			if (m_pBossSpeed->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
				return;
			}

			if (m_pBossShot->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect));
				return;
			}

			//if (m_pBossRast->GetIsClear())
			//{
			//	m_pManager.ChangeScene(std::make_shared<SceneGameClear>(m_pManager));
			//	return;
			//}
		}
	}

}

void SceneGamePlay::Draw()
{

	m_pField->Draw();

	if (m_pBossPower->GetIsClear() || m_pBossSpeed->GetIsClear() || m_pBossShot->GetIsClear())
	{
		m_pTomb->DrawTriangle(m_bossKind);
	}

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
		m_pBossRast->Draw();
	}

	m_pPlayer->Draw(*m_pPlayerWeapon);

	m_pCamera2->Draw();

	if (!m_pBossPower->GetIsClear() && !m_pBossSpeed->GetIsClear() && !m_pBossShot->GetIsClear())
	{
		m_pHpBarUi->Draw();
		m_pHpBarUi->DrawBossName(m_bossKind);
	}

	m_pPlayerBarUi->Draw();
	m_pButtonUi->Draw(*m_pPlayer);
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);

	if (m_isPowerTriangl || m_isSpeedTriangl || m_isShotTriangl)
	{
		DrawFadeSelectGraph(m_handles[kStageTransH], kHitPos);
	}

	EffectManager::GetInstance().Draw();

	//クリアしたときのみ白いフェードにしておく
	if (m_pBossPower->GetIsClear() || m_pBossSpeed->GetIsClear() || m_pBossShot->GetIsClear() || m_pBossRast->GetIsClear())
	{
		DrawFade(0xffffff);
	}
	else
	{
		DrawFade(0x000000);
	}



#ifdef _DEBUG

	DrawString(0, 0, "Scene Game Play", 0xffffff, false);


	//if (m_isTriangl)
	//{
	//	DrawString(0, 600, "Hit", 0xffffff, false);
	//}

#endif // DEBUG

}

//void SceneGamePlay::StartFadeOut()
//{
//	SceneBase::StartFadeOut();
//	m_isFadingOut = true;
//}

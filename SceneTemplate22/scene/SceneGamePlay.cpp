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
#include "object/stage/Tomb.h"

#include "ui/HpBar.h"
#include "ui/PlayerBarUi.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"


#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/SaveDataManager.h"
#include "util/EffectManager.h"

namespace
{

	//初期位置
	constexpr VECTOR kInitPos = { 20.0f,10.0f,-100.0f };
	constexpr VECTOR kLightUpPos = { -15.0f,15.0f, -27.0f };
	constexpr VECTOR kLightDownPos = { -15.0f,15.0f, -27.0f };

	constexpr VECTOR kPlayerPowerTombPos = { -168.0f,-37.0f,624.0f };
	constexpr VECTOR kPlayerSpeedTombPos = { 979.0f,-37.0f,608.0f };
	constexpr VECTOR kPlayerShotTombPos = { 409.0f,-37.0f,-363.0f };

	constexpr int kGameOverTimeMax = 180;
	constexpr int kGameClearTimeMax = 240;

	const Vec2 kHitPos = { 440.0f , 480.0f };

	//使う画像の種類
	enum e_Ui
	{
		kStageTransH,
	};

	//プレイヤーの最初の位置
	constexpr VECTOR kPlayerPos = { 350.0f,-35.0f,0 };

	constexpr int kShadowMapSize = 16384;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = { -600.0f, -300.0f, -600.0f };		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = { 600.0f, 0.0f, 600.0f };	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = { 0.0f, -5.0f, 0.0f };					// ライト方向

	constexpr float kShadowColor = 0.7f;
	constexpr float kShadowAlpha = 0.3f;

	constexpr float kPlayerAngle = -3.149639f;
	constexpr float kCameraAngleH = -1.562750f;

}

SceneGamePlay::SceneGamePlay(SceneManager& manager, Game::e_BossKind bosskind, Game::e_StageKind stageKind) :
	SceneBase(manager),
	m_gameOverTime(0),
	m_gameClearTime(0),
	m_selectTime(0)
{
	m_isToNextScene = false;

	m_isCameraLockOn = true;
	m_isPowerTriangl = false;
	m_isSpeedTriangl = false;
	m_isShotTriangl = false;

	m_bossKind = bosskind;

	m_pPlayer = std::make_shared<Player>();
	m_pPlayerWeapon = std::make_shared<PlayerWeapon>();

	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();
	m_pHpBarUi = std::make_shared<HpBar>();

	m_pBossPower = std::make_shared<BossPower>();
	m_pBossSpeed = std::make_shared<BossSpeed>();
	m_pBossShot = std::make_shared<BossShot>();
	m_pBossRast = std::make_shared<BossRast>();

	m_pCamera = std::make_shared<Camera>();

	m_pField = std::make_shared<Field>(stageKind);
	m_pTomb = std::make_shared<Tomb>();

	m_pPhysics = std::make_shared<MyLib::Physics>(stageKind);

	//初期位置をセット
	m_pPlayer->Initialize(m_pPhysics, kInitPos, *m_pPlayerWeapon, kPlayerAngle);

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

	m_pCamera->Initialize(m_pPlayer->GetPos(), kCameraAngleH);
	m_pTomb->Initialize(m_pBossPower->GetPosUp(), m_pBossSpeed->GetPosUp(), m_pBossShot->GetPosUp());
	m_pField->Initialize();

	//画像のロード
	m_handles.push_back(LoadGraph("Data/Image/StageButton.png"));

	/* 影の初期設定 */
	m_shadowMap = MakeShadowMap(kShadowMapSize, kShadowMapSize);
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMap, kShadowAreaMinPos, kShadowAreaMaxPos);
	// シャドウマップが想定するライトの方向をセット
	SetShadowMapLightDirection(m_shadowMap, kShadowDir);

	// 影色を調整
	SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 0.0f));

}

SceneGamePlay::~SceneGamePlay()
{
	//シャドウマップの削除
	DeleteShadowMap(m_shadowMap);
	m_shadowMap = -1;

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

	//戦闘用BGMを最初に流す
	SoundManager::GetInstance().PlayBgm("battleBgm", true);

#ifdef _DEBUG
	MyLib::DebugDraw::Draw();
#endif

#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

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
	if (m_gameOverTime > kGameOverTimeMax)
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
		m_pTomb->Update();
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
		m_pTomb->Update();
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
		m_pTomb->Update();
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
	if (m_gameClearTime > kGameClearTimeMax)
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		//ラスボスタイプの顔が使用できるようになる
		SaveDataManager::GetInstance().OnRelease(Game::e_PlayerKind::kRassPlayer);

		m_pManager.ChangeScene(std::make_shared<SceneGameClear>(m_pManager));
		return;
	}

	m_pPlayer->SetCameraDirection(m_pCamera->GetDirection());

	//ボス別々にロックオンするための処理
	if (m_bossKind == Game::e_BossKind::kPower)
	{
		//カメラのアップデート処理
		m_pCamera->Update(m_pPlayer->GetPos(), m_pBossPower->GetPosUp(),
			m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);

		//プレイヤーのアップデート処理
		m_pPlayer->Update(m_pPhysics,*m_pPlayerWeapon, m_pCamera->GetCameraAngleX(),
			m_pBossPower->GetPosDown(), m_pCamera->GetIsLockOn(),m_pBossPower->GetAttackKind());

		//パワーボスのアップデート処理
		m_pBossPower->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());

		//ボスのHPバーのアップデート処理
		m_pHpBarUi->Update(m_pBossPower->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossPower->GetHitPos(), m_pBossPower->GetAttackPos(),
			m_pBossPower->GetAttackPos(), m_pBossPower->GetShockPos(), m_pBossPower->GetHitRadius(),
			m_pBossPower->GetAttackRadius(), m_pBossPower->GetWeaponRadius(), m_pBossPower->GetShockRadius(), 
			m_pBossPower->GetIsAttack());

		//プレイヤーの攻撃座標や半径を取得
		m_pBossPower->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(), 
			m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(),
			m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());

	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		//カメラのアップデート処理
		m_pCamera->Update(m_pPlayer->GetPos(), m_pBossSpeed->GetPosUp(),
			m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);

		//プレイヤーのアップデート処理
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(),
			m_pBossSpeed->GetPosDown(), m_pCamera->GetIsLockOn(),m_pBossSpeed->GetAttackKind());

		//スピードボスのアップデート処理
		m_pBossSpeed->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());

		//ボスのHPバーのアップデート処理
		m_pHpBarUi->Update(m_pBossSpeed->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossSpeed->GetHitPos(), m_pBossSpeed->GetAttackPos(),
			m_pBossSpeed->GetAttackPos(), m_pBossSpeed->GetShockPos(), m_pBossSpeed->GetHitRadius(),
			m_pBossSpeed->GetAttackRadius(), m_pBossSpeed->GetWeaponRadius(), m_pBossSpeed->GetShockRadius(),
			m_pBossSpeed->GetIsAttack());

		//プレイヤーの攻撃座標や半径を取得
		m_pBossSpeed->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(),
			m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(),
			m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());

	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		//カメラのアップデート処理
		m_pCamera->Update(m_pPlayer->GetPos(), m_pBossShot->GetPosUp(),
			m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);

		//プレイヤーのアップデート処理
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(),
			m_pBossShot->GetPosDown(), m_pCamera->GetIsLockOn(), m_pBossShot->GetAttackKind());

		//ショットボスのアップデート処理
		m_pBossShot->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());

		//ボスのHPバーのアップデート処理
		m_pHpBarUi->Update(m_pBossShot->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossShot->GetHitPos(), m_pBossShot->GetAttackPos(),
			m_pBossShot->GetAttackPos(), m_pBossShot->GetShockPos(), m_pBossShot->GetHitRadius(),
			m_pBossShot->GetAttackRadius(), m_pBossShot->GetWeaponRadius(), m_pBossShot->GetShockRadius(),
			m_pBossShot->GetIsAttack());

		//プレイヤーの攻撃座標や半径を取得
		m_pBossShot->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(),
			m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(),
			m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());

	}
	else if (m_bossKind == Game::e_BossKind::kRast)
	{
		//カメラのアップデート処理
		m_pCamera->Update(m_pPlayer->GetPos(), m_pBossRast->GetPosUp(),
			m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);

		//プレイヤーのアップデート処理
		m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(),
			m_pBossRast->GetPosDown(), m_pCamera->GetIsLockOn(), m_pBossRast->GetAttackKind());

		//ラストボスのアップデート処理
		m_pBossRast->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());

		//ボスのHPバーのアップデート処理
		m_pHpBarUi->Update(m_pBossRast->GetHp());

		//ボスの攻撃座標や半径を取得
		m_pPlayer->HitUpdate(m_pBossRast->GetHitPos(), m_pBossRast->GetAttackPos(), 
			m_pBossRast->GetAttackPos(), m_pBossRast->GetShockPos(), m_pBossRast->GetHitRadius(),
			m_pBossRast->GetAttackRadius(), m_pBossRast->GetWeaponRadius(), m_pBossRast->GetShockRadius(),
			m_pBossRast->GetIsAttack());

		//プレイヤーの攻撃座標や半径を取得
		m_pBossRast->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(),
			m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(),
			m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());

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
			if (m_pBossPower->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerPowerTombPos));
				return;
			}

			if (m_pBossSpeed->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerSpeedTombPos));
				return;
			}

			if (m_pBossShot->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerShotTombPos));
				return;
			}
		}
	}
}

void SceneGamePlay::Draw()
{

	ShadowDraw();

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

	m_pCamera->Draw();

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
	if (m_pBossPower->GetIsClear() ||
		m_pBossSpeed->GetIsClear() ||
		m_pBossShot->GetIsClear()  ||
		m_pBossRast->GetIsClear())
	{
		DrawFade(0xffffff);
	}
	else
	{
		DrawFade(0x000000);
	}

#ifdef _DEBUG
	DrawString(0, 0, "Scene Game Play", 0xffffff, false);
#endif // DEBUG

}

void SceneGamePlay::ShadowDraw()
{
	ShadowMap_DrawSetup(m_shadowMap);	//シャドウマップ描画開始

	m_pPlayer->Draw(*m_pPlayerWeapon);

	if (m_bossKind == Game::e_BossKind::kPower)
	{
		if(m_pBossPower->GetHp() != 0)
		{
			m_pBossPower->Draw();
		}
	}
	else if (m_bossKind == Game::e_BossKind::kSpeed)
	{
		if (m_pBossSpeed->GetHp() != 0)
		{
			m_pBossSpeed->Draw();
		}
	}
	else if (m_bossKind == Game::e_BossKind::kShot)
	{
		if (m_pBossShot->GetHp() != 0)
		{
			m_pBossShot->Draw();
		}
	}
	else if (m_bossKind == Game::e_BossKind::kRast && !m_pBossRast->GetIsClear())
	{
		m_pBossRast->Draw();
	}



	ShadowMap_DrawEnd();	//シャドウマップ描画終了

	SetUseShadowMap(0, m_shadowMap);	// シャドウマップの反映開始

	m_pField->Draw();

	SetUseShadowMap(0, -1);	// シャドウマップの反映終了
}

﻿#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/ScenePause.h"
#include "scene/SceneOption.h"
#include "scene/SceneSelect.h"
#include "scene/SceneGameOver.h"
#include "scene/SceneWords.h"
#include "scene/SceneDebug.h"
#include "scene/SceneTutorial.h"

#include "object/player/Player.h"
#include "object/player/PlayerWeapon.h"
#include "object/enemy/EnemyNormal.h"
#include "object/enemy/EnemySpecial.h"
#include "object/boss/BossTutorial.h"
#include "object/Camera.h"

#include "object/item/ItemHp.h"
#include "object/item/ItemMp.h"

#include "object/stage/Field.h"
#include "object/stage/SkyDome.h"

#include "ui/HpBar.h"
#include "ui/FaceUi.h"
#include "ui/FaceFrameUi.h"
#include "ui/ButtonUi.h"
#include "ui/PlayerBarUi.h"

#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/EffectManager.h"
#include "util/Font.h"
#include "util/SaveDataManager.h"

#include "myLib/Physics.h"

namespace
{
	constexpr int kTextX = 64;
	constexpr int kTextBlankSpaceY = 32;
	constexpr int kTextIntervalY = 24;

	enum e_Ui
	{
		kHitH,
	};

	constexpr int kGameOverTimeMax = 180;

	//フォントのパス
	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";

	//フォントサイズ	
	constexpr int kFontSize = 38;

	//エフェクトを出すフレーム数
	constexpr int kEffectFrame = 190;
	//チュートリアルのフレーム数
	constexpr int kTutorialFrame = 60;
	constexpr int kTutorialBossFrame = 180;

	//チュートリアルの説明を行う位置
	constexpr float kTutorialJumpPosZ = 405.0f;
	constexpr float kTutorialJumpClearPosZ = 292.0f;
	constexpr float kTutorialDashJumpPosZ = 92.0f;
	constexpr float kTutorialDashJumpClearPosZ = -208.0f;
	constexpr float kTutorialActionPosZ = -117.0f;
	constexpr float kTutorialBossBattlePosZ = -420.0f;
	constexpr float kTutorialSelectPosZ = -875.0f;

	//ボスに行く部屋に表示される画像の座標
	const Vec2 kHitPos = { 440.0f , 480.0f };
	const Vec2 kHitBossPos = { 340.0f , 30.0f };
	const Vec2 kHitTextPos = { 740.0f , 230.0f };
	const Vec2 kHitText2Pos = { 743.0f , 230.0f };
	const Vec2 kHitItemPos = { 790.0f , 160.0f };
	const Vec2 kHitStarPos = { 790.0f , 220.0f };

	//調整する座標
	const Vec2 kHitAdjustmentPos = { 25.0f , 40.0f };
	const Vec2 kHitAdjustmentUpPos = { 25.0f , 80.0f };

	//プレイヤーの最初の位置
	constexpr VECTOR kPlayerPos = { 383.0f,-53.0f,450.0f };
	constexpr VECTOR kSelectPlayerPos = { 400.0f,-35.0f,740.0f };

	constexpr int kShadowMapSize = 16384;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = { -0.0f, -70.0f, -1000.0f };		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = { 800.0f, 50.0f, 1000.0f };	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = { 0.0f, -5.0f, 0.0f };					// ライト方向

	constexpr float kShadowColor = 0.7f;
	constexpr float kShadowAlpha = 0.3f;

	constexpr float kPlayerAngle = 0.014954f;
	constexpr float kCameraAngleH = 1.562750f;

}

SceneTutorial::SceneTutorial(SceneManager& manager, Game::e_StageKind stageKind) :
	SceneBase(manager)
{
	m_pPlayer = std::make_shared<Player>();
	m_pPlayerWeapon = std::make_shared<PlayerWeapon>();
	m_pCamera = std::make_shared<Camera>();

	m_pBossTutorial = std::make_shared<BossTutorial>();

	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();

	m_pHpBarUi = std::make_shared<HpBar>();

	m_pField = std::make_shared<Field>(stageKind);
	m_pSkyDome = std::make_shared<SkyDome>();

	m_pPhysics = std::make_shared<MyLib::Physics>(stageKind);

	m_isCameraLockOn = false;
	m_isToNextScene = false;
	m_effectFrame = 0;
	m_tutorialFrame = 0;
	m_gameOverTime = 0;

	m_pPlayer->Initialize(m_pPhysics, kPlayerPos, *m_pPlayerWeapon, kPlayerAngle);
	
	m_pBossTutorial->Initialize(m_pPhysics);

	m_pField->Initialize();

	m_pCamera->Initialize(m_pPlayer->GetPos(), kCameraAngleH);

	//タイトルのBgmが流れていたら止める用
	SoundManager::GetInstance().StopBgm("titleBgm");

	m_handles.push_back(LoadGraph("Data/Image/StageButton.png"));

	//チュートリアルの時に使うフラグ
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);

	m_fontH = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", kFontSize);

	/* 影の初期設定 */
	m_shadowMap = MakeShadowMap(kShadowMapSize, kShadowMapSize);
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMap, kShadowAreaMinPos, kShadowAreaMaxPos);
	// シャドウマップが想定するライトの方向をセット
	SetShadowMapLightDirection(m_shadowMap, kShadowDir);

	// 影色を調整
	SetLightAmbColor(GetColorF(0.8f, 0.8f, 0.8f, 0.0f));

}

SceneTutorial::~SceneTutorial()
{

	m_pPlayer->Finalize(m_pPhysics);

	//シャドウマップの削除
	DeleteShadowMap(m_shadowMap);
	m_shadowMap = -1;

	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}
	m_handles.clear();

	//フラグのクリア
	m_isTutorial.clear();

	SoundManager::GetInstance().StopBgm("stageClearBgm");

}

void SceneTutorial::Update()
{
#ifdef _DEBUG
	MyLib::DebugDraw::Draw();
#endif

#ifdef _DEBUG
	MyLib::DebugDraw::Clear();
#endif

	Pad::Update();
	UpdateFade();
	UpdateFadeSelectGraph();
	EffectManager::GetInstance().Update();

	SoundManager::GetInstance().PlayBgm("selectBgm", true);

	/*　チュートリアル　*/

	//1回だけチュートリアルの説明を入れるためのフラグ
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialStart])
	{
		m_tutorialFrame++;

		//ゲームを開始して2秒後にチュートリアルの説明を入れる
		if (m_tutorialFrame >= kTutorialFrame)
		{

			//もうチュートリアルを行わないよう
			m_isTutorial[Game::e_TutorialProgress::kTutorialStart] = true;
			m_tutorialFrame = 0;
			//セリフシーンに飛んで、解説する、Game.hにenumでチュートリアルを入れておく
			m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager,Game::e_TutorialProgress::kTutorialStart));
		}

	}
	
	//ジャンプする位置に行ったらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialJump])
	{

		if (m_pPlayer->GetPos().z <= kTutorialJumpPosZ)
		{
			m_isTutorial[Game::e_TutorialProgress::kTutorialJump] = true;
			m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialJump));
		}
	}

	//ジャンプして位置に行ったらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialJumpClear])
	{
		if (m_pPlayer->GetPos().z <= kTutorialJumpClearPosZ)
		{
			m_tutorialFrame++;

			if (m_tutorialFrame >= kTutorialFrame)
			{
				m_isTutorial[Game::e_TutorialProgress::kTutorialJumpClear] = true;
				m_tutorialFrame = 0;
				m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialJumpClear));
			}
		}
	}

	//歩いて位置まで行ったらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialDashJump])
	{
		if (m_pPlayer->GetPos().z <= kTutorialDashJumpPosZ)
		{
			m_isTutorial[Game::e_TutorialProgress::kTutorialDashJump] = true;
			m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialDashJump));
		}
	}
	
	//ダッシュジャンプで位置まで行ったらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialDashJumpClear])
	{
		if (m_pPlayer->GetPos().z <= kTutorialDashJumpClearPosZ)
		{
			m_tutorialFrame++;

			if (m_tutorialFrame >= kTutorialFrame)
			{
				m_isTutorial[Game::e_TutorialProgress::kTutorialDashJumpClear] = true;
				m_tutorialFrame = 0;
				m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialDashJumpClear));
			}
		}
	}

	//ボス戦する位置に行ったらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialBoss])
	{
		if (m_pPlayer->GetPos().z <= kTutorialBossBattlePosZ)
		{
			m_isTutorial[Game::e_TutorialProgress::kTutorialBoss] = true;
			m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialBoss));
		}
	}

	//ボスを倒したらチュートリアルの説明を入れる
	if (!m_isTutorial[Game::e_TutorialProgress::kTutorialBossClear])
	{
		if (m_pBossTutorial->GetHp() <= 0.0f)
		{
			m_tutorialFrame++;

			if (m_tutorialFrame >= kTutorialBossFrame)
			{
				m_isTutorial[Game::e_TutorialProgress::kTutorialBossClear] = true;
				m_pManager.PushScene(std::make_shared<SceneWords>(m_pManager, Game::e_TutorialProgress::kTutorialBossClear));
			}
		}
	}

	//ボスを倒したらステージセレクトに移動
	if (m_isTutorial[Game::e_TutorialProgress::kTutorialBossClear])
	{
		if (m_pPlayer->GetPos().z <= kTutorialSelectPosZ)
		{
			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				SoundManager::GetInstance().PlaySe("gamePlayTransSe");
				StartFadeOut();
				m_isToNextScene = true;
			}
		}
	}
	

#ifdef _DEBUG
	if (Pad::IsTrigger PAD_INPUT_7)
	{
		SoundManager::GetInstance().StopBgm("selectBgm");
		m_pManager.ChangeScene(std::make_shared<SceneDebug>(m_pManager));
		return;
	}

#endif

	if (!IsFading())
	{
		//ポーズシーンに遷移させる
		if (Pad::IsTrigger(PAD_INPUT_8))
		{
			SoundManager::GetInstance().PlaySe("pouseSe");
			m_pManager.PushScene(std::make_shared<ScenePause>(m_pManager));
		}
	}

	/*　キャラクターたちのアップデート処理　*/

	if (m_pBossTutorial->GetIsBattle())
	{
		if (!m_pBossTutorial->GetIsClear())
		{
			m_isCameraLockOn = true;
		}

		SoundManager::GetInstance().StopBgm("selectBgm");
		SoundManager::GetInstance().PlayBgm("battleBgm", true);
	}

	//プレイヤーのゲームオーバーフラグがtrueの場合
	if (m_pPlayer->GetIsGameOver())
	{
		m_isCameraLockOn = false;
		m_gameOverTime++;

		SoundManager::GetInstance().StopBgm("battleBgm");

		StartFadeOut();
		m_isToNextScene = true;
	}

	//ゲームオーバー時間が過ぎたら
	if (m_gameOverTime > kGameOverTimeMax)
	{
		m_pManager.ChangeScene(std::make_shared<SceneGameOver>(m_pManager, e_BossKind::kTutorial));
		return;
	}

	m_pPlayer->SetCameraDirection(m_pCamera->GetDirection());

	//カメラのアップデート処理
	m_pCamera->Update(m_pPlayer->GetPos(), m_pBossTutorial->GetPosUp(),
		m_pField->GetModelHandle(), m_pPlayer->GetAngle(), m_isCameraLockOn);

	//プレイヤーのアップデート処理
	m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(),
		m_pBossTutorial->GetPosDown(), m_pCamera->GetIsLockOn(), m_pBossTutorial->GetAttackKind());

	//パワーボスのアップデート処理
	m_pBossTutorial->Update(m_pPhysics, *m_pPlayer, m_pPlayer->GetAttackKind());

	//ボスのHPバーのアップデート処理
	m_pHpBarUi->Update(m_pBossTutorial->GetHp());

	//ボスの攻撃座標や半径を取得
	m_pPlayer->HitUpdate(m_pBossTutorial->GetHitPos(), m_pBossTutorial->GetAttackPos(),
		m_pBossTutorial->GetAttackPos(), m_pBossTutorial->GetShockPos(), m_pBossTutorial->GetHitRadius(),
		m_pBossTutorial->GetAttackRadius(), m_pBossTutorial->GetWeaponRadius(), m_pBossTutorial->GetShockRadius(),
		m_pBossTutorial->GetIsAttack());

	//プレイヤーの攻撃座標や半径を取得
	m_pBossTutorial->HitUpdate(m_pPlayer->GetAttackXPos(), m_pPlayer->GetAttackYPos(),
		m_pPlayer->GetShockPos(), m_pPlayer->GetAttackXRadius(), m_pPlayer->GetAttackYRadius(),
		m_pPlayer->GetShockRadius(), m_pPlayer->GetIsAttack());

	m_pSkyDome->Update();
	m_pPhysics->Update();

	/*フレームにアタッチするための更新処理*/
	//Physicsの後に入れておかないと補正の影響でワンテンポ遅れる
	m_pPlayerWeapon->SwordUpdate();

	m_pFaceUi->Update();
	m_pPlayerBarUi->Update(*m_pPlayer);

	if (m_pBossTutorial->GetIsClear())
	{
		SoundManager::GetInstance().StopBgm("battleBgm");
		SoundManager::GetInstance().PlayBgm("stageClearBgm", true);

		m_pBossTutorial->Finalize(m_pPhysics);
		m_isCameraLockOn = false;
	}

	//シーンフラグがたった場合
	if (m_isToNextScene)
	{
		if (!IsFadingOut())
		{
			if (m_pBossTutorial->GetIsClear())
			{
				m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kSelectPlayerPos));
				return;
			}
		}
	}
}

void SceneTutorial::Draw()
{
	m_pSkyDome->Draw();

	ShadowMap_DrawSetup(m_shadowMap);	//シャドウマップ描画開始

	m_pPlayer->Draw(*m_pPlayerWeapon);

	if (m_pBossTutorial->GetHp() != 0)
	{
		m_pBossTutorial->Draw();
	}

	ShadowMap_DrawEnd();	//シャドウマップ描画終了

	SetUseShadowMap(0, m_shadowMap);	// シャドウマップの反映開始

	m_pField->Draw();

	SetUseShadowMap(0, -1);	// シャドウマップの反映終了

	m_pPlayer->Draw(*m_pPlayerWeapon);
	m_pBossTutorial->Draw();

	EffectManager::GetInstance().Draw();
	m_pPlayerBarUi->Draw();
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);
	m_pButtonUi->Draw(*m_pPlayer);

	m_pCamera->Draw();

	if (!m_pBossTutorial->GetIsClear() && m_pBossTutorial->GetIsBattle())
	{
		m_pHpBarUi->Draw();
		m_pHpBarUi->DrawBossName(Game::e_BossKind::kTutorial);
	}

	if (m_pBossTutorial->GetIsClear() && m_pPlayer->GetPos().z <= kTutorialSelectPosZ)
	{
		DrawFadeSelectGraph(m_handles[kHitH], kHitPos);
	}

	if (!m_isFadeColor)
	{
		DrawFade(0x000000);
	}
	else
	{
		DrawFade(0xffffff);
	}


#ifdef _DEBUG
	DrawString(0, 0, "Scene Select", 0xffffff, false);

	DrawFormatString(0, 600, 0xffffff,"%d", m_tutorialFrame);
#endif
}

#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/SceneSelect.h"
#include "scene/ScenePause.h"
#include "scene/SceneOption.h"
#include "scene/SceneGamePlay.h"
#include "scene/SceneDebug.h"

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
		kPowerH,
		kSpeedH,
		kShotH,
		kRastH,
		kNoClearItemH,
		kPowerClearItemH,
		kSpeedClearItemH,
		kShotClearItemH,
		kRastNoClearItemH,
		kRastClearItemH,
	};

	//フォントのパス
	const char* kFontPath = "Data/Font/Dela-Gothic-One.ttf";

	//フォントサイズ	
	constexpr int kFontSize = 38;

	//エフェクトを出すフレーム数
	constexpr int kEffectFrame = 190;

	//初期位置
	constexpr VECTOR kInitPos = { 400.0f,-35.0f,740.0f };
	constexpr VECTOR kLookPos = { -100.0f,30.0f,500.0f };

	constexpr VECTOR kPowerTrianglePos = { 296.0f, 18.0f, 340.0f };
	constexpr VECTOR kSpeedTrianglePos = { 520.0f,18.0f,338.0f };
	constexpr VECTOR kShotTrianglePos = { 406.0f,18.0f,112.0f };


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
	constexpr int kShadowMapSize = 16384;								// ステージのシャドウマップサイズ
	const VECTOR kShadowAreaMinPos = { -10000.0f, -80.0f, -10000.0f };		// シャドウマップに描画する最小範囲
	const VECTOR kShadowAreaMaxPos = { 10000.0f, 100.0f, 10000.0f };	// シャドウマップに描画する最大範囲
	const VECTOR kShadowDir = { 0.0f, -5.0f, 0.0f };					// ライト方向

	constexpr float kShadowColor = 0.7f;
	constexpr float kShadowAlpha = 0.3f;
}


SceneSelect::SceneSelect(SceneManager& manager , Game::e_StageKind stageKind, VECTOR playerPos) :
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

	m_pField = std::make_shared<Field>(stageKind);
	m_pSkyDome = std::make_shared<SkyDome>();

	m_pItemHp = std::make_shared<ItemHp>();
	m_pItemMp = std::make_shared<ItemMp>();

	m_pTomb = std::make_shared<Tomb>();

	m_pPhysics = std::make_shared<MyLib::Physics>(stageKind);

	m_isToNextScene = false;
	m_effectFrame = 0;

	m_pTomb->Initialize(kPowerTrianglePos, kSpeedTrianglePos, kShotTrianglePos);

	m_pPlayer->Initialize(m_pPhysics, playerPos, *m_pPlayerWeapon);
	m_pItemHp->Initialize(m_pPhysics);
	m_pItemMp->Initialize(m_pPhysics);
	m_pField->Initialize();

	m_pCamera->Initialize(m_pPlayer->GetPos());

	m_isPowerStage = false;
	m_isSpeedStage = false;
	m_isShotStage = false;
	m_isRastStage = false;

	//タイトルのBgmが流れていたら止める用
	SoundManager::GetInstance().StopBgm("selectBgm");

	m_handles.push_back(LoadGraph("Data/Image/StageButton.png"));
	m_handles.push_back(LoadGraph("Data/Image/Power.png"));
	m_handles.push_back(LoadGraph("Data/Image/Speed.png"));
	m_handles.push_back(LoadGraph("Data/Image/Shot.png"));
	m_handles.push_back(LoadGraph("Data/Image/Rast.png"));
	m_handles.push_back(LoadGraph("Data/Image/NoClearItem.png"));
	m_handles.push_back(LoadGraph("Data/Image/PowerClearItem.png"));
	m_handles.push_back(LoadGraph("Data/Image/SpeedClearItem.png"));
	m_handles.push_back(LoadGraph("Data/Image/ShotClearItem.png"));
	m_handles.push_back(LoadGraph("Data/Image/NoClearStar.png"));
	m_handles.push_back(LoadGraph("Data/Image/ClearStar.png"));

	m_fontH = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", kFontSize);


	/* 影の初期設定 */
	m_shadowMap = MakeShadowMap(kShadowMapSize, kShadowMapSize);
	// シャドウマップに描画する範囲を設定
	SetShadowMapLightDirection(m_shadowMap, kShadowDir);
	// シャドウマップに描画する範囲を設定
	SetShadowMapDrawArea(m_shadowMap, kShadowAreaMinPos, kShadowAreaMaxPos);

	// 影色を調整
	SetLightAmbColor(GetColorF(kShadowColor, kShadowColor, kShadowColor, kShadowAlpha));
}

SceneSelect::~SceneSelect()
{
	m_pPlayer->Finalize(m_pPhysics);
	m_pItemHp->Finalize(m_pPhysics);
	m_pItemMp->Finalize(m_pPhysics);

	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}


	m_handles.clear();

	SoundManager::GetInstance().StopBgm("selectBgm");

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
	UpdateFadeSelectGraph();
	EffectManager::GetInstance().Update();

	SoundManager::GetInstance().PlayBgm("selectBgm", true);

	//ゲームステージに移動するための当たり判定
	m_isPowerStage = m_pTomb->TombPowerHit(m_pPlayer);
	m_isSpeedStage = m_pTomb->TombSpeedHit(m_pPlayer);
	m_isShotStage = m_pTomb->TombShotHit(m_pPlayer);
	m_isRastStage = m_pTomb->TombRastHit(m_pPlayer);


	if (m_effectFrame % kEffectFrame == 0)
	{
		EffectManager::GetInstance().CreateEffect("stagePower", m_pTomb->GetPowerPos());
		EffectManager::GetInstance().CreateEffect("stageSpeed", m_pTomb->GetSpeedPos());
		EffectManager::GetInstance().CreateEffect("stageShot", m_pTomb->GetShotPos());
		EffectManager::GetInstance().CreateEffect("stageRass", m_pTomb->GetRastPos());
	
		m_effectFrame = 0;
	}
	m_effectFrame++;


	if (m_isPowerStage)
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			SoundManager::GetInstance().PlaySe("gamePlayTransSe");
			StartFadeOut();
			m_isToNextScene = true;
			m_sceneTrans = e_SceneTrans::kPowerTypeBoss;
		}
	}

	if (m_isSpeedStage)
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			SoundManager::GetInstance().PlaySe("gamePlayTransSe");
			StartFadeOut();
			m_isToNextScene = true;
			m_sceneTrans = e_SceneTrans::kSpeedTypeBoss;
		}
	}

	if (m_isShotStage)
	{
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			SoundManager::GetInstance().PlaySe("gamePlayTransSe");
			StartFadeOut();
			m_isToNextScene = true;
			m_sceneTrans = e_SceneTrans::kShooterTypeBoss;
		}
	}

	if (m_isRastStage)
	{
		if (Pad::IsTrigger(PAD_INPUT_1) &&
			SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer) &&
			SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer) && 
			SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
		{
			SoundManager::GetInstance().PlaySe("gamePlayTransSe");
			StartFadeOut();
			m_isToNextScene = true;
			m_sceneTrans = e_SceneTrans::kRastTypeBoss;
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

	//ロックオンするのがいないため
	VECTOR noPos = VGet(0, 0, 0);

	m_pSkyDome->Update();
	m_pItemHp->Update(m_pPhysics);
	m_pItemMp->Update(m_pPhysics);
	m_pTomb->Update();

	m_pCamera->Update(m_pPlayer->GetPos(), m_pPlayer->GetPos(),m_pField->GetModelHandle(),m_pPlayer->GetAngle(), false);
	m_pPlayer->SetCameraDirection(m_pCamera->GetDirection());
	m_pPlayer->Update(m_pPhysics, *m_pPlayerWeapon, m_pCamera->GetCameraAngleX(), noPos, false, Game::e_BossAttackKind::kBossAttackNone);

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
			if (m_sceneTrans == e_SceneTrans::kPowerTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager , Game::e_BossKind::kPower, Game::e_StageKind::kGamePlay));
				return;
			}
			else if (m_sceneTrans == e_SceneTrans::kSpeedTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, Game::e_BossKind::kSpeed, Game::e_StageKind::kGamePlay));
				return;
			}
			else if (m_sceneTrans == e_SceneTrans::kShooterTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, Game::e_BossKind::kShot, Game::e_StageKind::kGamePlay));
				return;
			}
			else if (m_sceneTrans == e_SceneTrans::kRastTypeBoss)
			{
				m_pManager.ChangeScene(std::make_shared<SceneGamePlay>(m_pManager, Game::e_BossKind::kRast, Game::e_StageKind::kGamePlay));
				return;
			}
		}
	}

}

void SceneSelect::Draw()
{

	m_pSkyDome->Draw();

	ShadowMap_DrawSetup(m_shadowMap); //シャドウマップ描画開始

	//影を描画するための球体
	DrawSphere3D(VGet(m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + 5.0f, m_pPlayer->GetPos().z), 3.0f, 128, 0xffffff, 0xffffff, false);


	m_pTomb->DrawTriangleSelect();


	m_pItemHp->Draw();
	m_pItemMp->Draw();

	ShadowMap_DrawEnd(); //シャドウマップ描画終了


	SetUseShadowMap(0, m_shadowMap); // シャドウマップの反映開始

	m_pField->Draw();

	m_pTomb->DrawTriangleSelect();

	SetUseShadowMap(0, -1); // シャドウマップの反映終了

	m_pItemHp->Draw();
	m_pItemMp->Draw();

	m_pPlayer->Draw(*m_pPlayerWeapon);

	EffectManager::GetInstance().Draw();

	m_pTomb->Draw();

	m_pPlayerBarUi->Draw();
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);
	m_pButtonUi->Draw(*m_pPlayer);

	//ゲームシーンに行けるかどうかを分かりやすくするための画像
	if (m_isPowerStage) 
	{
		DrawFadeSelectGraph(m_handles[kHitH], kHitPos);
		DrawGraph(kHitBossPos.x, kHitBossPos.y, m_handles[kPowerH], true);

		DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kNoClearItemH], true);
		//クリアしていた場合
		if(SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer))
		{
			DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kPowerClearItemH], true);
		}
	}
	if (m_isSpeedStage)
	{
		DrawFadeSelectGraph(m_handles[kHitH], kHitPos);
		DrawGraph(kHitBossPos.x, kHitBossPos.y, m_handles[kSpeedH], true);

		DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kNoClearItemH], true);
		//クリアしていた場合
		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer))
		{
			DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kSpeedClearItemH], true);
		}
	}
	if (m_isShotStage)
	{
		DrawFadeSelectGraph(m_handles[kHitH], kHitPos);
		DrawGraph(kHitBossPos.x, kHitBossPos.y, m_handles[kShotH], true);

		DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kNoClearItemH], true);
		//クリアしていた場合
		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
		{
			DrawGraph(kHitItemPos.x, kHitItemPos.y, m_handles[kShotClearItemH], true);
		}
	}
	if (m_isRastStage)
	{
		DrawFadeSelectGraph(m_handles[kHitH], kHitPos);
		DrawGraph(kHitBossPos.x, kHitBossPos.y, m_handles[kRastH], true);

		DrawGraph(kHitItemPos.x - kHitAdjustmentPos.x, kHitItemPos.y - kHitAdjustmentPos.y, m_handles[kNoClearItemH], true);
		DrawGraph(kHitItemPos.x, kHitItemPos.y - kHitAdjustmentUpPos.y, m_handles[kNoClearItemH], true);
		DrawGraph(kHitItemPos.x + kHitAdjustmentPos.x, kHitItemPos.y - kHitAdjustmentPos.y, m_handles[kNoClearItemH], true);

		DrawGraph(kHitStarPos.x, kHitStarPos.y, m_handles[kRastNoClearItemH], true);

		//条件を満たしていた場合していた場合
		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer))
		{
			DrawGraph(kHitItemPos.x - kHitAdjustmentPos.x, kHitItemPos.y - kHitAdjustmentPos.y, m_handles[kPowerClearItemH], true);
		}

		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
		{
			DrawGraph(kHitItemPos.x, kHitItemPos.y - kHitAdjustmentUpPos.y, m_handles[kShotClearItemH], true);
		}

		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer))
		{
			DrawGraph(kHitItemPos.x + kHitAdjustmentPos.x, kHitItemPos.y - kHitAdjustmentPos.y, m_handles[kSpeedClearItemH], true);
		}

		if (SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kRassPlayer))
		{
			DrawGraph(kHitStarPos.x, kHitStarPos.y , m_handles[kRastClearItemH], true);
		}
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
	m_pCamera->Draw();
#endif

}

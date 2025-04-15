#include "DxLib.h"

#include "scene/SceneManager.h"
#include "scene/ScenePause.h"
#include "scene/SceneOption.h"
#include "scene/SceneSelect.h"
#include "scene/SceneDebug.h"
#include "scene/SceneTutorial.h"

#include "object/player/Player.h"
#include "object/player/PlayerWeapon.h"
#include "object/Camera.h"
#include "object/boss/BossShot.h"

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
	constexpr VECTOR kPlayerPos = { 350.0f,-35.0f,0 };
}

SceneTutorial::SceneTutorial(SceneManager& manager, Game::e_StageKind stageKind) :
	SceneBase(manager)
{
	m_pPlayer = std::make_shared<Player>();
	m_pPlayerWeapon = std::make_shared<PlayerWeapon>();
	m_pCamera = std::make_shared<Camera>();

	m_pPlayerBarUi = std::make_shared<PlayerBarUi>();
	m_pFaceUi = std::make_shared<FaceUi>();
	m_pFaceFrameUi = std::make_shared<FaceFrameUi>();
	m_pButtonUi = std::make_shared<ButtonUi>();

	m_pField = std::make_shared<Field>(stageKind);
	m_pSkyDome = std::make_shared<SkyDome>();


	m_pPhysics = std::make_shared<MyLib::Physics>(stageKind);

	m_isToNextScene = false;
	m_effectFrame = 0;


	m_pPlayer->Initialize(m_pPhysics, kPlayerPos, *m_pPlayerWeapon);
	m_pField->Initialize();

	m_pCamera->Initialize(m_pPlayer->GetPos());

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

	//チュートリアルの時に使うフラグ
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);
	m_isTutorial.push_back(false);

	m_fontH = Font::GetInstance().GetFontHandle(kFontPath, "Dela Gothic One", kFontSize);

}

SceneTutorial::~SceneTutorial()
{
	m_pPlayer->Finalize(m_pPhysics);

	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}
	m_handles.clear();

	//フラグのクリア
	m_isTutorial.clear();

	SoundManager::GetInstance().StopBgm("selectBgm");
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

	//ゲームステージに移動するための当たり判定
	//m_isPowerStage = m_pTomb->TombPowerHit(m_pPlayer);
	//m_isSpeedStage = m_pTomb->TombSpeedHit(m_pPlayer);
	//m_isShotStage = m_pTomb->TombShotHit(m_pPlayer);
	//m_isRastStage = m_pTomb->TombRastHit(m_pPlayer);

	//プレイヤーの距離がここを超えたら
	if (m_pPlayer->GetPos().z < -50.0f)
	{
		
	}
	
	if(m_pPlayer->GetPos().z == 100)
	{

	}

	if (m_pPlayer->GetPos().z == 100)
	{

	}

	if (m_pPlayer->GetPos().z == 100)
	{

	}

	if (m_effectFrame % kEffectFrame == 0)
	{
		//EffectManager::GetInstance().CreateEffect("stagePower", m_pTomb->GetPowerPos());
		//EffectManager::GetInstance().CreateEffect("stageSpeed", m_pTomb->GetSpeedPos());
		//EffectManager::GetInstance().CreateEffect("stageShot", m_pTomb->GetShotPos());
		//EffectManager::GetInstance().CreateEffect("stageRass", m_pTomb->GetRastPos());

		m_effectFrame = 0;
	}
	m_effectFrame++;


	//if (Pad::IsTrigger(PAD_INPUT_1))
	//{
	//	SoundManager::GetInstance().PlaySe("gamePlayTransSe");
	//	StartFadeOut();
	//	m_isToNextScene = true;
	//}

	

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
	//m_pTomb->Update();

	m_pCamera->Update(m_pPlayer->GetPos(), m_pPlayer->GetPos(), m_pField->GetModelHandle(), m_pPlayer->GetAngle(), false);
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
			m_pManager.ChangeScene(std::make_shared<SceneSelect>(m_pManager, Game::e_StageKind::kSelect, kPlayerPos));
			return;
		}
	}
}

void SceneTutorial::Draw()
{
	m_pField->Draw();
	m_pSkyDome->Draw();
	m_pPlayer->Draw(*m_pPlayerWeapon);

	EffectManager::GetInstance().Draw();

	//m_pTomb->Draw();
	//m_pTomb->DrawTriangleSelect();

	m_pPlayerBarUi->Draw();
	m_pFaceFrameUi->Draw(*m_pPlayer);
	m_pFaceUi->Draw(*m_pPlayer);
	m_pButtonUi->Draw(*m_pPlayer);

	// 反映終了
	//SetUseShadowMap(0, -1);

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

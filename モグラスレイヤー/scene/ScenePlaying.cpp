#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "DxLib.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "GimmickObj.h"

#include "Stage.h"
#include "SkyDome.h"

/// <summary>
/// コンストラクタ
/// </summary>
ScenePlaying::ScenePlaying() :
	m_isPlayerHit(false),
	m_isAttackHit(false)
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pGimmick = std::make_shared<GimmickObj>();
	m_pStage = std::make_shared<Stage>();
	m_pSkyDome = std::make_shared<SkyDome>();


	m_pPlayer->Load();
	m_pEnemy->Load();
	m_pStage->Load();

}

/// <summary>
/// デストラクタ
/// </summary>
ScenePlaying::~ScenePlaying()
{
}

/// <summary>
/// 初期化
/// </summary>
void ScenePlaying::Init()
{
	m_pCamera->Init();

	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pGimmick->Init();
	m_pStage->Init();
}

/// <summary>
/// アップデート
/// </summary>
std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	//カメラのアングルをセットする
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	m_pPlayer->Update();
	m_pCamera->PlayCameraUpdate(*m_pPlayer);
	//m_pCamera->TitleCameraUpdate();
	m_pEnemy->Update(*m_pPlayer);
	m_pGimmick->Update();

	m_pStage->Update();
	m_pSkyDome->Update(m_pPlayer->GetPos());

	//当たった場合のフラグの取得
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);
	m_isAttackHit;

	//ギミックに当たった場合のフラグ取得
	m_isGimmickHit = m_pGimmick->SphereHitFlag(m_pPlayer);

	VECTOR toEnemy = VSub(m_pEnemy->GetPos(),m_pPlayer->GetPos() );
	float length = VSize(toEnemy);

	//プレイヤーのhpを取得
	int playerHp = m_pPlayer->GetHp();

	if (m_isGimmickHit)
	{
		printfDx("d");
		m_pPlayer->OnGimmickHitUpdate();
	}

	//プレイヤーと敵が当たった場合
	if (m_isPlayerHit)
	{
		VECTOR posVec;
		VECTOR moveVec;


		//エネミーのベクトル座標からプレイヤーのベクトル座標を引いたベクトル
		posVec = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());

		//
		moveVec = VScale(posVec, length - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
		m_pPlayer->SetPos(VAdd(m_pPlayer->GetPos(), moveVec));


		playerHp -= 1;
		m_pPlayer->SetHp(playerHp);
		
	}

	//プレイヤーの攻撃と敵が当たった場合
	if (m_isAttackHit)
	{
		printfDx("当たった");
	}

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void ScenePlaying::Draw()
{

	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pGimmick->Draw();
	m_pStage->Draw();
	m_pSkyDome->Draw();

	DrawString(0, 0, "Scene Playing", 0xffffff, false);
}

/// <summary>
/// メモリの解放
/// </summary>
void ScenePlaying::End()
{
	m_pPlayer->Delete();
	m_pEnemy->Delete();
	m_pStage->Delete();
}

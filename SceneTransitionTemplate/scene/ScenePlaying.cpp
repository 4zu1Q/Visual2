#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "DxLib.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"

#include "Stage.h"
#include "SkyDome.h"
#include "Game.h"
#include "Pad.h"


namespace
{
	//フェード関連
	constexpr int kFadeTime = 120;
	constexpr int kBlend = 255;

	constexpr int kSelectLeft = Game::kScreenWidth * 0.3;
	constexpr int kSelectRight = Game::kScreenWidth * 0.4;

	constexpr int kLeft = Game::kScreenWidth * 0.45;
	constexpr int kRight = Game::kScreenWidth * 0.55;

	constexpr int kStartTop = 220;
	constexpr int kStartDown = 300;

	constexpr int kOptionTop = 320;
	constexpr int kOptionDown = 400;

	constexpr int kEndTop = 420;
	constexpr int kEndDown = 500;

	constexpr float kSelectSpeed = 0.05f;
	constexpr float kSelectAnimationSize = 9.0f;
}

/// <summary>
/// コンストラクタ
/// </summary>
ScenePlaying::ScenePlaying() :
	m_isInterval(false),
	m_isPlayerHit(false),
	m_isAttackHit(false),
	m_isMenu(false),
	m_isCommand(false),
	m_isTitle(false),
	m_isDamageCount(false),
	m_isHitCount(false),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_restartH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
	m_titleH(LoadGraph("data/image/Title.png")),
	m_select(kRestart),
	m_frameScene(0),
	m_frameHit(0),
	m_frameDamage(0)
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
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
	DeleteGraph(m_selectH);
	DeleteGraph(m_restartH);
	DeleteGraph(m_optionH);
	DeleteGraph(m_titleH);

}

/// <summary>
/// 初期化
/// </summary>
void ScenePlaying::Init()
{
	m_pCamera->Init();

	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pStage->Init();
}

/// <summary>
/// アップデート
/// </summary>
std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	Pad::Update();
	if (Pad::IsTrigger(PAD_INPUT_8)) m_isMenu = true;


	//カメラのアングルをセットする
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	//メニューを表示していない場合
	if (!m_isMenu)
	{
		m_pPlayer->Update();
		m_pEnemy->Update();
		m_pStage->Update();
		m_pSkyDome->Update(*m_pPlayer);
	}
	else //メニューを表示してる場合
	{

		if (!m_isCommand)
		{
			//上方向を押したとき
			if (Pad::IsTrigger(PAD_INPUT_UP))
			{

				if (m_select == kRestart)
				{
					m_select = kTitle;
				}
				else if (m_select == kOption)
				{
					m_select = kRestart;
				}
				else if (m_select == kTitle)
				{
					m_select = kOption;
				}
			}

			//下方向を押したとき
			if (Pad::IsTrigger(PAD_INPUT_DOWN))
			{
				if (m_select == kRestart)
				{
					m_select = kOption;
				}
				else if (m_select == kOption)
				{
					m_select = kTitle;
				}
				else if (m_select == kTitle)
				{
					m_select = kRestart;
				}
			}

			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				if (m_select == kRestart)
				{
					m_isMenu = false;
					m_isCommand = false;
				}
				else if (m_select == kOption)
				{
					m_isCommand = true;
				}
				else if (m_select == kTitle)
				{
					m_isInterval = true;
					m_isTitle = true;
				}
			}
		}

	}

	m_pCamera->PlayCameraUpdate(*m_pPlayer);



	//プレイヤーと敵が当たった場合のフラグの取得
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);

	//プレイヤーの攻撃に当たった場合のフラグ取得
	m_isAttackHit = m_pEnemy->AttackSphereHitFlag(m_pPlayer);

	//敵の攻撃に当たった場合のフラグ取得
	m_isDamageHit = m_pEnemy->DamageSphereHitFlag(m_pPlayer);

	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());
	float length = VSize(toEnemy);

	//プレイヤーのhpを取得
	int playerHp = m_pPlayer->GetHp();
	int enemyHp = m_pEnemy->GetHp();



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

	}

	//攻撃のクールタイム
	if (!m_isHitCount)
	{
		//プレイヤーの攻撃が敵に入った場合
		if (m_isAttackHit)
		{
			enemyHp -= 1;
			m_pEnemy->SetHp(enemyHp);
			m_isHitCount = true;
		}
	}
	else
	{
		m_frameHit++;

		if (m_frameHit >= 30)
		{
			m_isHitCount = false;
			m_frameHit = 0;
		}
	}

	//ダメージのクールタイム
	if (!m_isDamageCount)
	{
		//敵の攻撃を受けた場合
		if (m_isDamageHit)
		{
			playerHp -= 1;
			m_pPlayer->SetHp(playerHp);
			m_isDamageCount = true;
			m_pPlayer->SetDamage(true);
		}
	}
	else
	{
		m_frameDamage++;

		if (m_frameDamage >= 120)
		{
			m_isDamageCount = false;
			m_frameDamage = 0;
		}
	}

	//プレイヤーのHPがゼロになったら
	if (m_pPlayer->GetHp() <= 0)
	{
		m_isInterval = true;
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneLose>();
		}
	}

	//ボスのHPがゼロになったら
	if (m_pEnemy->GetHp() <= 0)
	{
		m_isInterval = true;
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneWin>();
		}
	}



	//メニューからタイトルへ
	if (m_isTitle)
	{
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneTitle>();
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;

	return shared_from_this();
}

/// <summary>
/// 描画
/// </summary>
void ScenePlaying::Draw()
{

	m_pSkyDome->Draw();
	m_pStage->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();

	if (m_isMenu)
	{
		// 半透明にしてメニュー背景の四角
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFillBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x000000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 不透明に戻して白い枠
		DrawLineBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x00ffff);


		//セレクト
		if (m_select == kRestart)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
		}
		else if (m_select == kOption)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
		}
		else if (m_select == kTitle)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
		}


		DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_restartH, true); //スタート
		DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//オプション
		DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_titleH, true);//ゲーム終了
	}

	//フェード暗幕
	if (m_isInterval)
	{
		int alpha = kBlend * m_frameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#ifdef _DEBUG
	DrawFormatString(700, 16, 0xffffff, "Select:%d", m_select);
	DrawString(0, 0, "Scene Playing", 0xffffff, false);
#endif
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


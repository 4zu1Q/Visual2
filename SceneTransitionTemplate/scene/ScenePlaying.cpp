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
#include "ItemBase.h"
#include "ItemHp.h"

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

	constexpr int kPlayerMaxHp = 10;

	//アイテムのファイル名
	const char* const kItemHpModelFilename = "data/model/item/Heart.mv1";
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";

	//SEのファイル名
	const char* const kSelectFilename = "data/sound/se/SelectSe.mp3";
	const char* const kDecisionFilename = "data/sound/se/DecisionSe.mp3";
	const char* const kCancelFilename = "data/sound/se/CancelSe.mp3";

	const char* const kADamageFilename = "data/sound/se/EnemyADamageSe.mp3";
	const char* const kSDamageFilename = "data/sound/se/EenemySDamageSe.mp3";
	const char* const kPlayerDamageFilename = "data/sound/se/PlayerDamageSe.mp3";
	const char* const kItemFilename = "data/sound/se/HealSe.mp3";


	//BGMのファイル名
	const char* const kBgmFilename = "data/sound/bgm/GamePlayBgm.mp3";

	constexpr float kWall = 475;


	constexpr int kSceneTime = 600;
}

/// <summary>
/// コンストラクタ
/// </summary>
ScenePlaying::ScenePlaying() :
	m_isInterval(false),
	m_isPlayerHit(false),
	m_isPlayerAttackHit(false),
	m_isMenu(false),
	m_isCommand(false),
	m_isTitle(false),
	m_isEnemyAttackHitCount(false),
	m_isEnemySkillHitCount(false),
	m_isPlayerAttackHitCount(false),
	m_isPlayerSkillHitCount(false),
	m_isPlayerSkillHit(false),
	m_isItemHit(false),
	m_isEnemySearch(false),
	m_isEnemyStop(false),
	m_isEnemyAttackHit(false),
	m_isEnemySkillHit(false),
	m_isOption(false),
	m_isLose(false),
	m_isWin(false),
	m_isEnemyDeath(false),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_restartH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
	m_titleH(LoadGraph("data/image/Title.png")),
	m_operatorH(LoadGraph("data/image/Operator.png")),
	m_select(kRestart),
	m_frameScene(0),
	m_playerAttackHitFrame(0),
	m_playerSkillHitFrame(0),
	m_enemyAttackHitFrame(0),
	m_enemySkillHitFrame(0),
	m_fadeFrameScene(0),
	m_soundBgmH(-1),
	m_soundCancelH(-1),
	m_soundDecsionH(-1),
	m_soundSelectH(-1),
	m_soundPlayerDamageH(-1),
	m_soundADamageH(-1),
	m_soundSDamageH(-1)
{

	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pStage = std::make_shared<Stage>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pItem.resize(20);

	CreateItemHp(VGet(460, 5, 460));
	CreateItemHp(VGet(-460, 5, 460));
	CreateItemHp(VGet(460, 5, -460));
	CreateItemHp(VGet(-460, 5, -460));

	CreateItemHp(VGet(460, 5, 0));
	CreateItemHp(VGet(0, 5, 460));
	CreateItemHp(VGet(-460, 5, 0));
	CreateItemHp(VGet(0, 5, -460));

	CreateItemHp(VGet(230, 5, 0));
	CreateItemHp(VGet(0, 5, -230));
	CreateItemHp(VGet(-230, 5, 0));
	CreateItemHp(VGet(0, 5, 230));


	CreateItemHp(VGet(230, 5, 230));
	CreateItemHp(VGet(-230, 5, 230));
	CreateItemHp(VGet(230, 5, -230));
	CreateItemHp(VGet(-230, 5, -230));

	m_soundBgmH = LoadSoundMem(kBgmFilename);	  //BGM

	m_soundSelectH = LoadSoundMem(kSelectFilename);	  //選択音
	m_soundDecsionH = LoadSoundMem(kDecisionFilename);	  //決定音
	m_soundCancelH = LoadSoundMem(kCancelFilename);	  //キャンセル音
	m_soundPlayerDamageH = LoadSoundMem(kPlayerDamageFilename);		//プレイヤーダメージ音
	m_soundADamageH = LoadSoundMem(kADamageFilename);		//敵ダメージ音
	m_soundSDamageH = LoadSoundMem(kSDamageFilename);		//敵ダメージ音
	m_soundItemH = LoadSoundMem(kItemFilename);		//アイテム音

	ChangeVolumeSoundMem(64, m_soundBgmH);
	ChangeVolumeSoundMem(128, m_soundSelectH);
	ChangeVolumeSoundMem(128, m_soundDecsionH);
	ChangeVolumeSoundMem(128, m_soundCancelH);
	ChangeVolumeSoundMem(128, m_soundPlayerDamageH);
	ChangeVolumeSoundMem(128, m_soundADamageH);
	ChangeVolumeSoundMem(128, m_soundSDamageH);
	ChangeVolumeSoundMem(128, m_soundItemH);



	m_pPlayer->Load();
	m_pEnemy->Load();
	m_pStage->Load();

	PlaySoundMem(m_soundBgmH, DX_PLAYTYPE_LOOP);

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

	DeleteSoundMem(m_soundSelectH);
	DeleteSoundMem(m_soundDecsionH);
	DeleteSoundMem(m_soundCancelH);
	DeleteSoundMem(m_soundBgmH);
	DeleteSoundMem(m_soundPlayerDamageH);
	DeleteSoundMem(m_soundADamageH);
	DeleteSoundMem(m_soundSDamageH);
	DeleteSoundMem(m_soundItemH);

	MV1DeleteModel(m_modelHeartH);
	m_modelHeartH = -1;

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

	//プレイヤーのhpを取得
	int playerHp = m_pPlayer->GetHp();
	int enemyHp = m_pEnemy->GetHp();


	//カメラのアングルをセットする
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	//メニューを表示していない場合
	if (!m_isMenu)
	{
		ChangeVolumeSoundMem(64, m_soundBgmH);

		m_pPlayer->Update();
		m_pEnemy->Update(m_pPlayer);
		m_pStage->Update();
		m_pSkyDome->Update(*m_pPlayer);

		for (int i = 0; i < m_pItem.size(); i++)
		{
			if (m_pItem[i])
			{
				m_pItem[i]->Update();
			}
		}

	}
	else //メニューを表示してる場合
	{

		if (!m_isCommand)
		{
			ChangeVolumeSoundMem(32, m_soundBgmH);


			//上方向を押したとき
			if (Pad::IsTrigger(PAD_INPUT_UP))
			{

				if (m_select == kRestart)
				{
					m_select = kTitle;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
				else if (m_select == kOption)
				{
					m_select = kRestart;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
				else if (m_select == kTitle)
				{
					m_select = kOption;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
			}

			//下方向を押したとき
			if (Pad::IsTrigger(PAD_INPUT_DOWN))
			{
				if (m_select == kRestart)
				{
					m_select = kOption;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
				else if (m_select == kOption)
				{
					m_select = kTitle;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
				else if (m_select == kTitle)
				{
					m_select = kRestart;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//選択音
				}
			}

			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				if (m_select == kRestart)
				{
					m_isMenu = false;
					m_isCommand = false;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//決定音
				}
				else if (m_select == kOption)
				{
					m_isCommand = true;
					m_isOption = true;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//決定音
				}
				else if (m_select == kTitle)
				{
					m_isInterval = true;
					m_isTitle = true;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//決定音
					StopSoundMem(m_soundBgmH);
				}
			}

		}

		if (m_isOption)
		{
			if (Pad::IsTrigger PAD_INPUT_2)
			{
				m_isOption = false;
				m_isCommand = false;
			}
		}

	}

	m_pCamera->PlayCameraUpdate(*m_pPlayer);
	//m_pCamera->TargetCameraUpadate(*m_pPlayer, *m_pEnemy);


	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());
	float length = VSize(toEnemy);

	VECTOR posVec = VGet(0, 0, 0);
	VECTOR moveVec = VGet(0, 0, 0);

	//プレイヤーと敵が当たった場合のフラグの取得
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);

	//敵の攻撃に当たった場合のフラグ取得
	m_isEnemyAttackHit = m_pEnemy->EnemyAttackSphereHitFlag(m_pPlayer);

	//敵のスキルに当たった場合のフラグ取得
	m_isEnemySkillHit = m_pEnemy->EnemySkillSphereHitFlag(m_pPlayer);

	//敵が死んだら当たり判定を消す
	if (!m_isEnemyDeath)
	{
		//プレイヤーの攻撃に当たった場合のフラグ取得
		m_isPlayerAttackHit = m_pEnemy->PlayerAttackSphereHitFlag(m_pPlayer);

		//プレイヤーのスキルに当たった場合のフラグ取得
		m_isPlayerSkillHit = m_pEnemy->PlayerSkillSphereHitFlag(m_pPlayer);

		//敵の索敵範囲に入った場合のフラグ取得
		m_isEnemySearch = m_pEnemy->SearchSphereFlag(m_pPlayer);

		//敵の止まる範囲に入った場合のフラグ取得
		m_isEnemyStop = m_pEnemy->StopSphereFlag(m_pPlayer);

		//敵の行動の当たり判定
		//敵の索敵にプレイヤーがいなかったら
		if (!m_isEnemySearch && !m_isEnemyStop)
		{
			m_pEnemy->SetState(Enemy::kIdle);
		}
		//敵の索敵にプレイヤーがいたら
		if (m_isEnemySearch && !m_isEnemyStop)
		{
			m_pEnemy->SetState(Enemy::kRun);
		}
		//敵の攻撃する範囲にプレイヤーがいたら
		if (m_isEnemyStop)
		{
			m_pEnemy->SetState(Enemy::kAttack);
		}

		//プレイヤーと敵が当たった場合
		if (m_isPlayerHit)
		{
			Knockback(posVec, moveVec, length);
		}
	}
	else
	{
		m_pEnemy->SetState(Enemy::kDeath);
	}


	//アイテムとプレイヤーの当たり判定
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (m_pItem[i])
		{
			//アイテムに当たったら
			if (m_isItemHit = m_pItem[i]->ItemSphereFlag(m_pPlayer))
			{
				if (m_pPlayer->GetHp() == kPlayerMaxHp)
				{
					//何もしない
				}
				else
				{

					PlaySoundMem(m_soundItemH, DX_PLAYTYPE_BACK, true);//アイテム音

					playerHp += 1;
					m_pPlayer->SetHp(playerHp);
					m_pItem[i]->ItemLost();

					delete m_pItem[i];
					m_pItem[i] = nullptr;
				}
			}
		}
	}

	if (!m_isEnemyDeath)
	{
		//プレイヤー攻撃した時だけ発生する
		if (m_pPlayer->GetAttackGeneration())
		{
			//攻撃のクールタイム
			if (!m_isPlayerAttackHitCount)
			{
				//プレイヤーの攻撃が敵に入った場合
				if (m_isPlayerAttackHit)
				{
					PlaySoundMem(m_soundADamageH, DX_PLAYTYPE_BACK, true);//ダメージ音

					//enemyHp -= 100;
					enemyHp -= 10;
					m_pEnemy->SetHp(enemyHp);
					m_isPlayerAttackHitCount = true;
					m_pEnemy->SetDamage(true);

				}

			}
			else
			{
				m_playerAttackHitFrame++;

				if (m_playerAttackHitFrame >= 40)
				{
					m_isPlayerAttackHitCount = false;
					m_playerAttackHitFrame = 0;
				}
			}
		}

		//プレイヤースキルを使用した時のみ発生する
		if (m_pPlayer->GetSkillGeneration())
		{
			//スキルのクールタイム
			if (!m_isPlayerSkillHitCount)
			{

				//プレイヤーのスキルが敵に入った場合
				if (m_isPlayerSkillHit)
				{
					PlaySoundMem(m_soundSDamageH, DX_PLAYTYPE_BACK, true);//ダメージ音
					enemyHp -= 50;
					m_pEnemy->SetHp(enemyHp);
					m_isPlayerSkillHitCount = true;
					m_pEnemy->SetDamage(true);
				}

			}
			else
			{
				m_playerSkillHitFrame++;

				if (m_playerSkillHitFrame >= 50)
				{
					m_isPlayerSkillHitCount = false;
					m_playerSkillHitFrame = 0;
				}
			}
		}
	}

	

	if (m_pEnemy->GetAttackGeneration() && !m_pEnemy->GetSkillGeneration())
	{
		//ダメージのクールタイム
		if (!m_isEnemyAttackHitCount)
		{
			//敵の攻撃を受けた場合
			if (m_isEnemyAttackHit)
			{
				PlaySoundMem(m_soundPlayerDamageH, DX_PLAYTYPE_BACK, true);//ダメージ音
				playerHp -= 1;
				m_pPlayer->SetHp(playerHp);
				m_isEnemyAttackHitCount = true;
				m_pPlayer->SetDamage(true);
				m_pPlayer->SetAnimDamage(true);

			}
		}
		else
		{
			m_enemyAttackHitFrame++;
			if (m_enemyAttackHitFrame >= 120)
			{
				//m_pEnemy->SetAttackAnim(false);
				m_isEnemyAttackHitCount = false;
				m_enemyAttackHitFrame = 0;
				m_pEnemy->SetAttackGeneration(false);
			}
		}
	}




	if (m_pEnemy->GetSkillGeneration() && !m_pEnemy->GetAttackGeneration())
	{
		//ダメージのクールタイム
		if (!m_isEnemySkillHitCount)
		{
			//敵の攻撃を受けた場合
			if (m_isEnemySkillHit)
			{

				PlaySoundMem(m_soundPlayerDamageH, DX_PLAYTYPE_BACK, true);//ダメージ音
				playerHp -= 2;
				m_pPlayer->SetHp(playerHp);
				m_isEnemySkillHitCount = true;
				m_pPlayer->SetDamage(true);
				m_pPlayer->SetAnimDamage(true);
			}
		}
		else
		{
			m_enemySkillHitFrame++;

			if (m_enemySkillHitFrame >= 120)
			{
				//m_pEnemy->SetSkillAnim(false);
				m_isEnemySkillHitCount = false;
				m_enemySkillHitFrame = 0;
				m_pEnemy->SetSkillGeneration(false);
			}
		}
	}



	






	//プレイヤーのHPがゼロになったら
	if (m_pPlayer->GetHp() <= 0)
	{
		m_isLose = true;
		m_frameScene++;


		if (m_frameScene >= kSceneTime - 120)
		{
			m_fadeFrameScene++;
			m_isInterval = true;
		}
		if (m_frameScene >= kSceneTime)
		{
			return std::make_shared<SceneLose>();
		}
	}
	else
	{
		m_isLose = false;
	}

	//ボスのHPがゼロになったら
	if (m_pEnemy->GetHp() <= 0)
	{
		m_isEnemyDeath = true;
		m_isWin = true;
		m_frameScene++;

		if (m_frameScene >= 240)
		{
			m_pPlayer->SetCommand(true);

		}

		if (m_frameScene >= kSceneTime - 120)
		{
			m_fadeFrameScene++;
			m_isInterval = true;
		}

		if (m_frameScene >= kSceneTime)
		{
			return std::make_shared<SceneWin>();
		}
	}
	else
	{
		m_isWin = false;
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
	m_pEnemy->Draw();
	m_pPlayer->Draw();
	
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (m_pItem[i])
		{
			m_pItem[i]->Draw();
		}
	}

	if (m_isMenu)
	{
		// 半透明にしてメニュー背景の四角
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFillBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x000000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// 不透明に戻して白い枠
		DrawLineBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x00ffff);

		if (!m_isOption)
		{
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
		else
		{
			DrawExtendGraph(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, m_operatorH, true);
		}

	}

	//フェード暗幕
	if (m_isInterval)
	{
		int alpha = kBlend * m_fadeFrameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//if (m_isInterval)
	//{
	//	int alpha2 = kBlend * m_frameScene / kFadeTime;
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	//	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha2);
	//}

#ifdef _DEBUG
	DrawFormatString(700, 16, 0xffffff, "Select:%d", m_select);

	DrawFormatString(700, 96, 0x0000ff, "EnemyAtk:%d", m_enemyAttackHitFrame);
	DrawFormatString(700, 128, 0x0000ff, "EnemySkl:%d", m_enemySkillHitFrame);


	DrawFormatString(700, 160, 0x0000ff, "EnemyAtkGen:%d", m_pEnemy->GetAttackGeneration());
	DrawFormatString(700, 196, 0x0000ff, "EnemySklGen:%d", m_pEnemy->GetSkillGeneration());
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

void ScenePlaying::CreateItemHp(VECTOR pos)
{
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (!m_pItem[i])
		{
			m_pItem[i] = new ItemHp;
			m_pItem[i]->Start(pos);
			return;
		}
	}
}

void ScenePlaying::Knockback(VECTOR pos, VECTOR move, float length)
{
	//エネミーのベクトル座標からプレイヤーのベクトル座標を引いたベクトル
	pos = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());

	//
	move = VScale(pos, length - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
	m_pPlayer->SetPos(VAdd(m_pPlayer->GetPos(), move));
}


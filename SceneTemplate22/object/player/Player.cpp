#include "Player.h"
#include "PlayerWeapon.h"

#include "util/Pad.h"
#include "util/AnimController.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"
#include "util/SaveDataManager.h"

#include "myLib/MyLib.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kNormalModelFilename = "Data/Model/Player/Player.mv1";
	const char* const kPowerModelFilename = "Data/Model/Player/Barbarian.mv1";
	const char* const kSpeedModelFilename = "Data/Model/Player/RogueHooded.mv1";
	const char* const kShotModelFilename = "Data/Model/Player/Mage.mv1";
	const char* const kRassModelFilename = "Data/Model/Player/Knight.mv1";

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;
	constexpr float kWeaponScale = 1.0f;

	//プレイヤーの手のフレーム番号
	constexpr int kRightModelFrameNo = 14;
	constexpr int kLeftModelFrameNo = 9;

	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.0f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.9f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	constexpr float kSpeedConstant = 1.4f;

	//最小移動速度
	constexpr float kNormalMinSpeed = 0.5f * kSpeedConstant;
	constexpr float kPowerMinSpeed = 0.3f * kSpeedConstant;
	constexpr float kSpeedMinSpeed = 0.7f * kSpeedConstant;
	constexpr float kShotMinSpeed = 0.4f * kSpeedConstant;

	//最大移動速度
	constexpr float kNormalMaxSpeed = 0.7f * kSpeedConstant;
	constexpr float kPowerMaxSpeed = 0.5f * kSpeedConstant;
	constexpr float kSpeedMaxSpeed = 0.9f * kSpeedConstant;
	constexpr float kShotMaxSpeed = 0.6f * kSpeedConstant;

	//ダッシュスピード
	constexpr float kNormalDashSpeed = 1.0f * kSpeedConstant;
	constexpr float kPowerDashSpeed = 0.7f * kSpeedConstant;
	constexpr float kSpeedDashSpeed = 1.5f * kSpeedConstant;
	constexpr float kShotDashSpeed = 1.1f * kSpeedConstant;
	constexpr float kStrongestDashSpeed = 1.2f * kSpeedConstant;

	//ジャンプ力最小値
	constexpr float kMinJumpPower = 0.22f;
	//ジャンプ力最大値
	constexpr float kMaxJumpPower = 0.85f;
	//落ちるときの重力
	constexpr float kGravityPower = 0.005f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };
	//上の座標
	constexpr VECTOR kUpPos = { 0.0f,12.0f,0.0f };

	//プレイヤーのヒット座標
	constexpr float kUpHitPos = 9.0f;
	constexpr float kUpAttackPos = 6.0f;
	constexpr float kDownAttackPos = 5.0f;

	//見えない壁を作るための座標
	constexpr VECTOR kWallRightPos = { 510.0f,0.0f,790.0f };
	constexpr VECTOR kWallLeftPos = { -685.0f,0.0f,-600.0f };

	/*　プレイヤーのアニメーションの種類　*/

	//Csvファイルパス名
	const char* const kNormalAnimInfoFilename = "Data/Csv/AnimPlayerNormal.csv";
	const char* const kPowerAnimInfoFilename = "Data/Csv/AnimPlayerPower.csv";
	const char* const kSpeedAnimInfoFilename = "Data/Csv/AnimPlayerSpeed.csv";
	const char* const kShotAnimInfoFilename = "Data/Csv/AnimPlayerShot.csv";
	const char* const kRassAnimInfoFilename = "Data/Csv/AnimPlayerRass.csv";

	//アニメーションの名前
	const char* const kAnimIdle = "Idle";						//アイドル状態
	const char* const kAnimWalk = "Walk";						//歩き状態
	const char* const kAnimLockOnWalk = "LockOnWalk";			//ロックオン歩き状態
	const char* const kAnimDash = "Dash";						//ダッシュ状態
	const char* const kAnimAttackCharge = "AttackCharge";		//チャージ状態
	const char* const kAnimAttackX_First = "AttackX_First";		//攻撃の1撃目
	const char* const kAnimAttackX_Second = "AttackX_Second";	//攻撃の2撃目
	const char* const kAnimAttackX_Third = "AttackX_Third";		//攻撃の3撃目
	const char* const kAnimAttackX_Fourth = "AttackX_Fourth";	//攻撃の4撃目
	const char* const kAnimAttackY = "AttackY";					//特殊攻撃状態
	const char* const kAnimJump = "Jump";						//ジャンプ状態
	const char* const kAnimFall = "Jumping";					//空中状態
	const char* const kAnimHit = "Hit";							//ヒット状態
	const char* const kAnimDead = "Down";						//死亡状態
	const char* const kAnimUseFace = "UseFace";					//カオの使用状態

	//落ちる速度の倍率
	constexpr float kMinFallScale = 1.5f;
	constexpr float kMaxFallScale = 4.0f;

	//パッドのボタン情報
	constexpr int kPadButtonA = 0x00000010;
	constexpr int kPadButtonB = 0x00000020;
	constexpr int kPadButtonX = 0x00000040;
	constexpr int kPadButtonY = 0x00000080;

	constexpr int kPadButtonRB = 0x00000200;
	constexpr int kPadButtonLB = 0x00000100;

	constexpr int kPadButtonRStick = 0x00002000;
	constexpr int kPadButtonLStick = 0x00001000;

	//ライトトリガーが反応する値
	constexpr int kRTButtonNum = -1000;

	//最大値
	constexpr float kMaxHp = 10.0f;
	constexpr float kMaxMp = 300.0f;
	constexpr float kMaxStamina = 300.0f;

	//スタミナの減るスピード
	constexpr float kStaminaDiminishSpeed = 0.5f;
	constexpr float kStaminaIncreaseSpeed = 1.0f;
	constexpr float kStaminaActionIncreaseSpeed = 0.8f;

	//MPの減る値
	constexpr float kMpDiminishNum = 30.0f;
	constexpr float kMpIncreaseNum = 40.0f;

	constexpr float kShadowSize = 5.0f;
	constexpr float kShadowHeight = 50.0f;

	constexpr float kInitRadius = 6.0f;
	constexpr float kHitRadius = 3.0f;

	constexpr int kJumpCountNum = 6;
	constexpr int kJumpCountMaxNum = 30;
	constexpr float kJumpPowerNum = 4.0f;


	constexpr int kChargeTimeMax = 60;
	constexpr int kDeadTime = 30;
	constexpr int kDamageFrame = 120;

	constexpr int kAttackFrame = 30;
	constexpr int kAttackXFrame = 20;
	//constexpr int kChargeTimeMax = 60;

	//プレイヤーの種類によって変わる当たり判定の半径
	constexpr float kNormalAttackXRadius = 6.0f;
	constexpr float kNormalAttackYRadius = 15.0f;
	constexpr float kNormalAttackShockRadius = 10.0f;
	constexpr float kNormalAttackXMoveScale = 7.0f;
	constexpr float kNormalAttackYMoveScale = 1.0f;

	constexpr float kPowerAttackXRadius = 6.0f;
	constexpr float kPowerAttackYRadius = 15.0f;
	constexpr float kPowerAttackShockRadius = 10.0f;
	constexpr float kPowerAttackXMoveScale = 8.0f;
	constexpr float kPowerAttackYMoveScale = 1.0f;

	constexpr float kSpeedAttackXRadius = 3.5f;
	constexpr float kSpeedAttackYRadius = 6.5f;
	constexpr float kSpeedAttackShockRadius = 3.0f;
	constexpr float kSpeedAttackXMoveScale = 7.0f;
	constexpr float kSpeedAttackYMoveScale = 9.0f;

	constexpr float kShotAttackXRadius = 12.0f;
	constexpr float kShotAttackYRadius = 16.0f;
	constexpr float kShotAttackShockRadius = 10.0f;
	constexpr float kShotAttackXMoveScale = 20.0f;
	constexpr float kShotAttackYMoveScale = 50.0f;

	constexpr float kRassAttackXRadius = 9.0f;
	constexpr float kRassAttackYRadius = 15.0f;
	constexpr float kRassAttackShockRadius = 3.0f;
	constexpr float kRassAttackXMoveScale = 9.0f;
	constexpr float kRassAttackYMoveScale = 1.0f;

	constexpr int kDamageSmashNum = 8;
	constexpr int kDrawHiddenNum = 4;

	constexpr int kMoveCountNum = 15;
	constexpr int kWalkCountNum = 25;

}

Player::Player() :
	CharaBase(Collidable::e_Priority::kHigh, Game::e_GameObjectTag::kPlayer, MyLib::ColliderData::e_Kind::kSphere, false),
	m_isGameOver(false),
	m_pos(VGet(0,0,0)),
	m_hitPos(VGet(0,0,0)),
	m_bossPos(VGet(0, 0, 0)),
	m_bossAttackPos(VGet(0, 0, 0)),
	m_bossWeaponPos(VGet(0, 0, 0)),
	m_bossShockPos(VGet(0, 0, 0)),
	m_bossToPlayerVec(VGet(0, 0, 0)),
	m_radius(2),
	m_posUp(kInitPos),
	m_move(VGet(0, 0, 0)),
	m_attackMove(VGet(0, 0, 0)),
	m_attackXPos(VGet(0, 0, 0)),
	m_attackYPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_cameraDirection(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(kMaxHp),
	m_mp(kMaxMp),
	m_stamina(kMaxStamina),
	m_cameraAngle(0.0f),
	m_isDead(false),
	m_isDash(false),
	m_isStamina(false),
	m_isMp(false),
	m_isUseMp(false),
	m_multiAttack(0),
	m_isNextAttackFlag(false),
	m_isLockOn(false),
	m_chargeTime(0),
	m_jumpCount(0),
	m_jumpPower(0.0f),
	m_bossAttackRadius(0.0f),
	m_bossWeaponRadius(0.0f),
	m_bossShockRadius(0.0f),
	m_moveCount(0),
	m_deadTime(0),
	m_damageFrame(0),
	m_attackFrame(0)
{
	//攻撃の種類を
	m_attackKind = Game::e_PlayerAttackKind::kPlayerAttackNone;
	m_bossAttackKind = Game::e_BossAttackKind::kBossAttackNone;

	//モデルのロード
	m_modelH = MV1LoadModel(kNormalModelFilename);
	m_modelPowerH = MV1LoadModel(kPowerModelFilename);
	m_modelSpeedH = MV1LoadModel(kSpeedModelFilename);
	m_modelShotH = MV1LoadModel(kShotModelFilename);
	m_modelRassH = MV1LoadModel(kRassModelFilename);

	//アニメーション
	m_pAnim = std::make_shared<AnimController>();

	m_isAttack = false;
	m_isBossAttack = false;
	m_isHitDamage = false;

	//マスク関連の初期化
	m_isFaceUse = false;
	m_playerKind = e_PlayerKind::kPowerPlayer;
	
	//ボタンUI関連
	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;

	//コライダーデータの生成
	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);
	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = kInitRadius;
	m_radius = circleColliderData->m_radius;

	m_hitRadius = kHitRadius;
	m_attackXRadius = 0.0f;
	m_attackYRadius = 0.0f;
	m_attackShockRadius = 0.0f;
}

Player::~Player()
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	MV1DeleteModel(m_modelPowerH);
	MV1DeleteModel(m_modelSpeedH);
	MV1DeleteModel(m_modelShotH);
	MV1DeleteModel(m_modelRassH);

	m_modelH = -1;
	m_modelPowerH = -1;
	m_modelSpeedH = -1;
	m_modelShotH = -1;
	m_modelRassH = -1;
}

void Player::Initialize(std::shared_ptr<MyLib::Physics> physics, VECTOR pos, PlayerWeapon& weapon)
{
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(pos);

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_modelPowerH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_modelSpeedH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_modelShotH, VGet(kModelScale, kModelScale, kModelScale));
	MV1SetScale(m_modelRassH, VGet(kModelScale, kModelScale, kModelScale));

	//武器の初期化
	weapon.Load();
	weapon.Initialize(m_modelH, kRightModelFrameNo, kLeftModelFrameNo);

	//アニメーションの初期化
	m_pAnim->Initialize(kNormalAnimInfoFilename, m_modelH, kAnimIdle);
	m_angle = 48.0f;

	// メンバ関数ポインタの初期化
	m_updateFunc = &Player::IdleUpdate;

}

void Player::Finalize(std::shared_ptr<MyLib::Physics> physics )
{
	Collidable::Finalize(physics);
}

void Player::Update(std::shared_ptr<MyLib::Physics> physics, PlayerWeapon& weapon, float cameraAngleX, VECTOR bossPos, bool isLockOn, Game::e_BossAttackKind bossAttackKind)
{
	//アップデート
	(this->*m_updateFunc)();

	//RTのインプットを取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &m_input);

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();
	m_bossAttackKind = bossAttackKind;

	//プレイヤーのステータス更新
	StatusUpdate();
	//ダメージの更新処理
	DamageUpdate();
	//プレイヤーの当たり判定座標の更新
	CollisionPosUpdate();

	//ボスの座標を代入
	m_isLockOn = isLockOn;
	m_bossPos = bossPos;
	m_bossToPlayerVec = VSub(m_pos, m_bossPos);

	//プレイヤーモデルの回転処理
	PlayerSetPosAndRotation(m_pos, m_angle);
}

void Player::Draw(PlayerWeapon& weapon)
{

#ifdef _DEBUG

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackXPos.x, m_attackXPos.y, m_attackXPos.z);
	DrawFormatString(0, 148, 0xff0fff, "playerHp:%f,playerMp:%f,playerStamina:%f", m_hp, m_mp, m_stamina);
	DrawFormatString(0, 348, 0xffffff, "playerAngle:%f", m_angle);

	DrawFormatString(0, 500, 0xfff00f, "playerIsJump:%d", GetIsJump());

	DrawSphere3D(m_hitPos, m_hitRadius, 16, 0xffffff, 0xffffff, false);

	DrawSphere3D(m_attackXPos, m_attackXRadius, 16, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_attackYPos, m_attackYRadius, 16, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_attackYPos, m_attackShockRadius, 16, 0xff00ff, 0xffffff, false);

	if (m_isAttack)
	{
		if (m_attackKind == Game::e_PlayerAttackKind::kPlayerAttackX) DrawSphere3D(m_attackXPos, m_attackXRadius, 16, 0xffff00, 0xffffff, false);
		if (m_attackKind == Game::e_PlayerAttackKind::kPlayerAttackY) DrawSphere3D(m_attackYPos, m_attackYRadius, 16, 0xffff00, 0xffffff, false);
		if (m_attackKind == Game::e_PlayerAttackKind::kPlayerShock) DrawSphere3D(m_attackYPos, m_attackShockRadius, 16, 0xffff00, 0xffffff, false);
	}

	if (m_isHitDamage)
	{
		DrawSphere3D(m_hitPos, m_hitRadius, 16, 0xff00ff, 0xffffff, false);
	}

#endif

	// ダメージ演出  2フレーム間隔で表示非表示切り替え
	if (m_damageFrame % kDamageSmashNum >= kDrawHiddenNum) return;

	//武器の描画
	WeaponDraw(weapon);
	////モデルの描画
	PlayerDraw();

}

void Player::BossLook(VECTOR bossPos)
{
	m_pos = m_rigidbody.GetPos();
	m_bossPos = bossPos;
	m_bossToPlayerVec = VSub(m_pos, m_bossPos);

	PlayerSetPosAndRotation(m_pos, m_angle);
}

void Player::HitUpdate(VECTOR hitPos,VECTOR attackPos, VECTOR weaponPos, VECTOR shockPos, float hitRadius, float attackRadius, float weaponRadius, float shockRadius, bool isBossAttack)
{
	m_isBossAttack = isBossAttack;

	m_bossHitPos = hitPos;
	m_bossAttackPos = attackPos;
	m_bossWeaponPos = weaponPos;
	m_bossShockPos = shockPos;

	m_bossHitRadius = hitRadius;
	m_bossAttackRadius = attackRadius;
	m_bossWeaponRadius = weaponRadius;
	m_bossShockRadius = shockRadius;
}

void Player::Hit()
{
	/* ボスの攻撃に当たった判定 */
	//ボスが攻撃アクションをしたかどうかの判定
	if (!m_isGameOver)
	{
		if (m_isBossAttack)
		{
			//ヒット判定がfalseだった場合	
			if (!m_isHitDamage)
			{
				//手の攻撃をした場合
				if (IsAttackHit(m_bossAttackPos, m_bossAttackRadius) && m_bossAttackKind == Game::e_BossAttackKind::kBossAttack)
				{
					//武器なし攻撃は1ダメージ
					OnHitOneDamage();
				}

				//手の攻撃をした場合
				if (IsWeaponHit(m_bossWeaponPos, m_bossWeaponRadius) && m_bossAttackKind == Game::e_BossAttackKind::kBossWeapon)
				{
					//武器は2ダメージ
					OnHitTwoDamage();
				}

				//手の攻撃をした場合 (enumでどこで攻撃したかを教える)
				if (IsShockAttackHit(m_bossShockPos, m_bossShockRadius) && m_bossAttackKind == Game::e_BossAttackKind::kBossShock)
				{
					//武器なし攻撃は1ダメージ
					OnHitTwoDamage();
				}
			}
		}
	}
}

bool Player::IsAttackHit(VECTOR attackPos, float radius)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - attackPos.x) * (m_hitPos.x - attackPos.x);
	float delY = ((m_hitPos.y) - (attackPos.y)) * ((m_hitPos.y) - (attackPos.y));
	float delZ = (m_hitPos.z - attackPos.z) * (m_hitPos.z - attackPos.z);
	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + radius)
	{
		return true;
	}

	return false;
}

bool Player::IsWeaponHit(VECTOR attackPos, float radius)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - attackPos.x) * (m_hitPos.x - attackPos.x);
	float delY = ((m_hitPos.y) - (attackPos.y)) * ((m_hitPos.y) - (attackPos.y));
	float delZ = (m_hitPos.z - attackPos.z) * (m_hitPos.z - attackPos.z);
	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + radius)
	{
		return true;
	}

	return false;
}

bool Player::IsShockAttackHit(VECTOR attackPos, float radius)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - attackPos.x) * (m_hitPos.x - attackPos.x);
	float delY = ((m_hitPos.y) - (attackPos.y)) * ((m_hitPos.y) - (attackPos.y));
	float delZ = (m_hitPos.z - attackPos.z) * (m_hitPos.z - attackPos.z);
	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + radius)
	{
		return true;
	}

	return false;
}

bool Player::IsPlayerAttackHit(VECTOR attackPos, float attackRadius)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_bossHitPos.x - attackPos.x) * (m_bossHitPos.x - attackPos.x);
	float delY = ((m_bossHitPos.y) - (attackPos.y)) * ((m_bossHitPos.y) - (attackPos.y));
	float delZ = (m_bossHitPos.z - attackPos.z) * (m_bossHitPos.z - attackPos.z);
	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_bossHitRadius + attackRadius)
	{
		return true;
	}

	return false;
}

const VECTOR& Player::GetPos() const
{
	return m_rigidbody.GetPos();
}

void Player::IdleUpdate()
{	
	//ヒット処理
	Hit();

	m_stamina += kStaminaIncreaseSpeed;

	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR input = VGet(m_analogX, 0.0f, -m_analogZ);

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}

	//スティックの入力があったら
	if (VSquareSize(input) != 0.0f)
	{
		OnWalk();
		return;
	}
	
	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		OnJump();
		return;
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(kPadButtonY) && !m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプ以外はチャージあり)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプのみショートカット)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isUseMp = true;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackY();
	}

	//プレイヤーを変える関数
	FaceUse();

	//ロックオンした場合
	if (m_isLockOn)
	{
		OnLockOnIdle();
	}

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

	//顔を選択する関数
	FaceSelect();

}

void Player::LockOnIdleUpdate()
{
	//ヒット処理
	Hit();


	m_bossToPlayerVec = VNorm(m_bossToPlayerVec);
	m_angle = atan2f(m_bossToPlayerVec.x, m_bossToPlayerVec.z);

	m_stamina += kStaminaIncreaseSpeed;

	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR input = VGet(m_analogX, 0.0f, -m_analogZ);

	if (VSquareSize(input) > 0.0f)
	{
		VECTOR playerToBoss = VSub(m_bossPos, m_pos);
		playerToBoss = VNorm(playerToBoss);
		m_angle = atan2f(m_bossToPlayerVec.x, m_bossToPlayerVec.z);
		m_attackDir = VGet(playerToBoss.x, playerToBoss.y, playerToBoss.z);
	}

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}

	//スティックの入力があったら
	if (VSquareSize(input) != 0.0f)
	{
		OnLockOnWalk();
		return;
	}

	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		OnJump();
		return;
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(kPadButtonY) && !m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプ以外はチャージあり)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプのみショートカット)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isUseMp = true;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackY();
	}

	FaceUse();

	//ロックオンしていなかったら
	if (!m_isLockOn)
	{
		OnIdle();
	}

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

	//顔を選択する関数
	FaceSelect();
}

void Player::WalkUpdate()
{
	//ヒット処理
	Hit();

	//プレイヤーの移動
	Move();

	m_stamina += kStaminaIncreaseSpeed;

	//ロックオン機能を入れたら
	if (m_isLockOn)
	{
		OnLockOnWalk();
		return;
	}

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(kPadButtonY) && !m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		//OnAttackY();
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプ以外はチャージあり)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプのみショートカット)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isUseMp = true;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackY();
	}

	//ダッシュ
	if (Pad::IsPress(kPadButtonA) && !m_isStamina)
	{
		OnDash();
		return;
	}

	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		OnJump();
		return;
	}

	FaceUse();


	//顔を選択する関数
	FaceSelect();
}

void Player::LockOnWalkUpdate()
{
	//ヒット処理
	Hit();

	//プレイヤーの移動
	Move();

	m_stamina += kStaminaIncreaseSpeed;

	//ロックオン機能を外したら
	if (!m_isLockOn)
	{
		OnWalk();
		return;
	}

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(kPadButtonY) && !m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		//OnAttackY();
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプ以外はチャージあり)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
	}
	//攻撃Y(スピードタイプのみショートカット)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isUseMp = true;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackY();
	}

	//ダッシュ
	if (Pad::IsPress(kPadButtonA) && !m_isStamina)
	{
		OnDash();
		return;
	}

	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		OnJump();
		return;
	}

	FaceUse();

	//顔を選択する関数
	FaceSelect();
}

void Player::DashUpdate()
{
	//ヒット処理
	Hit();

	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);

	//移動中に足音のSEを鳴らす
	//キャラクターの種類によって変える
	if (len != 0.0f)
	{
		if (m_moveCount % kMoveCountNum == 0)
		{
			SoundManager::GetInstance().PlaySe("footstepsSe");
			auto pos = m_rigidbody.GetPos();
			EffectManager::GetInstance().CreateEffect("moveEffect", pos);
		}
		m_moveCount++;

	}
	else
	{
		m_moveCount = 0;
	}


	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	float speed = 0;



	//動いている間
	if (VSquareSize(move) > 0.01f)
	{
		m_stamina -= kStaminaDiminishSpeed;

		//カメラの正面方向ベクトル
		VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
		//カメラの右方向ベクトル
		VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

		//向きベクトル*移動量
		front = VScale(front, -move.x);
		//向きベクトル*移動量
		right = VScale(right, -move.z);

		move = VAdd(front, right);
		move = VNorm(move);

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		//プレイヤーのタイプで攻撃アニメーションを変える
		if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kPowerDashSpeed * rate;
		}
		else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kSpeedDashSpeed * rate;
		}
		else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kShotDashSpeed * rate;
		}
		else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kStrongestDashSpeed * rate;
		}

		if (!m_isFaceUse)
		{
			//ダッシュ
			speed = kNormalDashSpeed * rate;
		}

		move = VScale(move, speed);

		//カメラのいる場所(角度)から
		//コントローラーによる移動方向を決定する
		MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
		move = VTransform(move, mtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);
		m_attackDir = move;

	}
	else
	{
		OnIdle();
		return;
	}

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}



	//歩き
	if (Pad::IsRelase(kPadButtonA) || m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;

		if (m_isLockOn)
		{
			OnLockOnWalk();
			return;
		}
		else
		{
			OnWalk();
			return;
		}
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(kPadButtonY) && !m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		//OnAttackY();
		OnAttackCharge();
		return;
	}
	//攻撃Y(スピードタイプ以外はチャージあり)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackCharge();
		return;
	}
	//攻撃Y(スピードタイプのみショートカット)
	else if (Pad::IsPress(kPadButtonY) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && !m_isStamina && !m_isMp)
	{
		m_isUseMp = true;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackY();
		return;
	}

	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnDashJump();
		return;
	}

	FaceUse();



	//顔を選択する関数
	FaceSelect();
}

void Player::JumpUpdate()
{
	//ヒット処理
	Hit();

	m_stamina += kStaminaIncreaseSpeed;
	m_jumpCount++;

	auto vel = m_rigidbody.GetVelocity();

	if (m_jumpCount < kJumpCountNum)
	{
		vel.y += kMaxJumpPower;
		m_jumpPower += kMaxJumpPower;
	}
	else
	{
		vel.y += kMinJumpPower;
		m_jumpPower += kMinJumpPower;
	}

	if (m_jumpPower > kJumpPowerNum)
	{
		OnAir();
	}

	m_rigidbody.SetVelocity(vel);

	if (m_jumpCount > kJumpCountMaxNum)
	{
		OnFall();
	}

	//Move();

	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//カメラの正面方向ベクトル
	VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//カメラの右方向ベクトル
	VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

	//向きベクトル*移動量
	front = VScale(front, -move.x);
	//向きベクトル*移動量
	right = VScale(right, -move.z);

	move = VAdd(front, right);
	move = VNorm(move);

	float speed = 0;

	//プレイヤーのタイプで攻撃アニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kPowerMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kSpeedMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kShotMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kStrongestDashSpeed * rate;
	}

	if (!m_isFaceUse)
	{
		//ダッシュ
		speed = kNormalMaxSpeed * rate;
	}

	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);



	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);

}

void Player::DashJumpUpdate()
{
	//ヒット処理
	Hit();


	m_stamina += kStaminaIncreaseSpeed;

	m_jumpCount++;

	auto vel = m_rigidbody.GetVelocity();

	if (m_jumpCount < kJumpCountNum)
	{
		vel.y += kMaxJumpPower;
		m_jumpPower += kMaxJumpPower;
	}
	else
	{
		vel.y += kMinJumpPower;
		m_jumpPower += kMinJumpPower;
	}

	if (m_jumpPower > kJumpPowerNum)
	{
		OnDashAir();
	}

	m_rigidbody.SetVelocity(vel);

	if (m_jumpCount > kJumpCountMaxNum)
	{
		OnDashFall();
	}

	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//カメラの正面方向ベクトル
	VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//カメラの右方向ベクトル
	VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

	//向きベクトル*移動量
	front = VScale(front, -move.x);
	//向きベクトル*移動量
	right = VScale(right, -move.z);

	move = VAdd(front, right);
	move = VNorm(move);

	float speed = 0;

	//プレイヤーのタイプで攻撃アニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kPowerDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kSpeedDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kShotDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kStrongestDashSpeed * rate;
	}

	if (!m_isFaceUse)
	{
		//ダッシュ
		speed = kNormalDashSpeed * rate;
	}

	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);



	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::FallUpdate()
{
	//ヒット処理
	Hit();

	m_stamina += kStaminaIncreaseSpeed;

	m_jumpCount++;

	auto vel = m_rigidbody.GetVelocity();

	if (m_jumpCount < 15)
	{
		vel.y += kMinJumpPower;
	}
	else if (m_jumpCount < 30)
	{
		vel.y -= kMinJumpPower;
	}
	else if (m_jumpCount < 45)
	{
		vel.y -= kMinJumpPower * kMinFallScale;
	}
	else
	{
		vel.y -= kMinJumpPower * kMaxFallScale;
	}

	m_rigidbody.SetVelocity(vel);

	if (GetIsJump())
	{
		m_buttonKind = e_ButtonKind::kNone;
		m_isButtonPush = false;

		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}

	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);

	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//カメラの正面方向ベクトル
	VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//カメラの右方向ベクトル
	VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

	//向きベクトル*移動量
	front = VScale(front, -move.x);
	//向きベクトル*移動量
	right = VScale(right, -move.z);

	move = VAdd(front, right);
	move = VNorm(move);

	float speed = 0;

	//プレイヤーのタイプで攻撃アニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kPowerMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kSpeedMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kShotMaxSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		//ダッシュ
		speed = kStrongestDashSpeed * rate;
	}

	if (!m_isFaceUse)
	{
		//ダッシュ
		speed = kNormalMaxSpeed * rate;
	}

	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);
		m_attackDir = move;
	}



	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::DashFallUpdate()
{
	//ヒット処理
	Hit();

	m_stamina += kStaminaIncreaseSpeed;

	m_jumpCount++;

	auto vel = m_rigidbody.GetVelocity();

	if (m_jumpCount < 15)
	{
		//vel.y -= kGravityPower * 0.75f;
		vel.y += kMinJumpPower;
	}
	else if (m_jumpCount < 30)
	{
		vel.y -= kMinJumpPower;
	}
	else if (m_jumpCount < 45)
	{
		vel.y -= kMinJumpPower * kMinFallScale;
	}
	else
	{
		vel.y -= kMinJumpPower * kMaxFallScale;
	}

	m_rigidbody.SetVelocity(vel);

	if (GetIsJump())
	{
		m_buttonKind = e_ButtonKind::kNone;
		m_isButtonPush = false;

		if (m_isLockOn)
		{
			OnLockOnIdle();
		}
		else
		{
			OnIdle();
		}
	}

	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);

	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//カメラの正面方向ベクトル
	VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
	//カメラの右方向ベクトル
	VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

	//向きベクトル*移動量
	front = VScale(front, -move.x);
	//向きベクトル*移動量
	right = VScale(right, -move.z);

	move = VAdd(front, right);
	move = VNorm(move);

	float speed = 0;

	//プレイヤーのタイプで攻撃アニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		speed = kPowerDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		speed = kSpeedDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		speed = kShotDashSpeed * rate;
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		speed = kStrongestDashSpeed * rate;
	}

	if (!m_isFaceUse)
	{
		speed = kNormalDashSpeed * rate;
	}

	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);
		m_attackDir = move;
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::AttackCharge()
{
	//ヒット処理
	Hit();
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));


	m_stamina += kStaminaIncreaseSpeed;

	m_chargeTime++;

	if (m_chargeTime == 1)
	{
		auto pos = m_rigidbody.GetPos();
		EffectManager::GetInstance().CreateEffect("attackChargeEffect", pos);
	}

	if (m_chargeTime == kChargeTimeMax)
	{
		SoundManager::GetInstance().PlaySe("healHpSe");
		auto pos = m_rigidbody.GetPos();
		EffectManager::GetInstance().CreateEffect("attackChargeFinishEffect", pos);
	}

	//Yボタンを離した場合
	if (Pad::IsRelase(kPadButtonY) && m_chargeTime > kChargeTimeMax)
	{
		m_chargeTime = 0;

		m_isUseMp = true;
		OnAttackY();
	}
	else if(Pad::IsRelase(kPadButtonY) && m_chargeTime < kChargeTimeMax)
	{
		m_chargeTime = 0;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;

		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}
}

void Player::AttackXUpdate()
{
	//ヒット処理
	Hit();
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));
	m_stamina += kStaminaIncreaseSpeed;

	m_attackFrame++;
	if (m_attackFrame > kAttackXFrame)
	{
		m_isAttack = false;
	}

	//攻撃ボタンが押されたとき
	if (Pad::IsTrigger(PAD_INPUT_3) && !m_isNextAttackFlag)
	{
		m_isNextAttackFlag = true;
	}

	//アニメーションが終わった段階で次の攻撃フラグがたっていなかったら
	if (m_pAnim->IsLoop() && !m_isNextAttackFlag)
	{

		m_multiAttack = 0;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;

		//アイドル状態に戻る
		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}

	//アニメーションが終わった段階で次の攻撃フラグがたっていたら
	if (m_pAnim->IsLoop() && m_isNextAttackFlag)
	{
		m_isAttack = false;
		m_isNextAttackFlag = false;
		m_multiAttack++;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		//もう一回攻撃状態に戻る
		OnAttackX();
	}
}

void Player::AttackYUpdate()
{
	//ヒット処理
	Hit();
	m_rigidbody.SetVelocity(VGet(0.0f, 0.0f, 0.0f));

	m_attackFrame++;
	m_stamina += kStaminaIncreaseSpeed;

	if (m_isUseMp)
	{
		m_mp -= kMpDiminishNum;
		m_isUseMp = false;
	}

	if (m_attackFrame > kAttackFrame)
	{
		m_isAttack = true;
	}
	else if (m_attackFrame > 40)
	{
		m_isAttack = false;
	}

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{

		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;

		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}

}

void Player::HitOneDamageUpdate()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_stamina += kStaminaIncreaseSpeed;

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}
}

void Player::HitTwoDamageUpdate()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		//ロックオンしていた場合
		if (m_isLockOn)
		{
			OnLockOnIdle();
			return;
		}
		else //していない場合
		{
			OnIdle();
			return;
		}
	}
}

void Player::DeadUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;
	m_deadTime++;

	if (!m_isDead && m_deadTime > kDeadTime)
	{
		m_isDead = true;
		SoundManager::GetInstance().PlaySe("deadSe");
	}

	//ゲームオーバーシーンに遷移するためにフラグを立てる
	m_isGameOver = true;


	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;
}

void Player::FaceChangeUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//プレイヤーのタイプで攻撃アニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeModel(m_modelPowerH, kAnimIdle, kPowerAnimInfoFilename);
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeModel(m_modelSpeedH, kAnimIdle, kSpeedAnimInfoFilename);
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeModel(m_modelShotH, kAnimIdle, kShotAnimInfoFilename);
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeModel(m_modelRassH, kAnimIdle, kRassAnimInfoFilename);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeModel(m_modelH, kAnimIdle, kNormalAnimInfoFilename);
	}

	//ロックオンしていた場合
	if (m_isLockOn)
	{
		OnLockOnIdle();
		return;
	}
	else //していない場合
	{
		OnIdle();
		return;
	}

}

void Player::FaceUseUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	if (m_pAnim->IsLoop())
	{
		OnFaceChange();
	}
}

void Player::TalkUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	//会話するときの処理を入れる
}

void Player::WeaponDraw(PlayerWeapon& weapon)
{
	//顔を付けている場合
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		//m_pWeapon->AxeDraw();
		weapon.AxeDraw();
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		//m_pWeapon->DaggerDraw();
		weapon.DaggerDraw();
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		//m_pWeapon->MagicWandDraw();
		weapon.MagicWandDraw();
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		//m_pWeapon->LongSwordDraw();
		weapon.LongSwordDraw();
	}

	//顔を付けていない場合
	if (!m_isFaceUse)
	{
		//武器をアタッチする描画関数
		//m_pWeapon->SwordDraw();
		weapon.SwordDraw();
	}
}

void Player::PlayerDraw()
{
	//顔を付けている場合
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer))
	{
		//モデルの描画
		MV1DrawModel(m_modelPowerH);
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer))
	{
		//モデルの描画
		MV1DrawModel(m_modelSpeedH);
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
	{
		//モデルの描画
		MV1DrawModel(m_modelShotH);
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kRassPlayer))
	{
		//モデルの描画
		MV1DrawModel(m_modelRassH);
	}

	//顔を付けていない場合
	if (!m_isFaceUse)
	{
		//モデルの描画
		MV1DrawModel(m_modelH);
	}
}

void Player::SetAttackCollision(float attackXRadius, float attackYRadius, float attackShockRadius, float attackXMoveScale, float attackYMoveScale)
{
	//球の攻撃半径
	m_attackXRadius = attackXRadius;
	m_attackYRadius = attackYRadius;
	m_attackShockRadius = attackShockRadius;

	//攻撃X座標
	m_attackMove = VScale(m_attackDir, attackXMoveScale);
	m_attackXPos = VAdd(VGet(m_hitPos.x, m_hitPos.y - kDownAttackPos, m_hitPos.z), m_attackMove);

	//攻撃Y座標
	m_attackMove = VScale(m_attackDir, attackYMoveScale);
	m_attackYPos = VAdd(VGet(m_hitPos.x, m_hitPos.y - kDownAttackPos, m_hitPos.z), m_attackMove);
}

void Player::PlayerSetPosAndRotation(VECTOR pos, float angle)
{
	//モデルに座標をセットする
	MV1SetPosition(m_modelH, pos);
	MV1SetPosition(m_modelPowerH, pos);
	MV1SetPosition(m_modelSpeedH, pos);
	MV1SetPosition(m_modelShotH, pos);
	MV1SetPosition(m_modelRassH, pos);

	//モデルに回転をセットする
	MV1SetRotationXYZ(m_modelH, VGet(0, angle, 0));
	MV1SetRotationXYZ(m_modelPowerH, VGet(0, angle, 0));
	MV1SetRotationXYZ(m_modelSpeedH, VGet(0, angle, 0));
	MV1SetRotationXYZ(m_modelShotH, VGet(0, angle, 0));
	MV1SetRotationXYZ(m_modelRassH, VGet(0, angle, 0));

}

void Player::Move()
{
	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);

	//移動中に足音のSEを鳴らす
	//キャラクターの種類によって変える
	if (len != 0.0f)
	{
		if (m_moveCount % kWalkCountNum == 0)
		{
			SoundManager::GetInstance().PlaySe("footstepsSe");
		}
		m_moveCount++;

	}
	else
	{
		m_moveCount = 0;
	}

	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	float speed = 0;

	//動いている間
	if (len != 0.0f)
	{

		//カメラの正面方向ベクトル
		VECTOR front = VGet(m_cameraDirection.x, 0.0f, m_cameraDirection.z);
		//カメラの右方向ベクトル
		VECTOR right = VGet(-m_cameraDirection.z, 0.0f, m_cameraDirection.x);

		//向きベクトル*移動量
		front = VScale(front, -move.x);
		//向きベクトル*移動量
		right = VScale(right, -move.z);

		//移動量に加算
		move = VAdd(front, right);
		move = VNorm(move);

		if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kPowerMinSpeed * rate;
				move = VScale(move, speed);
			}
			if (rate >= 0.6f)
			{
				speed = kPowerMaxSpeed * rate;
				move = VScale(move, speed);
			}
		}
		if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kSpeedMinSpeed * rate;
				move = VScale(move, speed);
			}
			if (rate >= 0.6f)
			{
				speed = kSpeedMaxSpeed * rate;
				move = VScale(move, speed);
			}
		}
		if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kShotMinSpeed * rate;
				move = VScale(move, speed);
			}
			if (rate >= 0.6f)
			{
				speed = kShotMaxSpeed * rate;
				move = VScale(move, speed);
			}
		}
		if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kNormalMinSpeed * rate;
				move = VScale(move, speed);
			}
			if (rate >= 0.6f)
			{
				speed = kNormalMaxSpeed * rate;
				move = VScale(move, speed);
			}
		}

		if (!m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kNormalMinSpeed * rate;
				move = VScale(move, speed);
			}
			if (rate >= 0.6f)
			{
				speed = kNormalMaxSpeed * rate;
				move = VScale(move, speed);
			}
		}

		//カメラのいる場所(角度)から
		//コントローラーによる移動方向を決定する
		MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
		move = VTransform(move, mtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_move = move;

		if (!m_isLockOn)
		{
			m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

			//速度が決定できるので移動ベクトルに反映する
			move = VNorm(move);
			m_attackDir = move;

		}
		else
		{
			//速度が決定できるので移動ベクトルに反映する
			move = VNorm(move);
			m_bossToPlayerVec = VNorm(m_bossToPlayerVec);

			//移動方向からプレイヤーの向く方向を決定する
			//移動していない場合(ゼロベクトル)の場合は変更しない
			if (VSquareSize(move) > 0.0f)
			{
				VECTOR playerToBoss = VSub(m_bossPos, m_pos);
				playerToBoss = VNorm(playerToBoss);
				m_angle = atan2f(m_bossToPlayerVec.x, m_bossToPlayerVec.z);
				m_attackDir = VGet(playerToBoss.x, playerToBoss.y, playerToBoss.z);
			}
		}
	}
	//動かなかったらアイドル状態へ
	else
	{
		OnIdle();
		return;
	}
}

void Player::CollisionPosUpdate()
{
	//プレイヤーの座標を代入
	m_pos = m_rigidbody.GetPos();

	//ヒット座標
	m_hitPos = VGet(m_pos.x, m_pos.y + kUpHitPos, m_pos.z);
	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//プレイヤーの当たり判定の位置と大きさ
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//攻撃の半径と座標をセット
		SetAttackCollision(kPowerAttackXRadius, kPowerAttackYRadius, kPowerAttackShockRadius, kPowerAttackXMoveScale, kPowerAttackYMoveScale);
		m_attackYPos = VGet(m_pos.x, m_pos.y + kUpAttackPos, m_pos.z);
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//攻撃の半径と座標をセット
		SetAttackCollision(kSpeedAttackXRadius, kSpeedAttackYRadius, kSpeedAttackShockRadius, kSpeedAttackXMoveScale, kSpeedAttackYMoveScale);
		m_attackYPos = VAdd(VGet(m_hitPos.x, m_hitPos.y - kDownAttackPos, m_hitPos.z), m_attackMove);
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//球の攻撃半径
		SetAttackCollision(kShotAttackXRadius, kShotAttackYRadius, kShotAttackShockRadius, kShotAttackXMoveScale, kShotAttackYMoveScale);
		m_attackYPos = VAdd(VGet(m_hitPos.x, m_hitPos.y - kDownAttackPos, m_hitPos.z), m_attackMove);
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		//球の攻撃半径
		SetAttackCollision(kRassAttackXRadius, kRassAttackYRadius, kRassAttackShockRadius, kRassAttackXMoveScale, kRassAttackYMoveScale);
		m_attackYPos = VGet(m_pos.x, m_pos.y + kUpAttackPos, m_pos.z);
	}

	//顔を付けていない場合
	if (!m_isFaceUse)
	{
		//球の攻撃半径
		SetAttackCollision(kNormalAttackXRadius, kNormalAttackYRadius, kNormalAttackShockRadius, kNormalAttackXMoveScale, kNormalAttackYMoveScale);
		m_attackYPos = VGet(m_pos.x, m_pos.y + kUpAttackPos, m_pos.z);
	}
}

void Player::OnIdle()
{
	//リセット処理
	SetOnReset();

	//アイドルアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &Player::IdleUpdate;
}

void Player::OnLockOnIdle()
{
	//リセット処理
	SetOnReset();

	//アイドルアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &Player::LockOnIdleUpdate;
}

void Player::OnWalk()
{
	//リセット処理
	SetOnReset();

	//歩きアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimWalk);
	m_updateFunc = &Player::WalkUpdate;
}

void Player::OnLockOnWalk()
{
	//リセット処理
	SetOnReset();

	//ロックオン歩きアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimLockOnWalk);
	m_updateFunc = &Player::LockOnWalkUpdate;
}

void Player::OnDash()
{
	//リセット処理
	SetOnReset();
	//Aボタンの処理をセット
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kAbutton;

	//走りアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimDash);
	m_updateFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	m_attackFrame = 0;
	m_isAttack = true;
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kXbutton;
	m_attackKind = Game::e_PlayerAttackKind::kPlayerAttackX;

	//攻撃エフェクト(ショットのみ)
	if (m_playerKind == Game::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		EffectManager::GetInstance().CreateEffect("shotPlayerAttackXEffect", VGet(m_attackXPos.x, m_attackXPos.y - 4.0f, m_attackXPos.z));
	}

	//タイプによって攻撃アニメーションを変える
	switch (m_multiAttack)
	{
	case 0:
		//一番目の攻撃アニメーション
		m_pAnim->ChangeAnim(kAnimAttackX_First);
		SoundManager::GetInstance().PlaySe("attackFirstSe");

		break;
	case 1:
		//二番目の攻撃アニメーション
		m_pAnim->ChangeAnim(kAnimAttackX_Second);
		SoundManager::GetInstance().PlaySe("attackSecondSe");
		break;
	case 2:
		//三番目の攻撃アニメーション
		m_pAnim->ChangeAnim(kAnimAttackX_Third);
		SoundManager::GetInstance().PlaySe("attackFirstSe");
		break;
	case 3:
		//四番目の攻撃アニメーション
		m_pAnim->ChangeAnim(kAnimAttackX_Fourth);
		SoundManager::GetInstance().PlaySe("attackThirdSe");
		break;
	default:
		break;
	}

	m_updateFunc = &Player::AttackXUpdate;
}

void Player::OnAttackY()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//攻撃YのSE
	SoundManager::GetInstance().PlaySe("attackYSe");
	//攻撃Yのエフェクト
	if (m_playerKind == Game::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		EffectManager::GetInstance().CreateEffect("shotPlayerAttackYEffect", VGet(m_attackYPos.x, m_attackYPos.y - 4.0f, m_attackYPos.z));
		m_isAttack = false;
	}
	else if (m_playerKind == Game::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		EffectManager::GetInstance().CreateEffect("speedPlayerAttackYEffect", m_attackYPos);
		m_isAttack = true;
	}
	else
	{
		EffectManager::GetInstance().CreateEffect("attackYEffect", m_pos);
		m_isAttack = true;
	}

	m_attackFrame = 0;
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;
	m_attackKind = Game::e_PlayerAttackKind::kPlayerAttackY;

	//Y攻撃のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimAttackY);
	m_updateFunc = &Player::AttackYUpdate;
}

void Player::OnJump()
{
	//ジャンプ処理
	SetOnJump();

	//ジャンプのアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
	m_updateFunc = &Player::JumpUpdate;
}

void Player::OnDashJump()
{
	//ジャンプ処理
	SetOnJump();
	m_stamina -= 20;

	//ダッシュジャンプのアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
	m_updateFunc = &Player::DashJumpUpdate;
}

void Player::OnAir()
{
	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::JumpUpdate;
}

void Player::OnDashAir()
{
	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::DashJumpUpdate;
}

void Player::OnFall()
{
	m_jumpCount = 0;

	//落下中のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::FallUpdate;
}

void Player::OnDashFall()
{
	m_jumpCount = 0;

	//ダッシュ中の落下アニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::DashFallUpdate;
}

void Player::OnAttackCharge()
{
	//リセット処理
	SetOnReset();

	//チャージSE
	SoundManager::GetInstance().PlaySe("attackChargeSe");
	//ボタンの種類や押したかのフラグをセット
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;

	//攻撃チャージのアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimAttackCharge);
	m_updateFunc = &Player::AttackCharge;
}

void Player::OnHitOneDamage()
{
	//ダメージ処理
	SetOnDamage();
	m_hp -= 1;

	//ヒットした時のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimHit);
	m_updateFunc = &Player::HitOneDamageUpdate;
}

void Player::OnHitTwoDamage()
{
	//ダメージ処理
	SetOnDamage();
	m_hp -= 2;

	//ヒットした時のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimHit);
	m_updateFunc = &Player::HitTwoDamageUpdate;
}

void Player::OnDead()
{
	//リセット処理
	SetOnReset();

	//死亡した時のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &Player::DeadUpdate;
}

void Player::OnFaceChange()
{
	m_isFaceUse = !m_isFaceUse;
	m_updateFunc = &Player::FaceChangeUpdate;
}

void Player::OnFaceUse()
{
	//リセット処理
	SetOnReset();

	//SE
	SoundManager::GetInstance().PlaySe("faceUseSe");
	//エフェクト
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("faceUseEffect", VGet(pos.x, pos.y + 8.0f, pos.z));

	//カオを使う時のアニメーションにチェンジ
	m_pAnim->ChangeAnim(kAnimUseFace);
	m_updateFunc = &Player::FaceUseUpdate;
}

void Player::SetOnReset()
{
	//遷移する時に移動しないよう力をリセット
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//攻撃している種類のリセット
	m_attackKind = Game::e_PlayerAttackKind::kPlayerAttackNone;
	
	//カウントをリセット
	m_jumpCount = 0;

	//攻撃関係をリセット
	m_isAttack = false;
	m_multiAttack = 0;
	m_attackFrame = 0;

}

void Player::SetOnDamage()
{
	//リセット処理
	SetOnReset();

	//プラスアルファでヒットした時の処理
	m_isHitDamage = true;
	SoundManager::GetInstance().PlaySe("damageSe");

	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("hitEffect", VGet(pos.x, pos.y + 4.0f, pos.z));

}

void Player::SetOnJump()
{
	//ジャンプSE
	SoundManager::GetInstance().PlaySe("jumpSe");
	//ジャンプエフェクト
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("jumpEffect", pos);

	//ジャンプに使う時とかの変数をリセット
	m_jumpCount = 0;
	m_jumpPower = 0;
	//ボタンについての処理
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kBbutton;
}

void Player::DamageUpdate()
{
	//ヒットした場合の処理
	if (m_isHitDamage)
	{
		m_damageFrame++;
	}
	else
	{
		m_damageFrame = 0;
	}

	if (m_damageFrame >= kDamageFrame)
	{
		//ヒット無敵時間を解除
		m_isHitDamage = false;
	}
}

void Player::StatusUpdate()
{
	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		//HPが反転しないよう
		m_hp = 0;
		//死亡状態へ遷移
		OnDead();
	}

	//MPが0以下になったら
	if (m_mp <= 0)
	{
		m_isMp = true;
		m_mp = 0;
	}

	//MPが最大値を超えたら
	if (m_mp > kMaxMp)
	{
		m_mp = kMaxMp;
	}

	//スタミナ関連
	if (m_stamina <= 0)
	{
		m_stamina = 0;
		m_isStamina = true;
	}

	//スタミナがスタミナの最大値を超えた場合
	if (m_stamina > kMaxStamina)
	{
		m_stamina = kMaxStamina;
	}

	//スタミナがスタミナの最大値に達した場合
	if (m_stamina == kMaxStamina)
	{
		m_isStamina = false;
	}
}

void Player::FaceSelect()
{
	//顔を選択
	if (Pad::IsTrigger(kPadButtonRB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kRassPlayer)
		{
			SoundManager::GetInstance().PlaySe("faceSelectSe");
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) + 1);
		}
		else if (m_playerKind == e_PlayerKind::kRassPlayer)
		{
			SoundManager::GetInstance().PlaySe("faceSelectSe");
			m_playerKind = e_PlayerKind::kPowerPlayer;
		}
	}
	//顔を選択
	if (Pad::IsTrigger(kPadButtonLB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kPowerPlayer)
		{
			SoundManager::GetInstance().PlaySe("faceSelectSe");
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) - 1);
		}
		else if (m_playerKind == e_PlayerKind::kPowerPlayer)
		{
			SoundManager::GetInstance().PlaySe("faceSelectSe");
			m_playerKind = e_PlayerKind::kRassPlayer;
		}
	}
}

void Player::FaceUse()
{
	if (m_input.Z <= kRTButtonNum && m_playerKind == e_PlayerKind::kPowerPlayer&& SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kPowerPlayer))
	{
		OnFaceUse();
		return;
	}
	else if (m_input.Z <= kRTButtonNum && m_playerKind == e_PlayerKind::kSpeedPlayer && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kSpeedPlayer))
	{
		OnFaceUse();
		return;
	}
	else if (m_input.Z <= kRTButtonNum && m_playerKind == e_PlayerKind::kShotPlayer && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kShotPlayer))
	{
		OnFaceUse();
		return;
	}
	else if (m_input.Z <= kRTButtonNum && m_playerKind == e_PlayerKind::kRassPlayer && SaveDataManager::GetInstance().IsRelease(Game::e_PlayerKind::kRassPlayer))
	{
		OnFaceUse();
		return;
	}

}

const float& Player::GetRadius() const
{
	return m_radius;
}
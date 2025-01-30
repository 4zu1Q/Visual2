#include "Player.h"
#include "PlayerWeapon.h"
#include "object/weapon/WeaponBase.h"

#include "util/Pad.h"
#include "util/AnimController.h"
#include "util/EffectManager.h"
#include "util/SoundManager.h"

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
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
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
	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,12.0f,0.0f };

	//見えない壁を作るための座標
	constexpr VECTOR kWallRightPos = { 510.0f,0.0f,790.0f };
	constexpr VECTOR kWallLeftPos = { -685.0f,0.0f,-600.0f };

	/*　プレイヤーのアニメーションの種類　*/
	const char* const kNormalAnimInfoFilename = "Data/Master/AnimPlayerNormalMaster.csv";
	const char* const kPowerAnimInfoFilename = "Data/Master/AnimPlayerPowerMaster.csv";
	const char* const kSpeedAnimInfoFilename = "Data/Master/AnimPlayerSpeedMaster.csv";
	const char* const kShotAnimInfoFilename = "Data/Master/AnimPlayerShotMaster.csv";
	const char* const kRassAnimInfoFilename = "Data/Master/AnimPlayerRassMaster.csv";

	//アイドル状態
	const char* const kAnimIdle = "Idle";

	//歩き状態
	const char* const kAnimWalk = "Walk";

	//ロックオン歩き状態
	const char* const kAnimLockOnWalk = "LockOnWalk";

	//ダッシュ状態
	const char* const kAnimDash = "Dash";

	//Y攻撃をするためのチャージアニメーション
	const char* const kAnimAttackCharge = "AttackCharge";

	const char* const kAnimAttackX_First = "AttackX_First";
	const char* const kAnimAttackX_Second = "AttackX_Second";
	const char* const kAnimAttackX_Third = "AttackX_Third";
	const char* const kAnimAttackX_Fourth = "AttackX_Fourth";

	const char* const kAnimAttackY = "AttackY";

	//ジャンプアニメーション
	const char* const kAnimJump = "Jump";
	const char* const kAnimFall = "Jumping";

	//ダメージとダウンアニメーション
	const char* const kAnimHit = "Hit";
	const char* const kAnimDead = "Down";

	//顔の使用アニメーション
	const char* const kAnimUseFace = "UseFace";

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
}

Player::Player() :
	CharaBase(Collidable::e_Priority::kHigh, Game::e_GameObjectTag::kPlayer, MyLib::ColliderData::e_Kind::kSphere, false),
	m_isGameOver(false),
	m_pos(VGet(0,0,0)),
	m_bossPos(VGet(0, 0, 0)),
	m_bossToPlayerVec(VGet(0, 0, 0)),
	m_weaponH(-1),
	m_radius(2),
	m_posUp(kInitPos),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_cameraToPlayerVec(VGet(0, 0, 0)),
	m_cameraDirection(VGet(0, 0, 0)),
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(kMaxHp),
	m_mp(kMaxMp),
	m_stamina(kMaxStamina),
	m_cameraAngle(0.0f),
	m_rate(0.0f),
	m_isDead(false),
	m_isStamina(false),
	m_isMp(false),
	m_isUseMp(false),
	m_frame(0),
	m_isMove(false),
	m_multiAttack(0),
	m_isNextAttackFlag(false),
	m_isLockOn(false),
	m_chargeTime(0),
	m_jumpCount(0),
	m_jumpPower(0.0f),
	m_playerRotMtx(),
	m_moveCount(0),
	m_deadTime(0),
	m_damageFrame(0),
	m_shadowH(0)
{

	// 影描画用の画像の読み込み
	m_shadowH = LoadGraph("Data/Image/Shadow.png");

	//モデルのロード
	m_modelH = MV1LoadModel(kNormalModelFilename);
	m_modelPowerH = MV1LoadModel(kPowerModelFilename);
	m_modelSpeedH = MV1LoadModel(kSpeedModelFilename);
	m_modelShotH = MV1LoadModel(kShotModelFilename);
	m_modelRassH = MV1LoadModel(kRassModelFilename);

	assert(m_modelH > -1);


	//マスク関連の初期化
	m_isFaceUse = false;
	m_playerKind = e_PlayerKind::kPowerPlayer;
	
	//ボタンUI関連
	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;

	m_pAnim = std::make_shared<AnimController>();

	m_pWeaponBase = std::make_shared<WeaponBase>();
	m_pWeaponBase->Initialize(m_weaponH, m_modelH, "handslot.r", "handslot.l", kWeaponScale);


	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 6.0f;

	m_radius = circleColliderData->m_radius;
}

Player::~Player()
{
	DeleteGraph(m_shadowH);
}

void Player::Initialize(std::shared_ptr<MyLib::Physics> physics, VECTOR pos, PlayerWeapon& weapon)
{
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(pos);
	m_speed = 0.1f;

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


	// メンバ関数ポインタの初期化
	m_updateFunc = &Player::IdleUpdate;

}

void Player::Finalize(std::shared_ptr<MyLib::Physics> physics)
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

	Collidable::Finalize(physics);
}

void Player::Update(std::shared_ptr<MyLib::Physics> physics, PlayerWeapon& weapon, float cameraAngleX, VECTOR bossPos, bool isLockOn)
{
	//アップデート
	(this->*m_updateFunc)();

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	//プレイヤーの座標を代入
	m_pos = m_rigidbody.GetPos();
	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//ボスの座標を代入
	m_isLockOn = isLockOn;
	m_bossPos = bossPos;
	m_bossToPlayerVec = VSub(m_pos, m_bossPos);


	//モデルのポジションを合わせるよう
	//VECTOR modelPos = VGet(m_pos.x, m_pos.y, m_pos.z);

	m_playerRotMtx = MGetRotY(cameraAngleX);

	PlayerSetPosAndRotation(m_pos, m_angle);

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
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

void Player::Draw(PlayerWeapon& weapon)
{
	
	//MV1DrawModel(m_modelH);

#ifdef _DEBUG

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_rigidbody.GetPos().x, m_rigidbody.GetPos().y, m_rigidbody.GetPos().z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
	DrawFormatString(0, 148, 0xff0fff, "playerHp:%f,playerMp:%f,playerStamina:%f", m_hp, m_mp, m_stamina);
	DrawFormatString(0, 348, 0xffffff, "playerAngle:%f", m_angle);
	DrawFormatString(0, 328, 0xffffff, "playerAngle:%d", GetIsJump());

#endif

	// ダメージ演出  2フレーム間隔で表示非表示切り替え
	// 0: 表示される
	// 1: 表示される
	// 2: 非表示
	// 3: 非表示
	// 4: 表示される	...
	// % 4 することで012301230123... に変換する
	if (m_damageFrame % 8 >= 4) return;

	//武器の描画
	WeaponDraw(weapon);
	////モデルの描画
	PlayerDraw();

}

//この判定処理はまた使えるようになりたい(これでアイテム触った時と敵の攻撃とかを判定したいから)
void Player::OnCollide(const Collidable& colider)
{
	std::string message = "プレイヤーが";
	auto tag = colider.GetTag();
	switch (tag)
	{
	case Game::e_GameObjectTag::kItemHp:
		message += "アイテムHp";
		m_hp += 1;
		break;
	case Game::e_GameObjectTag::kBoss:
		message += "ボス";
		break;
	//case Game::e_GameObjectTag:::
	//	message += "足場";
	//	break;
	default:
		break;
	}
	message += "と当たった！\n";
	printfDx(message.c_str());
}

const VECTOR& Player::GetPos() const
{
	return m_rigidbody.GetPos();
}

void Player::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void Player::IdleUpdate()
{	

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
		//OnAttackX();
		OnHit();
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

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(kPadButtonLStick))
	{
		OnFaceUse();
		return;
	}

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

	m_bossToPlayerVec = VNorm(m_bossToPlayerVec);
	m_angle = atan2f(m_bossToPlayerVec.x, m_bossToPlayerVec.z);


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

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(kPadButtonLStick))
	{
		OnFaceUse();
		return;
	}

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

	m_stamina += kStaminaIncreaseSpeed;

	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);

	//移動中に足音のSEを鳴らす
	//キャラクターの種類によって変える
	if (len != 0.0f)
	{
		if (m_moveCount % 25 == 0)
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

	//動いている間
	if (len != 0.0f)
	{

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kPowerMinSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
			}
			if (rate >= 0.6f)
			{
				speed = kPowerMaxSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
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
				//move = VTransform(move, m_playerRotMtx);
			}
			if (rate >= 0.6f)
			{
				speed = kNormalMaxSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
			}
		}

		//カメラのいる場所(角度)から
		//コントローラーによる移動方向を決定する
		MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
		move = VTransform(move, mtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_move = move;

		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

	}
	//動かなかったらアイドル状態へ
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

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(kPadButtonLStick))
	{
		OnFaceUse();
		return;
	}

	//顔を選択する関数
	FaceSelect();
}

void Player::LockOnWalkUpdate()
{
	m_bossToPlayerVec = VNorm(m_bossToPlayerVec);
	m_angle = atan2f(m_bossToPlayerVec.x, m_bossToPlayerVec.z);

	m_stamina += kStaminaIncreaseSpeed;

	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);

	//移動中に足音のSEを鳴らす
	//キャラクターの種類によって変える
	if (len != 0.0f)
	{
		if (m_moveCount % 25 == 0)
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

	//動いている間
	if (len != 0.0f)
	{

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
		{
			//スティックの押し加減でプレイヤーのスピードを変える
			if (rate <= 0.6f && rate > 0.0f);
			{
				speed = kPowerMinSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
			}
			if (rate >= 0.6f)
			{
				speed = kPowerMaxSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
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
				//move = VTransform(move, m_playerRotMtx);
			}
			if (rate >= 0.6f)
			{
				speed = kNormalMaxSpeed * rate;
				move = VScale(move, speed);
				//move = VTransform(move, m_playerRotMtx);
			}
		}

		//カメラのいる場所(角度)から
		//コントローラーによる移動方向を決定する
		MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
		move = VTransform(move, mtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_move = move;

		//m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

	}
	//動かなかったらアイドル状態へ
	else
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

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(kPadButtonLStick))
	{
		OnFaceUse();
		return;
	}

	//顔を選択する関数
	FaceSelect();
}

void Player::DashUpdate()
{
	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);

	//移動中に足音のSEを鳴らす
	//キャラクターの種類によって変える
	if (len != 0.0f)
	{
		if (m_moveCount % 15 == 0)
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

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_stamina -= 0.5f;


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

		////カメラの角度によって進む方向を変える
		//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());
		////移動ベクトルとカメラ角度行列を乗算
		//move = VTransform(move, playerRotMtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;

	}

	//地面についていなかったら
	if (!GetIsJump())
	{
		OnFall();
		return;
	}

	//歩き
	if (Pad::IsRelase(kPadButtonA) || VSquareSize(move) == 0.0f || m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;

		if (m_isLockOn)
		{
			OnLockOnWalk();
		}
		else
		{
			OnWalk();
		}
	}

	//攻撃X
	if (Pad::IsTrigger(kPadButtonX) && !m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackX();
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

	//ジャンプ
	if (Pad::IsTrigger(kPadButtonB) && GetIsJump() && !m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnDashJump();
	}

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(kPadButtonLStick))
	{
		OnFaceUse();
		return;
	}

	//顔を選択する関数
	FaceSelect();
}

void Player::JumpUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;
	
	m_jumpCount++;

	//if (!m_isFaceUse)
	{
		auto vel = m_rigidbody.GetVelocity();
		//vel.y += kMinJumpPower;

		if (m_jumpCount < 6)
		{
			vel.y += kMaxJumpPower;
			m_jumpPower += kMaxJumpPower;
		}
		else
		{
			vel.y += kMinJumpPower;
			m_jumpPower += kMinJumpPower;
		}

		if (m_jumpPower > 4)
		{
			OnAir();
		}

		m_rigidbody.SetVelocity(vel);
	}

	if (m_jumpCount > 30)
	{
		OnFall();
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
	m_stamina += kStaminaIncreaseSpeed;

	m_jumpCount++;
	//m_frame++;

	//if (!m_isFaceUse)
	{
		auto vel = m_rigidbody.GetVelocity();
		//vel.y += kMinJumpPower;

		if (m_jumpCount < 6)
		{
			vel.y += kMaxJumpPower;
			m_jumpPower += kMaxJumpPower;
		}
		else
		{
			vel.y += kMinJumpPower;
			m_jumpPower += kMinJumpPower;
		}

		if (m_jumpPower > 4)
		{
			OnDashAir();
		}

		m_rigidbody.SetVelocity(vel);
	}

	if (m_jumpCount > 30)
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
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::DashFallUpdate()
{
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
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::AttackCharge()
{
	m_stamina += kStaminaIncreaseSpeed;

	m_chargeTime++;


	if (m_chargeTime == 60)
	{
		SoundManager::GetInstance().PlaySe("healHpSe");
	}

	//if (m_chargeTime > 60)
	//{
	//	//エフェクトを入れてチャージされたかを確認できるようにする
	//	//後音も入れる
	//}
	//else
	//{
	//	if (m_chargeTime % 30 == 0)
	//	{
	//		SoundManager::GetInstance().PlaySe("attackChargeSe");
	//	}
	//}

	//Yボタンを離した場合
	if (Pad::IsRelase(kPadButtonY) && m_chargeTime > 60)
	{
		m_chargeTime = 0;

		m_isUseMp = true;
		OnAttackY();
	}
	else if(Pad::IsRelase(kPadButtonY) && m_chargeTime < 60)
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
	m_stamina += kStaminaIncreaseSpeed;
	auto pos = m_rigidbody.GetPos();
	EffectManager::GetInstance().CreateEffect("hitEffect", pos);

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
	m_stamina += kStaminaIncreaseSpeed;

	if (m_isUseMp)
	{
		m_mp -= kMpDiminishNum;
		m_isUseMp = false;
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

void Player::HitUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;
	m_damageFrame++;

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop() && m_damageFrame == 120)
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

	if (!m_isDead && m_deadTime > 30)
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
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//モデルの描画
		MV1DrawModel(m_modelPowerH);
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//モデルの描画
		MV1DrawModel(m_modelSpeedH);
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//モデルの描画
		MV1DrawModel(m_modelShotH);
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
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

void Player::OnIdle()
{
	m_jumpCount = 0;
	m_frame = 0;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));


	//タイプによってアニメーションを変える
	AnimChange(kAnimIdle, kAnimIdle, kAnimIdle, kAnimIdle);

	m_updateFunc = &Player::IdleUpdate;
}

void Player::OnLockOnIdle()
{
	m_jumpCount = 0;
	m_frame = 0;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));


	//タイプによってアニメーションを変える
	AnimChange(kAnimIdle, kAnimIdle, kAnimIdle, kAnimIdle);

	m_updateFunc = &Player::LockOnIdleUpdate;
}

void Player::OnWalk()
{
	//タイプによってアニメーションを変える
	AnimChange(kAnimWalk, kAnimWalk, kAnimWalk, kAnimWalk);

	m_updateFunc = &Player::WalkUpdate;
}

void Player::OnLockOnWalk()
{
	//タイプによってアニメーションを変える
	AnimChange(kAnimLockOnWalk, kAnimLockOnWalk, kAnimLockOnWalk, kAnimLockOnWalk);

	m_updateFunc = &Player::LockOnWalkUpdate;
}

void Player::OnDash()
{
	//タイプによってアニメーションを変える
	AnimChange(kAnimDash, kAnimDash, kAnimDash, kAnimDash);

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kAbutton;
	m_updateFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	auto pos = m_rigidbody.GetPos();
	//EffectManager::GetInstance().CreateEffect("hitEffect", pos);

	//タイプによって攻撃アニメーションを変える
	switch (m_multiAttack)
	{
	case 0:
		//一番目の攻撃アニメーション
		AnimChange(kAnimAttackX_First, kAnimAttackX_First, kAnimAttackX_First, kAnimAttackX_First);
		SoundManager::GetInstance().PlaySe("attackFirstSe");

		break;
	case 1:
		//二番目の攻撃アニメーション
		AnimChange(kAnimAttackX_Second, kAnimAttackX_Second, kAnimAttackX_Second, kAnimAttackX_Second);
		SoundManager::GetInstance().PlaySe("attackSecondSe");
		break;
	case 2:
		//三番目の攻撃アニメーション
		AnimChange(kAnimAttackX_Third, kAnimAttackX_Third, kAnimAttackX_Third, kAnimAttackX_Third);
		SoundManager::GetInstance().PlaySe("attackFirstSe");
		break;
	case 3:
		//四番目の攻撃アニメーション
		AnimChange(kAnimAttackX_Fourth, kAnimAttackX_Fourth, kAnimAttackX_Fourth, kAnimAttackX_Fourth);
		SoundManager::GetInstance().PlaySe("attackThirdSe");
		break;
	default:
		break;
	}

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kXbutton;

	m_updateFunc = &Player::AttackXUpdate;
}

void Player::OnAttackY()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//タイプによってアニメーションを変える
	AnimChange(kAnimAttackY, kAnimAttackY, kAnimAttackY, kAnimAttackY);

	SoundManager::GetInstance().PlaySe("attackYSe");


	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;

	m_updateFunc = &Player::AttackYUpdate;
}

void Player::OnJump()
{
	SoundManager::GetInstance().PlaySe("jumpSe");

	//地面と接触しているかどうか
	auto vel = m_rigidbody.GetVelocity();

	m_jumpCount = 0;
	m_jumpPower = 0;

	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kBbutton;

	m_updateFunc = &Player::JumpUpdate;

}

void Player::OnDashJump()
{
	SoundManager::GetInstance().PlaySe("jumpSe");

	m_stamina -= 20;

	//地面と接触しているかどうか
	auto vel = m_rigidbody.GetVelocity();

	m_jumpCount = 0;
	m_jumpPower = 0;

	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kBbutton;

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

	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::FallUpdate;
}

void Player::OnDashFall()
{

	m_jumpCount = 0;

	m_pAnim->ChangeAnim(kAnimFall);
	m_updateFunc = &Player::DashFallUpdate;
}

void Player::OnAttackCharge()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	
	SoundManager::GetInstance().PlaySe("attackChargeSe");

	//タイプによってアニメーションを変える
	AnimChange(kAnimAttackCharge, kAnimAttackCharge, kAnimAttackCharge, kAnimAttackCharge);

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;

	m_updateFunc = &Player::AttackCharge;

}

void Player::OnHit()
{
	SoundManager::GetInstance().PlaySe("damageSe");
	m_hp -= 1;

	m_pAnim->ChangeAnim(kAnimHit);
	//m_pAnim->ChangeAnim(kAnimHit, false, true, true);
	m_updateFunc = &Player::HitUpdate;
}

void Player::OnDead()
{
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
	m_pAnim->ChangeAnim(kAnimUseFace);
	m_updateFunc = &Player::FaceUseUpdate;
}

void Player::OnTalk()
{
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updateFunc = &Player::TalkUpdate;
}

void Player::FaceSelect()
{
	//顔を選択
	if (Pad::IsTrigger(kPadButtonRB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kRassPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) + 1);
		}
		else if (m_playerKind == e_PlayerKind::kRassPlayer)
		{
			m_playerKind = e_PlayerKind::kPowerPlayer;
		}
	}
	//顔を選択
	if (Pad::IsTrigger(kPadButtonLB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kPowerPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) - 1);
		}
		else if (m_playerKind == e_PlayerKind::kPowerPlayer)
		{
			m_playerKind = e_PlayerKind::kRassPlayer;
		}
	}
}

void Player::AnimChange(const char* normal, const char* power, const char* speed, const char* shot)
{
	//プレイヤーのタイプでアニメーションを変える
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(power);
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(speed);
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(shot);
	}
	else if (m_playerKind == e_PlayerKind::kRassPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(normal);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(normal);
	}
}

const float& Player::GetRadius() const
{
	return m_radius;
}

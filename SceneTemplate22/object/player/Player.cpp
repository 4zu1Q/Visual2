#include "Player.h"
#include "PlayerWeapon.h"
#include "object/weapon/WeaponBase.h"

#include "object/Camera.h"

#include "util/Pad.h"
#include "util/AnimController.h"


#include "myLib/MyLib.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kModelFilename = "Data/Model/Player/Player.mv1";

	const char* const kSwordModelFileName = "Data/Model/Weapon/Player_Sword.mv1";


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

	//最小移動速度
	constexpr float kNormalMinSpeed = 0.5f;
	constexpr float kPowerMinSpeed = 0.3f;
	constexpr float kSpeedMinSpeed = 0.7f;
	constexpr float kShotMinSpeed = 0.4f;

	//最大移動速度
	constexpr float kNormalMaxSpeed = 0.7f;
	constexpr float kPowerMaxSpeed = 0.5f;
	constexpr float kSpeedMaxSpeed = 0.9f;
	constexpr float kShotMaxSpeed = 0.6f;

	//ダッシュスピード
	constexpr float kNormalDashSpeed = 1.0f;
	constexpr float kPowerDashSpeed = 0.7f;
	constexpr float kSpeedDashSpeed = 1.5f;
	constexpr float kShotDashSpeed = 1.1f;
	constexpr float kStrongestDashSpeed = 1.2f;

	//ジャンプ力
	constexpr float kNormalJumpPower = 6.0f;
	constexpr float kPowerJumpPower = 3.0f;
	constexpr float kSpeedJumpPower = 9.0f;
	constexpr float kShotJumpPower = 5.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };
	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,12.0f,0.0f };

	/*　プレイヤーのアニメーションの種類　*/
	const char* const kAnimInfoFilename = "Data/Master/AnimPlayerMaster.csv";

	//スポーン
	const char* const kAnimSpawn = "Spawn";

	//アイドル状態
	const char* const kAnimNormalIdle = "Idle";
	const char* const kAnimPowerIdle = "PowerIdle";
	const char* const kAnimSpeedIdle = "SpeedIdle";

	//歩き状態
	const char* const kAnimNormalWalk = "Walk";
	const char* const kAnimPowerWalk = "PowerWalk";
	const char* const kAnimSpeedWalk = "SpeedWalk";

	//ダッシュ状態
	const char* const kAnimNormalDash = "NormalDash";
	const char* const kAnimPowerDash = "PowerDash";
	const char* const kAnimSpeedDash = "SpeedDash";

	//Y攻撃をするためのチャージアニメーション
	const char* const kAnimAttackCharge = "AttackCharge";
	const char* const kAnimShotAttackCharge = "ShotAttackCharge";

	//通常時の攻撃アニメーション
	const char* const kAnimNormalAttackX_First = "NormalAttackX_First";
	const char* const kAnimNormalAttackX_Second = "NormalAttackX_Second";
	const char* const kAnimNormalAttackX_Third = "NormalAttackX_Third";
	const char* const kAnimNormalAttackX_Fourth = "NormalAttackX_Fourth";

	const char* const kAnimNormalAttackY = "NormalAttackY";

	//パワー型の攻撃アニメーション
	const char* const kAnimPowerAttackX_First = "PowerAttackX_First";
	const char* const kAnimPowerAttackX_Second = "PowerAttackX_Second";
	const char* const kAnimPowerAttackX_Third = "PowerAttackX_Third";
	const char* const kAnimPowerAttackX_Fourth = "PowerAttackX_Fourth";

	const char* const kAnimPowerAttackY = "PowerAttackY";

	//スピード型の攻撃アニメーション
	const char* const kAnimSpeedAttackX_First = "SpeedAttackX_First";
	const char* const kAnimSpeedAttackX_Second = "SpeedAttackX_Second";
	const char* const kAnimSpeedAttackX_Third = "SpeedAttackX_Third";
	const char* const kAnimSpeedAttackX_Fourth = "SpeedAttackX_Fourth";

	const char* const kAnimSpeedAttackY = "SpeedAttackY";

	//ショット型の攻撃アニメーション
	const char* const kAnimShotAttackX_First = "ShotAttackX_First";
	const char* const kAnimShotAttackX_Second = "ShotAttackX_Second";
	const char* const kAnimShotAttackX_Third = "ShotAttackX_Third";
	const char* const kAnimShotAttackX_Fourth = "ShotAttackX_Fourth";

	const char* const kAnimShotAttackY = "ShotAttackY";

	//ジャンプアニメーション
	const char* const kAnimJump = "Jump";
	const char* const kAnimAir = "Jumping";

	//ダメージとダウンアニメーション
	const char* const kAnimHit = "Hit";
	const char* const kAnimDead = "Down";

	//顔の使用アニメーション
	const char* const kAnimUseFace = "UseFace";


	//パッドのボタン情報
	constexpr int kPadButtonA = 0x00000010;
	constexpr int kPadButtonB = 0x00000020;
	constexpr int kPadButtonX = 0x00000040;
	constexpr int kPadButtonY = 0x00000080;

	constexpr int kPadButtonRB = 0x00000200;
	constexpr int kPadButtonLB = 0x00000100;

	constexpr int kPadButtonLStick = 0x00001000;

	//最大値
	constexpr float kMaxHp = 10.0f;
	constexpr float kMaxMp = 300.0f;
	constexpr float kMaxStamina = 300.0f;

	//スタミナの減るスピード
	constexpr float kStaminaDiminishSpeed = 0.5f;
	constexpr float kStaminaIncreaseSpeed = 0.4f;
	constexpr float kStaminaActionIncreaseSpeed = 0.2f;

	//MPの減る値
	constexpr float kMpDiminishNum = 30.0f;
	constexpr float kMpIncreaseNum = 40.0f;


}

Player::Player() :
	CharaBase(Collidable::e_Priority::kHigh, Game::e_GameObjectTag::kPlayer, MyLib::ColliderData::e_Kind::kSphere, false),
	m_isGameOver(false),
	m_pos(VGet(0,0,0)),
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
	m_chargeTime(0)
{

#ifdef _DEBUG
	m_isPowerFace = true;
	m_isSpeedFace = true;
	m_isShotFace = true;
	m_isStrongestFace = true;
#else

	m_isPowerFace = false;
	m_isSpeedFace = false;
	m_isShotFace = false;
	m_isStrongestFace = false;

#endif

	m_isJump = false;

	//モデルのロード
	m_modelH = MV1LoadModel(kModelFilename);
	assert(m_modelH > -1);

	m_weaponH = MV1LoadModel(kSwordModelFileName);

	//マスク関連の初期化
	m_isFaceUse = false;
	m_playerKind = e_PlayerKind::kPowerPlayer;
	
	//ボタンUI関連
	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;

	//
	m_pWeapon = std::make_shared<PlayerWeapon>();
	m_pAnim = std::make_shared<AnimController>();

	m_pWeaponBase = std::make_shared<WeaponBase>();
	m_pWeaponBase->Initialize(m_weaponH, m_modelH, "handslot.r", "handslot.l", kWeaponScale);

	//m_pCamera = std::make_shared<Camera>();

	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 5.0f;


	//m_pColliderData = std::make_shared<MyLib::ColliderDataCapsule>(false);

	//auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataCapsule>(m_pColliderData);
	//circleColliderData->m_radius = 2.0f;
	//circleColliderData->m_posDown = VGet(0,0,0);
	//circleColliderData->m_posUp = VGet(0,8,0);
}

Player::~Player()
{

}

void Player::Initialize(std::shared_ptr<MyLib::Physics> physics, VECTOR pos)
{

	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(pos);
	m_speed = 0.1f;

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//武器の初期化
	m_pWeapon->Load();
	m_pWeapon->Initialize(m_modelH, kRightModelFrameNo, kLeftModelFrameNo);

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimNormalIdle);

	// メンバ関数ポインタの初期化
	m_updateFunc = &Player::IdleUpdate;

}

void Player::Finalize(std::shared_ptr<MyLib::Physics> physics)
{

	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	Collidable::Finalize(physics);
}

void Player::Update(std::shared_ptr<MyLib::Physics> physics)
{

	//アップデート
	(this->*m_updateFunc)();

	/*フレームにアタッチするための更新処理*/
	m_pWeapon->SwordUpdate();
	m_pWeapon->AxeUpdate();
	m_pWeapon->DaggerUpdate();
	m_pWeapon->MagicWandUpdate();
	m_pWeapon->LongSwordUpdate();
		
	//SetCameraAngle(m_pCamera->GetAngle());

	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	m_pos = m_rigidbody.GetPos();

	//m_pCamera->DebugUpdate(pos);

	//カメラの更新
	//m_pCamera->Update(pos);
	//m_cameraToPlayerVec = VSub(pos, m_pCamera->GetNextPos());

	m_posUp = VGet(m_pos.x, m_pos.y + kUpPos.y, m_pos.z);

	//モデルのポジションを合わせるよう
	//VECTOR modelPos = VGet(m_pos.x, m_pos.y, m_pos.z);


	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_pos);

	//モデルに回転をセットする
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));


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

void Player::Draw()
{
	//武器の描画
	WeaponDraw();

	//モデルの描画
	MV1DrawModel(m_modelH);
	//MV1DrawModel(m_weaponH);

	//m_pWeaponBase->Draw();

#ifdef _DEBUG

	//auto pos = m_rigidbody.GetPos();
	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(0, 148, 0xff0fff, "playerHp:%f,playerMp:%f,playerStamina:%f", m_hp, m_mp, m_stamina);


	//DrawFormatString(0, 80, 0x000fff, " PlayerKind : %d ", m_playerKind);
#endif

	//m_pCamera->Draw();

}

//void Player::OnCollide(const Collidable& colider)
//{
//	std::string message = "プレイヤーが";
//	auto tag = colider.GetTag();
//	switch (tag)
//	{
//	case Game::e_GameObjectTag::kItemHp:
//		message += "アイテムHp";
//		m_hp += 1;
//		break;
//	case Game::e_GameObjectTag::kBoss:
//		message += "ボス";
//		break;
//	case Game::e_GameObjectTag::kStepGround:
//		message += "足場";
//		break;
//	default:
//		break;
//	}
//	message += "と当たった！\n";
//	printfDx(message.c_str());
//}

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

	if (VSquareSize(input) != 0.0f)
	{
		OnWalk();
		return;
	}

	if (Pad::IsTrigger(kPadButtonB) && !m_isJump && !m_isStamina)
	{
		OnJump();
		return;
	}

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

	VECTOR move;
	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(VGet(0, move.y, 0));

	//顔を選択する関数
	FaceSelect();

}

void Player::WalkUpdate()
{
	// カメラの角度によって進む方向を変える
	//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	m_stamina += kStaminaIncreaseSpeed;

	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);

	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	float speed = 0;

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
		if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse)
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

		////カメラの角度によって進む方向を変える
		//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());
		////移動ベクトルとカメラ角度行列を乗算
		//move = VTransform(move, playerRotMtx);

		move.y = m_rigidbody.GetVelocity().y;
		m_rigidbody.SetVelocity(move);

		m_move = move;

		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		//m_attackDir = VNorm(move);
		//m_avoid = VNorm(move);

	}
	//動かなかったらアイドル状態へ
	else
	{
		OnIdle();
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
	if (Pad::IsTrigger(kPadButtonB) && !m_isJump && !m_isStamina)
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
	// カメラの角度によって進む方向を変える
	//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);

	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	float speed = 0;


	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_stamina -= 0.5f;


		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		//プレイヤーのタイプで攻撃アニメーションを変える
		if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kPowerDashSpeed * rate;
		}
		else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kSpeedDashSpeed * rate;
		}
		else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
		{
			//ダッシュ
			speed = kShotDashSpeed * rate;
		}
		else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
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
		//m_attackDir = VNorm(move);
		//m_avoid = VNorm(move);

	}

	//歩き
	if (Pad::IsRelase(kPadButtonA) || VSquareSize(move) == 0.0f || m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnWalk();
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
	if (Pad::IsTrigger(kPadButtonB) && !m_isJump && !m_isStamina)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnJump();
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

	// カメラの角度によって進む方向を変える
	//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	if (m_pAnim->IsLoop())
	{
		OnAir();
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

	//ダッシュ
	float speed = kNormalDashSpeed * rate;
	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);



	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		//m_attackDir = VNorm(move);
		//m_avoid = VNorm(move);
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);

	//if (Pad::IsTrigger(kPadButtonB) && !m_isJump)
	//{
	//	OnJump();
	//}

}

void Player::AirUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	// カメラの角度によって進む方向を変える
	//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	m_frame++;

	if (m_frame > 6)
	{
		m_frame = 0;
		m_isJump = false;
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnIdle();
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

	//ダッシュ
	float speed = kNormalDashSpeed * rate;
	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);
	}

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::AttackCharge()
{
	m_stamina += kStaminaIncreaseSpeed;

	m_chargeTime++;

	if (m_chargeTime > 60)
	{
		//エフェクトを入れてチャージされたかを確認できるようにする
		//後音も入れる
	}

	//Yボタンを離した場合
	if (Pad::IsRelase(kPadButtonY) && m_chargeTime > 60)
	{
		m_isUseMp = true;
		m_chargeTime = 0;
		OnAttackY();
	}
	else if(Pad::IsRelase(kPadButtonY) && m_chargeTime < 60)
	{
		m_chargeTime = 0;
		OnIdle();
	}
}

void Player::AttackXUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	//攻撃ボタンが押されたとき
	if (Pad::IsTrigger(PAD_INPUT_3) && !m_isNextAttackFlag)
	{
		m_isNextAttackFlag = true;
	}

	//アニメーションが終わった段階で次の攻撃フラグがたっていなかったら
	if (m_pAnim->IsLoop() && !m_isNextAttackFlag)
	{
		m_multiAttack = 0;
		//アイドル状態に戻る
		OnIdle();
	}

	//アニメーションが終わった段階で次の攻撃フラグがたっていたら
	if (m_pAnim->IsLoop() && m_isNextAttackFlag)
	{
		m_isNextAttackFlag = false;
		m_multiAttack++;
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
		OnIdle();
	}

}

void Player::HitUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void Player::DeadUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	//ゲームオーバーシーンに遷移するためにフラグを立てる
	m_isGameOver = true;

	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;

}

void Player::SpawnUpdate()
{
	//セレクトシーンの始めに
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void Player::FaceUseUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	if (m_pAnim->IsLoop())
	{
		m_isFaceUse = !m_isFaceUse;
		OnIdle();
	}
}

void Player::TalkUpdate()
{
	m_stamina += kStaminaIncreaseSpeed;

	//会話するときの処理を入れる

}

void Player::WeaponDraw()
{
	//顔を付けている場合
	if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->AxeDraw();
	}
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->DaggerDraw();
	}
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->MagicWandDraw();
	}
	else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->LongSwordDraw();
	}

	//顔を付けていない場合
	if (!m_isFaceUse)
	{
		//武器をアタッチする描画関数
		m_pWeapon->SwordDraw();
	}
}

void Player::OnIdle()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//タイプによってアニメーションを変える
	AnimChange(kAnimNormalIdle, kAnimPowerIdle, kAnimSpeedIdle, kAnimNormalIdle);

	m_updateFunc = &Player::IdleUpdate;
}

void Player::OnWalk()
{
	//タイプによってアニメーションを変える
	AnimChange(kAnimNormalWalk, kAnimPowerWalk, kAnimSpeedWalk, kAnimNormalWalk);

	m_updateFunc = &Player::WalkUpdate;
}

void Player::OnDash()
{
	//タイプによってアニメーションを変える
	AnimChange(kAnimNormalDash, kAnimPowerDash, kAnimSpeedDash, kAnimNormalDash);

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kAbutton;
	m_updateFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{

	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//タイプによって攻撃アニメーションを変える
	switch (m_multiAttack)
	{
	case 0:
		//一番目の攻撃アニメーション
		AnimChange(kAnimNormalAttackX_First, kAnimPowerAttackX_First, kAnimSpeedAttackX_First, kAnimShotAttackX_First);
		break;
	case 1:
		//二番目の攻撃アニメーション
		AnimChange(kAnimNormalAttackX_Second, kAnimPowerAttackX_Second, kAnimSpeedAttackX_Second, kAnimShotAttackX_Second);
		break;
	case 2:
		//三番目の攻撃アニメーション
		AnimChange(kAnimNormalAttackX_Third, kAnimPowerAttackX_Third, kAnimSpeedAttackX_Third, kAnimShotAttackX_Third);
		break;
	case 3:
		//四番目の攻撃アニメーション
		AnimChange(kAnimNormalAttackX_Fourth, kAnimPowerAttackX_Fourth, kAnimSpeedAttackX_Fourth, kAnimShotAttackX_Fourth);
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
	AnimChange(kAnimNormalAttackY, kAnimPowerAttackY, kAnimSpeedAttackY, kAnimShotAttackY);

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;

	m_updateFunc = &Player::AttackYUpdate;
}

void Player::OnJump()
{
	//地面と接触しているかどうか
	m_isJump = true;
	auto vel = m_rigidbody.GetVelocity();

	//プレイヤーのタイプでジャンプ力を変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		vel.y = kPowerJumpPower;

	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		vel.y = kSpeedJumpPower;
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		vel.y = kShotJumpPower;
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		vel.y = kNormalJumpPower;
	}

	if (!m_isFaceUse)
	{
		vel.y = kNormalJumpPower;
	}

	m_rigidbody.SetVelocity(vel);

	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kBbutton;

	m_updateFunc = &Player::JumpUpdate;

}

void Player::OnAir()
{
	m_pAnim->ChangeAnim(kAnimAir);
	m_updateFunc = &Player::AirUpdate;
}

void Player::OnAttackCharge()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//タイプによってアニメーションを変える
	AnimChange(kAnimAttackCharge, kAnimAttackCharge, kAnimAttackCharge, kAnimShotAttackCharge);

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kYbutton;

	m_updateFunc = &Player::AttackCharge;

}

void Player::OnHit()
{
	m_pAnim->ChangeAnim(kAnimJump);
	m_updateFunc = &Player::HitUpdate;
}

void Player::OnDead()
{
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updateFunc = &Player::DeadUpdate;
}

void Player::OnSpawn()
{
	m_pAnim->ChangeAnim(kAnimSpawn);
	m_updateFunc = &Player::SpawnUpdate;
}

void Player::OnFaceUse()
{
	m_pAnim->ChangeAnim(kAnimUseFace);
	m_updateFunc = &Player::FaceUseUpdate;
}

void Player::OnTalk()
{
	m_pAnim->ChangeAnim(kAnimNormalIdle);
	m_updateFunc = &Player::TalkUpdate;
}

void Player::FaceSelect()
{
	//顔を選択
	if (Pad::IsTrigger(kPadButtonRB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kStrongestPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) + 1);
		}
	}
	//顔を選択
	if (Pad::IsTrigger(kPadButtonLB) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kPowerPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) - 1);
		}
	}
}

void Player::CameraUpdate()
{
	//カメラ更新
}

void Player::AnimChange(const char* normal, const char* power, const char* speed, const char* shot)
{
	//プレイヤーのタイプでアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(power);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(speed);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(shot);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(normal);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(normal);
	}
}


#include "Player.h"
#include "PlayerWeapon.h"
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

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;

	//プレイヤーの手のフレーム番号
	constexpr int kRightModelFrameNo = 14;
	constexpr int kLeftModelFrameNo = 9;

	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.1f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	//移動速度
	constexpr float kMaxSpeedN = 0.7f;
	constexpr float kMinSpeedN = 0.5f;

	//ダッシュスピード
	constexpr float kNormalDashSpeed = 1.0f;
	constexpr float kPowerDashSpeed = 0.4f;
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
	constexpr VECTOR kUpPos = { 0.0f,18.0f,0.0f };

	/*プレイヤーのアニメーションの種類*/
	const char* const kAnimInfoFilename = "Data/Master/AnimPlayerMaster.csv";

	const char* const kAnimSpawn = "Spawn";

	const char* const kNormalAnimIdle = "Idle";
	const char* const kPowerAnimIdle = "PowerIdle";
	const char* const kSpeedAnimIdle = "SpeedIdle";

	const char* const kNormalAnimWalk = "Walk";
	const char* const kPowerAnimWalk = "PowerWalk";
	const char* const kSpeedAnimWalk = "SpeedWalk";

	const char* const kAnimNormalDash = "NormalDash";
	const char* const kAnimPowerDash = "PowerDash";
	const char* const kAnimSpeedDash = "SpeedDash";

	const char* const kAnimAttackCharge = "AttackCharge";

	const char* const kAnimNormalAttackX = "NormalAttackX";
	const char* const kAnimNormalAttackY = "NormalAttackY";

	const char* const kAnimPowerAttackX = "PowerAttackX";
	const char* const kAnimPowerAttackY = "PowerAttackY";

	const char* const kAnimSpeedAttackX = "SpeedAttackX";
	const char* const kAnimSpeedAttackY = "SpeedAttackY";

	const char* const kAnimShotAttackX = "ShotAttackX";
	const char* const kAnimShotAttackY = "ShotAttackY";

	const char* const kAnimJump = "Jump";
	const char* const kAnimAir = "Jumping";

	const char* const kAnimHit = "Hit";

	const char* const kAnimDead = "Down";

	const char* const kAnimUseFace = "UseFace";


}

Player::Player() :
	CharaBase(Collidable::e_Priority::kHigh, Game::e_GameObjectTag::kPlayer, MyLib::ColliderData::e_Kind::kCapsule, false),
	m_weaponH(-1),
	m_radius(2),
	m_posUp(kInitPos),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_cameraToPlayerVec(VGet(0,0,0)),
	m_cameraDirection(VGet(0,0,0)),
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(10),
	m_cameraAngle(0.0f),
	m_rate(0.0f),
	m_isDead(false),
	m_frame(0),
	m_isMove(false)
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

	//マスク関連の初期化
	m_isFaceUse = false;
	m_playerKind = e_PlayerKind::kPowerPlayer;
	
	//ボタンUI関連
	m_isButtonPush = false;
	m_buttonKind = e_ButtonKind::kNone;

	//
	m_pWeapon = std::make_shared<PlayerWeapon>();
	m_pAnim = std::make_shared<AnimController>();


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
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kNormalAnimIdle);


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

	//カプセル用のポジション
	auto pos = m_rigidbody.GetPos();

	//m_pCamera->DebugUpdate(pos);

	//カメラの更新
	//m_pCamera->Update(pos);
	//m_cameraToPlayerVec = VSub(pos, m_pCamera->GetNextPos());

	m_posUp = VGet(pos.x, pos.y + kUpPos.y, pos.z);

	//モデルのポジションを合わせるよう
	VECTOR modelPos = VGet(pos.x, pos.y, pos.z);


	//モデルに座標をセットする
	MV1SetPosition(m_modelH, modelPos);

	//モデルに回転をセットする
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;

		OnDead();
	}
}

void Player::Draw()
{
	//武器の描画
	WeaponDraw();

	//モデルの描画
	MV1DrawModel(m_modelH);
	MV1DrawModel(m_weaponH);


#ifdef _DEBUG

	auto pos = m_rigidbody.GetPos();
	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", pos.x, pos.y, pos.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

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

const VECTOR& Player::GetPosDown() const
{
	return m_rigidbody.GetPos();
}

void Player::SetPosDown(const VECTOR pos)
{
	m_rigidbody.SetPos(pos);
}

void Player::IdleUpdate()
{

	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR input = VGet(m_analogX, 0.0f, -m_analogZ);

	if (VSquareSize(input) != 0.0f)
	{
		OnWalk();
		return;
	}

	if (Pad::IsTrigger(PAD_INPUT_2) && !m_isJump)
	{
		OnJump();
		return;
	}

	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		OnAttackX();
		return;
	}

	if (Pad::IsPress(PAD_INPUT_4))
	{
		//OnAttackY();
		OnAttackCharge();
		return;
	}

	//攻撃Y
	//if (Pad::IsPress(PAD_INPUT_4) && !m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	//OnAttackY();
	//	OnAttackCharge();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackCharge();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackY();
	//}


	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(PAD_INPUT_9))
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

	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);

	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//動いている間
	if (len != 0.0f)
	{

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		//スティックの押し加減でプレイヤーのスピードを変える
		if (rate <= 0.6f && rate > 0.0f);
		{
			float speed = kMinSpeedN * rate;
			move = VScale(move, speed);
		}
		if (rate >= 0.6f)
		{
			float speed = kMaxSpeedN * rate;
			move = VScale(move, speed);
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
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);
	}
	//動かなかったらアイドル状態へ
	else
	{
		OnIdle();
		return;
	}

	//攻撃X
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		OnAttackX();
		return;
	}

	//攻撃Y
	if (Pad::IsPress(PAD_INPUT_4))
	{
		//OnAttackY();
		OnAttackCharge();
		return;
	}

	//if (Pad::IsPress(PAD_INPUT_4) && !m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	//OnAttackY();
	//	OnAttackCharge();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackY();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackY();
	//}

	//ダッシュ
	if (Pad::IsPress(PAD_INPUT_1))
	{
		OnDash();
		return;
	}

	//ジャンプ
	if (Pad::IsTrigger(PAD_INPUT_2) && !m_isJump)
	{
		OnJump();
		return;
	}

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(PAD_INPUT_9))
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
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);

	}

	//歩き
	if (Pad::IsRelase(PAD_INPUT_1) || VSquareSize(move) == 0.0f)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnWalk();
	}

	//攻撃X
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnAttackX();
	}

	//攻撃Y
	if (Pad::IsPress(PAD_INPUT_4))
	{
		//OnAttackY();
		OnAttackCharge();
		return;
	}

	//攻撃Y
	//if (Pad::IsPress(PAD_INPUT_4) && !m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	//OnAttackY();
	//	OnAttackCharge();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind != e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackY();
	//}
	////攻撃Y(スピードタイプのみショートカット)
	//else if (Pad::IsPress(PAD_INPUT_4) && m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	//{
	//	m_isButtonPush = false;
	//	m_buttonKind = e_ButtonKind::kNone;
	//	OnAttackY();
	//}

	//ジャンプ
	if (Pad::IsTrigger(PAD_INPUT_2) && !m_isJump)
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnJump();
	}

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(PAD_INPUT_9))
	{
		OnFaceUse();
		return;
	}

	//顔を選択する関数
	FaceSelect();
}

void Player::JumpUpdate()
{
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

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);
	}

	if (Pad::IsTrigger(PAD_INPUT_2) && !m_isJump)
	{
		OnJump();
	}

}

void Player::AirUpdate()
{
	// カメラの角度によって進む方向を変える
	//MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	m_frame++;



	if (m_frame > 30)
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
	m_chargeFrame++;

	//Yボタンを離した場合
	if (Pad::IsRelase(PAD_INPUT_4) && m_chargeFrame > 60)
	{
		m_chargeFrame = 0;
		OnAttackY();
	}
	else if(Pad::IsRelase(PAD_INPUT_4) && m_chargeFrame < 60)
	{
		m_chargeFrame = 0;
		OnIdle();
	}
}

void Player::AttackXUpdate()
{

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		m_isButtonPush = false;
		m_buttonKind = e_ButtonKind::kNone;
		OnIdle();
	}

}

void Player::AttackYUpdate()
{
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
	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}
}

void Player::DeadUpdate()
{
	//ゲームオーバーシーンに遷移するフラグをたてる
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
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	if (m_pAnim->IsLoop())
	{
		m_isFaceUse = !m_isFaceUse;
		OnIdle();
	}
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

	//プレイヤーのタイプでダッシュアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kPowerAnimIdle);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kSpeedAnimIdle);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimIdle);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimIdle);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimIdle);
	}

	m_updateFunc = &Player::IdleUpdate;
}

void Player::OnWalk()
{
	//プレイヤーのタイプでダッシュアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kPowerAnimWalk);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kSpeedAnimWalk);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimWalk);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimWalk);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kNormalAnimWalk);
	}

	m_updateFunc = &Player::WalkUpdate;
}

void Player::OnDash()
{
	//プレイヤーのタイプでダッシュアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimPowerDash);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimSpeedDash);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalDash);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalDash);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalDash);
	}

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kAbutton;
	m_updateFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{
	m_hp -= 1;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//プレイヤーのタイプで攻撃Xアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimPowerAttackX, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimSpeedAttackX, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimShotAttackX, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalAttackX, true, true, true);
	}
	
	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalAttackX, true, true, true);
	}

	m_isButtonPush = true;
	m_buttonKind = e_ButtonKind::kXbutton;

	m_updateFunc = &Player::AttackXUpdate;
}

void Player::OnAttackY()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));

	//プレイヤーのタイプで攻撃Yアニメーションを変える
	if (m_playerKind == Player::e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimPowerAttackY, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimSpeedAttackY, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kShotPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimShotAttackY, true, true, true);
	}
	else if (m_playerKind == Player::e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalAttackY, true, true, true);
	}

	if (!m_isFaceUse)
	{
		m_pAnim->ChangeAnim(kAnimNormalAttackY, true, true, true);
	}

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
	m_pAnim->ChangeAnim(kAnimAttackCharge);

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

void Player::FaceSelect()
{
	//顔を選択
	if (Pad::IsTrigger(PAD_INPUT_6) && !m_isFaceUse)
	{
		if (m_playerKind != e_PlayerKind::kStrongestPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) + 1);
		}
	}
	if (Pad::IsTrigger(PAD_INPUT_5) && !m_isFaceUse)
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


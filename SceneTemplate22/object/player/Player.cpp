#include "Player.h"
#include "PlayerWeapon.h"
#include "object/Camera.h"

#include "util/Pad.h"
#include "util/AnimController.h"

#include "ui/ButtonUi.h"

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
	constexpr float kDashSpeed = 1.0f;

	//ジャンプ力
	constexpr float kJumpPower = 6.0f;

	//初期位置
	constexpr VECTOR kInitPos = { 0.0f,0.0f,0.0f };

	//カプセルの上の座標
	constexpr VECTOR kUpPos = { 0.0f,8.0f,0.0f };

	/*プレイヤーのアニメーションの種類*/
	const char* const kAnimInfoFilename = "Data/Master/AnimPlayerMaster.csv";

	const char* const kAnimSpawn = "Spawn";
	const char* const kAnimIdle = "Idle";
	const char* const kAnimWalk = "Walk";
	const char* const kAnimDash = "Dash";

	const char* const kAnimNormalAttackX = "NormalAttackX";
	const char* const kAnimNormalAttackY = "NormalAttackY";

	const char* const kAnimPowerAttackX = "PowerAttackX";
	const char* const kAnimPowerAttackY = "PowerAttackY";

	const char* const kAnimSpeedAttackX = "SpeedAttackX";
	const char* const kAnimSpeedAttackY = "SpeedAttackY";
	
	const char* const kAnimJump = "Jump";
	const char* const kAnimAir = "Jumping";

	const char* const kAnimHit = "Hit";

	const char* const kAnimDead = "Down";
	const char* const kAnimDeadPose = "DownPose";


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
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(10),
	m_cameraAngle(0.0f),
	m_rate(0.0f),
	m_isDead(false),
	m_isMove(false)
{

	/*マスク関連の初期化*/
	m_isFaceUse = false;

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

	m_isAnimIdle = false;
	m_isAnimWalk = false;
	m_isAnimDash = false;
	m_isAnimAttackX = false;
	m_isAnimAttackY = false;
	m_isAnimDamage = false;
	m_isAnimDown = false;

	m_isJump = false;

	//モデルのロード
	m_modelH = MV1LoadModel(kModelFilename);
	assert(m_modelH > -1);

	m_playerKind = e_PlayerKind::kPowerPlayer;

	m_pWeapon = std::make_shared<PlayerWeapon>();
	m_pAnim = std::make_shared<AnimController>();

	//m_pButtonUi = std::make_shared<ButtonUi>();

	m_pCamera = std::make_shared<Camera>();

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
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimIdle);


	// メンバ関数ポインタの初期化
	m_updaFunc = &Player::IdleUpdate;

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
	(this->*m_updaFunc)();

	/*フレームにアタッチするための更新処理*/
	m_pWeapon->SwordUpdate();
	m_pWeapon->AxeUpdate();
	m_pWeapon->DaggerUpdate();
	m_pWeapon->MagicWandUpdate();
	m_pWeapon->LongSwordUpdate();
	
	SetCameraAngle(m_pCamera->GetAngle());


	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	//カプセル用のポジション
	auto pos = m_rigidbody.GetPos();

	m_pCamera->DebugUpdate(pos);

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

	m_pCamera->Draw();

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

	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		OnAttackY();
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
	MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

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
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		OnAttackY();
		return;
	}

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



	//顔を選択する関数
	FaceSelect();
}

void Player::DashUpdate()
{
	// カメラの角度によって進む方向を変える
	MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	//アナログスティックを取得
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);

	float len = VSize(move);

	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	rate = min(rate, 1.0f);
	rate = max(rate, 0.0f);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);

		//ダッシュ
		float speed = kDashSpeed * rate;
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
		OnWalk();
	}

	//攻撃X
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		OnAttackX();
	}

	//攻撃Y
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		OnAttackY();
	}

	//ジャンプ
	if (Pad::IsTrigger(PAD_INPUT_2) && !m_isJump)
	{
		OnJump();
	}

	//顔を選択する関数
	FaceSelect();
}

void Player::JumpUpdate()
{
	// カメラの角度によって進む方向を変える
	MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

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
	float speed = kDashSpeed * rate;
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
	MATRIX playerRotMtx = MGetRotY(m_pCamera->GetCameraAngleX());

	if (!m_isJump)
	{
		OnIdle();
	}

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
	float speed = kDashSpeed * rate;
	move = VScale(move, speed);

	//カメラのいる場所(角度)から
	//コントローラーによる移動方向を決定する
	MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
	move = VTransform(move, mtx);

	move.y = m_rigidbody.GetVelocity().y;
	m_rigidbody.SetVelocity(move);
}

void Player::AttackXUpdate()
{

	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}

}

void Player::AttackYUpdate()
{
	//アニメーションが終わったら待機状態に遷移
	if (m_pAnim->IsLoop())
	{
		
		//m_pButtonUi->SetButtonKind(ButtonUi::e_ButtonKind::kNone);
		//m_pButtonUi->SetIsButtonPush(false);
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

}

void Player::SpawnUpdate()
{
	//セレクトシーンの始めに
	if (m_pAnim->IsLoop())
	{
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
	m_pAnim->ChangeAnim(kAnimIdle);
	m_updaFunc = &Player::IdleUpdate;
}

void Player::OnWalk()
{
	m_pAnim->ChangeAnim(kAnimWalk);
	m_updaFunc = &Player::WalkUpdate;
}

void Player::OnDash()
{
	m_pAnim->ChangeAnim(kAnimDash);			//アニメーションの変更
//	m_pButtonUi->SetIsButtonPush(true);		//ボタンが押された
//	m_pButtonUi->SetButtonKind(ButtonUi::e_ButtonKind::kAbutton);	//どのボタンが押されたか
	m_updaFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{
	m_hp -= 1;
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_pAnim->ChangeAnim(kAnimNormalAttackX,true,true,true);
//	m_pButtonUi->SetIsButtonPush(true);		//ボタンが押された
//	m_pButtonUi->SetButtonKind(ButtonUi::e_ButtonKind::kXbutton);	//どのボタンが押されたか
	m_updaFunc = &Player::AttackXUpdate;
}

void Player::OnAttackY()
{
	m_rigidbody.SetVelocity(VGet(0, 0, 0));
	m_pAnim->ChangeAnim(kAnimNormalAttackY,true, true, true);
//	m_pButtonUi->SetIsButtonPush(true);		//ボタンが押された
//	m_pButtonUi->SetButtonKind(ButtonUi::e_ButtonKind::kYbutton);	//どのボタンが押されたか
	m_updaFunc = &Player::AttackYUpdate;
}

void Player::OnJump()
{
	//地面と接触しているかどうか
	//m_isJump = true;
	auto vel = m_rigidbody.GetVelocity();
	vel.y = kJumpPower;
	m_rigidbody.SetVelocity(vel);
	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
//	m_pButtonUi->SetIsButtonPush(true);								//ボタンが押された
//	m_pButtonUi->SetButtonKind(ButtonUi::e_ButtonKind::kBbutton);	//どのボタンが押されたか

	m_updaFunc = &Player::JumpUpdate;

}

void Player::OnAir()
{
	m_pAnim->ChangeAnim(kAnimAir);
	m_updaFunc = &Player::AirUpdate;
}

void Player::OnHit()
{
	m_pAnim->ChangeAnim(kAnimJump);
	m_updaFunc = &Player::HitUpdate;
}

void Player::OnDead()
{
	m_pAnim->ChangeAnim(kAnimDead, false, true, true);
	m_updaFunc = &Player::DeadUpdate;
}

void Player::OnSpawn()
{
	m_pAnim->ChangeAnim(kAnimSpawn);
	m_updaFunc = &Player::SpawnUpdate;
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

	//顔を決定する	ここはZRで決定にする
	if (Pad::IsTrigger(PAD_INPUT_9))
	{
		m_isFaceUse = !m_isFaceUse;
	}

}

void Player::CameraUpdate()
{
	//カメラ更新
}


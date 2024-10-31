#include "Player.h"
#include "PlayerWeapon.h"

#include "util/Pad.h"
#include "util/AnimController.h"

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
	constexpr float kJumpPower = 10.0f;

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
	m_modelH(-1),
	m_weaponH(-1),
	m_radius(2),
	m_posDown(kInitPos),
	m_posUp(kInitPos),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
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


}

Player::~Player()
{
	Finalize();
}

void Player::Initialize(VECTOR pos)
{
	//初期位置を代入
	m_posDown = pos;

	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	//武器の初期化
	m_pWeapon->Load();
	m_pWeapon->Initialize(m_modelH, kRightModelFrameNo, kLeftModelFrameNo);

	//アニメーションの初期化
	m_pAnim->Initialize(kAnimInfoFilename, m_modelH, kAnimIdle);

	// メンバ関数ポインタ = &クラス名::入れたい関数
	// メンバ関数ポインタの初期化
	m_updaFunc = &Player::IdleUpdate;

}

void Player::Finalize()
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

void Player::Update()
{

	//アップデート
	(this->*m_updaFunc)();

	/*フレームにアタッチするための更新処理*/
	m_pWeapon->SwordUpdate();
	m_pWeapon->AxeUpdate();
	m_pWeapon->DaggerUpdate();
	m_pWeapon->MagicWandUpdate();
	m_pWeapon->LongSwordUpdate();


	//アニメーションの更新処理
	m_pAnim->UpdateAnim();

	//仮重力
	m_posDown.y -= 1.2f;

	//仮地面
	if (m_posDown.y < 0)
	{
		m_posDown.y = 0;
		m_isJump = false;
	}
	else
	{
		m_isJump = true;
	}


	//顔を選択する関数
	FaceSelect();



	//カプセル用のポジション
	m_posUp = VGet(m_posDown.x, m_posDown.y + kUpPos.y, m_posDown.z);

	//モデルのポジションを合わせるよう
	VECTOR modelPos = VGet(m_posDown.x, m_posDown.y, m_posDown.z);

	//モデルに座標をセットする
	MV1SetPosition(m_modelH, m_posDown);

	//モデルに回転をセットする
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));

	//HPがゼロより下にいった場合
	if (m_hp <= 0)
	{
		m_hp = 0;
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

	DrawSphere3D(m_attackPos, m_radius, 8, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_posDown, m_radius, 8, 0xff00ff, 0xffffff, false);

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_posDown.x, m_posDown.y, m_posDown.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(0, 80, 0x000fff, " PlayerKind : %d ", m_playerKind);
	DrawFormatString(0, 280, 0x000fff, " PlayerHp : %d ", m_hp);

	DrawCapsule3D(m_posDown, m_posUp, m_radius, 32, 0xffffff, 0xffffff, false);

#endif

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
		m_hp -= 1;
	}

	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		OnAttackY();
		return;
	}

	//HPがゼロより下にいった場合
	if (m_hp < 0)
	{
		OnDead();
		return;
	}
}

void Player::WalkUpdate()
{
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
	if (len > 0.0f)
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

		m_posDown = VAdd(m_posDown, move);
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
}

void Player::DashUpdate()
{
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

		m_posDown = VAdd(m_posDown, move);

		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);

		if (Pad::IsRelase(PAD_INPUT_1))
		{
			OnWalk();
		}
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

}

void Player::JumpUpdate()
{
	//仮重力
	m_posDown.y -= 1.2f;

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

	m_posDown = VAdd(m_posDown, move);

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);
	}



	m_posDown = VAdd(m_posDown, VGet(0, kJumpPower, 0));

	//仮重力
	//m_posDown.y = kJumpPower;
}

void Player::AirUpdate()
{
	//仮重力
	m_posDown.y -= 1.2f;

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

	m_posDown = VAdd(m_posDown, move);

}

void Player::AttackXUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}

}

void Player::AttackYUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}

}

void Player::HitUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnIdle();
	}

}

void Player::DeadUpdate()
{
	if (m_pAnim->IsLoop())
	{
		OnDeadPose();
	}
}

void Player::DeadPoseUpadte()
{

}

void Player::SpawnUpdate()
{
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
	m_pAnim->ChangeAnim(kAnimDash);
	m_updaFunc = &Player::DashUpdate;
}


void Player::OnAttackX()
{
	m_pAnim->ChangeAnim(kAnimNormalAttackX,true,true,true);
	m_updaFunc = &Player::AttackXUpdate;
}

void Player::OnAttackY()
{
	m_pAnim->ChangeAnim(kAnimNormalAttackY,true, true, true);
	m_updaFunc = &Player::AttackYUpdate;
}

void Player::OnJump()
{
	m_pAnim->ChangeAnim(kAnimJump, true, true, true);
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
	m_pAnim->ChangeAnim(kAnimDead);
	m_updaFunc = &Player::DeadUpdate;
}

void Player::OnDeadPose()
{
	m_pAnim->ChangeAnim(kAnimDeadPose);
	m_updaFunc = &Player::DeadPoseUpadte;
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

void Player::Move(VECTOR move)
{

}

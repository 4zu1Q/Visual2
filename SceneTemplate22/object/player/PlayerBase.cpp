#include "PlayerBase.h"
#include "PlayerWeapon.h"

#include "util/Pad.h"

#include <cmath>
#include <cassert>

namespace
{
	//プレイヤーのモデルファイル名
	const char* const kEnemyModelFilename = "Data/Model/Player/Player.mv1";

	//移動速度
	constexpr float kMaxSpeed = 2.0f;
	constexpr float kMinSpeed = 1.0f;

	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.1f;
	constexpr float kAnalogRangeMax = 0.8f;

	constexpr float kAnalogInputMax = 1000.0f;

	//ジャンプ力
	constexpr float kJumpPower = 10.0f;

	//モデルのスケール値
	constexpr float kModelScale = 5.0f;
}

PlayerBase::PlayerBase() :
	m_modelH(-1),
	m_radius(5),
	m_pos(VGet(0, 0, 0)),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_avoid(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0),
	m_hp(10),
	m_isFaceUse(false),
	m_frame(0)
{
	m_modelH = MV1LoadModel("Data/Model/Player/Player.mv1");
	assert(m_modelH > -1);

	m_playerKind = e_PlayerKind::kNormalPlayer;

	m_pWeapon = std::make_shared<PlayerWeapon>();


}

PlayerBase::~PlayerBase()
{

	Finalize();

}

void PlayerBase::Initialize()
{
	//モデルのスケールを決める
	MV1SetScale(m_modelH, VGet(kModelScale, kModelScale, kModelScale));

	m_pWeapon->Initialize();
}

void PlayerBase::Finalize()
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	m_pWeapon->Finalize();
}

void PlayerBase::Update()
{
	//武器をアタッチするアップデート関数
	m_pWeapon->BothHandsUpdate(m_modelH);

	//仮重力
	m_pos.y -= 2.0f;

	//仮地面
	if (m_pos.y < 0)
	{
		m_pos.y = 0;
	}

	//移動関数
	Move();

	//顔を選択する関数
	FaceSelect();


	//Aボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		//ジャンプする関数を呼び出す
		Jump();
	}

	//Bボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		//回避する関数を呼び出す
		Avoid();
	}

	//Xボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_3))
	{
		//攻撃する関数を呼び出す
		Attack();
	}

	//Yボタンを押した場合
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		//特殊攻撃する関数を呼び出す
		FaceAttack();
	}


	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
}

void PlayerBase::Draw()
{
	//武器の描画
	m_pWeapon->BothHandsDraw();

	//モデルの描画
	MV1DrawModel(m_modelH);

#ifdef _DEBUG

	DrawSphere3D(m_attackPos, m_radius, 8, 0xff00ff, 0xffffff, false);
	DrawSphere3D(m_pos, m_radius, 8, 0xff00ff, 0xffffff, false);

	DrawFormatString(0, 48, 0xff0fff, "playerPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
	DrawFormatString(0, 64, 0xff0fff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(0, 80, 0x000fff, " PlayerKind : %d ", m_playerKind);
	DrawFormatString(0, 280, 0x000fff, " PlayerHp : %d ", m_hp);

#endif

}

void PlayerBase::Move()
{
	/*プレイヤーの移動*/
	GetJoypadAnalogInput(&m_analogX, &m_analogZ, DX_INPUT_PAD1);
	VECTOR move = VGet(m_analogX, 0.0f, -m_analogZ);
	float len = VSize(move);
	float rate = len / kAnalogInputMax;

	//アナログスティック無効な範囲を除外する
	//あってもなくてもどっちでもよい
	//rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
	//rate = min(rate, 1.0f);
	//rate = max(rate, 0.0f);

	//速度が決定できるので移動ベクトルに反映する
	move = VNorm(move);

	
	//スティックの押し加減でプレイヤーのスピードを変える
	//歩き
	if (rate <= 0.8f && rate >= 0.0f);
	{
		float speed = kMinSpeed * rate;
		move = VScale(move, speed);
	}
	//走り
	if (rate >= 0.8f)
	{
		float speed = kMaxSpeed * rate;
		move = VScale(move, speed);
	}

	m_pos = VAdd(m_pos, move);

	m_move = move;

	//動いている間
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);
		m_avoid = VNorm(move);

	}

	VECTOR attackMove = VScale(m_attackDir, 10.0f);

	m_attackPos = VAdd(m_pos, attackMove);


}

void PlayerBase::Attack()
{

	//攻撃

}

void PlayerBase::FaceAttack()
{
	//通常時
	if (m_playerKind == e_PlayerKind::kNormalPlayer && m_isFaceUse)
	{

		printfDx("kNormalPlayer");
	}
	//特殊1
	else if (m_playerKind == e_PlayerKind::kPowerPlayer && m_isFaceUse)
	{

		printfDx("kPowerPlayer");
	}
	//特殊2
	else if (m_playerKind == e_PlayerKind::kSpeedPlayer && m_isFaceUse)
	{

		printfDx("kSpeedPlayer");
	}
	//特殊3
	else if (m_playerKind == e_PlayerKind::kShotPlayer && m_isFaceUse)
	{

		printfDx("kShotPlayer");
	}
	//特殊4
	else if (m_playerKind == e_PlayerKind::kHuckShotPlayer && m_isFaceUse)
	{

		printfDx("kHuckShotPlayer");
	}
	//特殊5
	else if (m_playerKind == e_PlayerKind::kStrongestPlayer && m_isFaceUse)
	{

		printfDx("kStrongestPlayer");
	}

}

void PlayerBase::Gard()
{

}

void PlayerBase::Avoid()
{

	VECTOR avoid = VScale(m_avoid, 10.0f);
	m_move = VAdd(m_move, avoid);

	m_pos = VAdd(m_pos, m_move);

}

void PlayerBase::Jump()
{
	m_pos.y = kJumpPower;
}

void PlayerBase::FaceSelect()
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
		if (m_playerKind != e_PlayerKind::kNormalPlayer)
		{
			m_playerKind = static_cast<e_PlayerKind>(static_cast<int>(m_playerKind) - 1);
		}
	}

	//顔を決定する
	if (Pad::IsTrigger(PAD_INPUT_4))
	{
		m_isFaceUse = !m_isFaceUse;
	}

}



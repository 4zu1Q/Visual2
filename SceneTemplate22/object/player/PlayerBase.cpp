
#include "PlayerBase.h"

#include "util/Pad.h"

#include <cmath>

namespace
{
	constexpr float kMaxSpeed = 2.9f;
	constexpr float kMinSpeed = 0.5f;

	constexpr float kAnalogRangeMin = 0.1f;
	constexpr float kAnalogRangeMax = 0.8f;

	constexpr float kAnalogInputMax = 1000.0f;
}

PlayerBase::PlayerBase() :
	m_modelH(-1),
	m_radius(5),
	m_pos(VGet(0, 0, 0)),
	m_move(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_analogX(0),
	m_analogZ(0)
{
	m_modelH = MV1LoadModel("Data/Model/Enemy.mv1");
	m_playerKind = e_PlayerKind::kNormalPlayer;

	m_pos2 = VAdd(m_pos, VGet(0, 4, 0));

}

PlayerBase::~PlayerBase()
{
	Finalize();
}

void PlayerBase::Initialize()
{

}

void PlayerBase::Finalize()
{

}

void PlayerBase::Update()
{
	Pad::Update();

	int pov = GetJoypadPOVState(DX_INPUT_PAD1, 0);

	Move();

	//Aボタンを押した場合
	if (Pad::IsPress(PAD_INPUT_1))
	{
		//ジャンプする関数を呼び出す

	}

	if (Pad::IsTrigger(PAD_INPUT_2))
	{
		//回避する関数を呼び出す
		Avoid();
	}

	if (Pad::IsPress(PAD_INPUT_3))
	{
		//攻撃する関数を呼び出す
		Attack();
	}

	if (Pad::IsPress(PAD_INPUT_4))
	{
		//特殊攻撃
		if (m_playerKind == e_PlayerKind::kNormalPlayer)
		{

		}
		else if (m_playerKind == e_PlayerKind::kPowerPlayer)
		{

		}
		else if (m_playerKind == e_PlayerKind::kSpeedPlayer)
		{

		}
		else if (m_playerKind == e_PlayerKind::kHuckShotPlayer)
		{

		}

	}

	
	if (Pad::IsTrigger(PAD_INPUT_8))
	{
		//顔使用
		FaceSelect();
	}


	m_pos2 = VAdd(m_pos, VGet(0, 4, 0));

}

void PlayerBase::Draw()
{
	//DrawSphere3D(m_pos, m, 8, 0xff00ff, 0xffffff, false);

	DrawSphere3D(m_attackPos, m_radius , 8, 0xff00ff, 0xffffff, false);
	DrawCapsule3D(m_pos, m_pos2, m_radius, 16, 0xffffff, 0xffffff, false);

	DrawFormatString(0, 48, 0xffffff, "playerPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
	DrawFormatString(0, 64, 0xffffff, "playerAttackPos:%f,%f,%f", m_attackPos.x, m_attackPos.y, m_attackPos.z);
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

#ifdef _DEBUG

	DrawFormatString(0, 32, 0xffffff, "analog:%f", rate);

#endif


	if (VSquareSize(m_move) > 0.0f)
	{
		m_attackDir = VNorm(m_move);
	}

	VECTOR attackMove = VScale(m_attackDir, 10.0f);

	m_attackPos = VAdd(m_pos, attackMove);

}

void PlayerBase::Attack()
{


}

void PlayerBase::Gard()
{

}

void PlayerBase::Avoid()
{
	m_pos = VAdd(m_pos, VGet(0, 0, 5));
}

void PlayerBase::FaceSelect()
{

}

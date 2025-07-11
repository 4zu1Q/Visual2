#include "MoveDirectionVec.h"
#include"Pad.h"

//#include"CharacterComponent.h"

namespace
{
	// 移動方向
	constexpr float kUp = -1.0f;	//上ベクトル
	constexpr float kDown = 1.0f;	//下ベクトル
	constexpr float kRight = 1.0f;	//右ベクトル
	constexpr float kLeft = -1.0f;	//左ベクトル
	constexpr float kZero = 0.0f;	//ベクトルなし
}


MoveDirectionVec::~MoveDirectionVec()
{
}

void MoveDirectionVec::Update()
{
	//パッドアナログ情報取得
	DINPUT_JOYSTATE input;

	input.X = 0;
	input.Y = 0;

	//移動角度
	float moveAngle = 0.0f;

	// 入力状態を取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	//移動ベクトル初期化
	m_directionVec = VGet(0.0f, 0.0f, 0.0f);

	//スティックでのアナログ入力状態が反応しているときはそっちを優先する
	if (input.X != 0 || input.Y != 0)
	{
		m_directionVec.x = 0.001f * input.X;
		m_directionVec.z = 0.001f * input.Y;
	}
	else
	{
		// 左ボタンを押されたらプレイヤーの向きを左に
		if (Pad::IsPress(PAD_INPUT_LEFT))
		{
			m_directionVec.x = kLeft;
		}
		// 右ボタンを押されたらプレイヤーの向き右に
		else if (Pad::IsPress(PAD_INPUT_RIGHT))
		{
			m_directionVec.x = kRight;
		}
		else
		{
			m_directionVec.x = kZero;
		}

		// 上ボタンを押されたらプレイヤーの向きを上に
		if (Pad::IsPress(PAD_INPUT_UP))
		{
			m_directionVec.z = kUp;
		}
		// 下ボタンを押されたらプレイヤーの向き下に
		else if (Pad::IsPress(PAD_INPUT_DOWN))
		{
			m_directionVec.z = kDown;
		}
		else
		{
			m_directionVec.z = kZero;
		}
	}

	//移動方向ベクトルを正規化
	if (VSize(m_directionVec) >= 1.0f)
	{
		m_directionVec = VNorm(m_directionVec);
	}
}

void MoveDirectionVec::SetDirectionVec()
{
	//移動方向ベクトルを初期化
	m_directionVec = VGet(0.0f, kUp, 0.0f);
}

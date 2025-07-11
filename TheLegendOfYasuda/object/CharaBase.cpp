#include "CharaBase.h"

using namespace MyLib;

namespace
{
	//角度を滑らかに移動させる値
	constexpr float kSmoothAngleSpeed = 0.25f;

	//角度の差
	constexpr float kAngleDiffMax = DX_PI_F + 0.3f;
	constexpr float kAngleDiffMin = -DX_PI_F - 0.3f;
}

CharaBase::CharaBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
	ObjectBase(priority, tag, kind, isTrigger),
	m_pos(VGet(0,0,0)),
	m_move(VGet(0,0,0)),
	m_collisionPos(VGet(0,0,0)),
	m_status()
{

}

CharaBase::~CharaBase()
{

}

void CharaBase::SmoothAngle(float& nowAngle, float nextAngle)
{
	// －１８０度以下になったら角度値が大きくなりすぎないように３６０度を足す
	if (nextAngle < -DX_PI_F)
	{
		nextAngle += DX_TWO_PI_F;
	}

	// １８０度以上になったら角度値が大きくなりすぎないように３６０度を引く
	if (nextAngle > DX_PI_F)
	{
		nextAngle -= DX_TWO_PI_F;
	}

	//角度の差を求める
	float angleDiff = nowAngle - nextAngle;

	//角度の差が１８０度以上の場合
	if (angleDiff > kAngleDiffMax)
	{
		nowAngle -= DX_TWO_PI_F;
	}
	else if (angleDiff < kAngleDiffMin)
	{
		nowAngle += DX_TWO_PI_F;
	}

	//角度の差が少ない場合はそのままの角度を返す
	if (angleDiff < kSmoothAngleSpeed &&
		angleDiff > -kSmoothAngleSpeed)
	{
		nowAngle = nextAngle;
		return;
	}
	//角度の差が大きい場合は滑らかに角度を変化させる
	else if (angleDiff > kSmoothAngleSpeed)
	{
		nowAngle -= kSmoothAngleSpeed;
		return;
	}
	//同様に滑らかに角度を変化させる
	else if (angleDiff < kSmoothAngleSpeed)
	{
		nowAngle += kSmoothAngleSpeed;
		return;
	}
}



#include "DxLib.h"
#include "Rigidbody.h"

using namespace MyLib;

/// <summary>
/// コンストラクタ
/// </summary>
Rigidbody::Rigidbody() :
	m_pos(VGet(0, 0, 0)),
	m_nextPos(VGet(0, 0, 0)),
	m_dir (VGet(0, 0, 0)),
	m_velocity (VGet(0, 0, 0)),
	m_isUseGravity (false)
{
	// 処理なし
}


void Rigidbody::Initialize(bool useGravity)
{
	m_pos = VGet(0, 0, 0);
	m_dir = VGet(0, 0, 0);
	m_velocity = VGet(0, 0, 0);
	this->m_isUseGravity = useGravity;
}

/// <summary>
/// velocityに力を加える
/// </summary>
void Rigidbody::AddForce(const VECTOR& force)
{
	m_velocity = VAdd(m_velocity, force);
}

/// <summary>
/// VelocityのSetとDir自動計算
/// </summary>
void Rigidbody::SetVelocity(const VECTOR& set)
{
	m_velocity = set;
	if (VSquareSize(m_velocity) > 0)
	{
		m_dir = VNorm(m_velocity);
	}
}
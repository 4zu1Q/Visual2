#include "BossBase.h"
#include "object/player/Player.h"

#include "myLib/MyLib.h"

BossBase::BossBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
	CharaBase(priority, tag, kind, isTrigger),
	m_hp(0),
	m_preliminaryActionFrame(0),
	m_pos(VGet(0,0,0)),
	m_hitPos(VGet(0,0,0)),
	m_playerAttackXPos(VGet(0,0,0)),
	m_playerAttackYPos(VGet(0,0,0)),
	m_playerShockPos(VGet(0,0,0)),
	m_playerAttackXRadius(0.0f),
	m_playerAttackYRadius(0.0f),
	m_playerShockRadius(0.0f)
{
	m_attackKind = Game::e_BossAttackKind::kBossAttackNone;
	m_isPlayerAttack = false;
}

BossBase::~BossBase()
{

}

void BossBase::Initialize()
{

}

void BossBase::Finalize()
{

}

void BossBase::Update()
{

}

void BossBase::Draw()
{

}

void BossBase::HitUpdate(VECTOR attackXPos, VECTOR attackYPos, VECTOR shockPos, float attackXRadius, float attackYRadius, float shockRadius, bool isAttack)
{
	m_playerAttackXPos = attackXPos;
	m_playerAttackYPos = attackYPos;
	m_playerShockPos = shockPos;

	m_playerAttackXRadius = attackXRadius;
	m_playerAttackYRadius = attackYRadius;
	m_playerShockRadius = shockRadius;

	m_isPlayerAttack = isAttack;
}

void BossBase::PlayerLockOn(Player& player)
{

}

bool BossBase::IsAttackXHit()
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - m_playerAttackXPos.x) * (m_hitPos.x - m_playerAttackXPos.x);
	float delY = ((m_hitPos.y) - (m_playerAttackXPos.y)) *
		((m_hitPos.y) - (m_playerAttackXPos.y));
	float delZ = (m_hitPos.z - m_playerAttackXPos.z) * (m_hitPos.z - m_playerAttackXPos.z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + m_playerAttackXRadius)
	{
		return true;
	}

	return false;
}

bool BossBase::IsAttackYHit()
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - m_playerAttackYPos.x) * (m_hitPos.x - m_playerAttackYPos.x);
	float delY = ((m_hitPos.y) - (m_playerAttackYPos.y)) *
		((m_hitPos.y) - (m_playerAttackYPos.y));
	float delZ = (m_hitPos.z - m_playerAttackYPos.z) * (m_hitPos.z - m_playerAttackYPos.z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + m_playerAttackYRadius)
	{
		return true;
	}

	return false;
}

bool BossBase::IsShockAttackHit()
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_hitPos.x - m_playerShockPos.x) * (m_hitPos.x - m_playerShockPos.x);
	float delY = ((m_hitPos.y) - (m_playerShockPos.y)) *
		((m_hitPos.y) - (m_playerShockPos.y));
	float delZ = (m_hitPos.z - m_playerShockPos.z) * (m_hitPos.z - m_playerShockPos.z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_hitRadius + m_playerShockRadius)
	{
		return true;
	}

	return false;
}

#include "EnemyBase.h"
#include "Player.h"

namespace
{
	constexpr float kAddPosY = 8.0f;

}

EnemyBase::EnemyBase()
{
}

void EnemyBase::EnemyLost()
{
}

/// <summary>
/// 球の当たり判定
/// </summary>
bool EnemyBase::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pPlayer"></param>
/// <returns></returns>
bool EnemyBase::AttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::SkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetSkillRadius())
	{
		return true;
	}

	return false;

}

bool EnemyBase::DamageSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_attackPos.x - pPlayer->GetPos().x) * (m_attackPos.x - pPlayer->GetPos().x);
	float delY = ((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_attackPos.z - pPlayer->GetPos().z) * (m_attackPos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::SearchSphereFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::StopSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

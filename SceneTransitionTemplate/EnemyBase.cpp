#include "EnemyBase.h"
#include "Player.h"

namespace
{
	constexpr float kAddPosY = 8.0f;

}

EnemyBase::EnemyBase():
	m_handle(-1),
	m_pos(VGet(0,0,0)),
	m_radius(6.0f)
{
	
}

void EnemyBase::EnemyLost()
{
}

/// <summary>
/// ���̓����蔻��
/// </summary>
bool EnemyBase::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
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
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::SkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetSkillRadius())
	{
		return true;
	}

	return false;

}

bool EnemyBase::DamageSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_attackPos.x - pPlayer->GetPos().x) * (m_attackPos.x - pPlayer->GetPos().x);
	float delY = ((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_attackPos.z - pPlayer->GetPos().z) * (m_attackPos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::SearchSphereFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool EnemyBase::StopSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

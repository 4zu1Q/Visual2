#include "AttackCol.h"
#include "object/player/Player.h"
#include "object/boss/BossBase.h"

AttackCol::AttackCol():
	m_radius(0.0f),
	m_pos(VGet(0, 0, 0))
{

}

AttackCol::~AttackCol()
{

}

void AttackCol::Initialize(float radius, VECTOR pos)
{
	m_radius = radius;
	m_pos = pos;
}

void AttackCol::Update(VECTOR pos)
{
	m_pos = pos;
}

bool AttackCol::CheckCollisionWithPlayer(const std::shared_ptr<Player>& player)
{
	return false;
}

bool AttackCol::CheckCollisionWithBoss(const std::shared_ptr<BossBase>& boss)
{
	return false;
}

void AttackCol::Draw()
{

}

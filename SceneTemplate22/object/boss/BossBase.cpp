#include "BossBase.h"
#include "object/player/Player.h"

#include "myLib/MyLib.h"

BossBase::BossBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
	CharaBase(priority, tag, kind, isTrigger),
	m_hp(0),
	m_pos(VGet(0,0,0))
{

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

void BossBase::PlayerLockOn(Player& player)
{

}

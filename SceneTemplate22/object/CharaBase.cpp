#include "CharaBase.h"

using namespace MyLib;

CharaBase::CharaBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind colliderKind, bool isTrigger):
	ObjectBase(priority, tag, colliderKind, isTrigger),
	m_pos(VGet(0,0,0)),
	m_move(VGet(0,0,0)),
	m_collisionPos(VGet(0,0,0)),
	m_status()
{

}

CharaBase::~CharaBase()
{

}



#include "ItemBase.h"
#include "DxLib.h"

#include <cmath>
#include <cassert>
#include <string>

ItemBase::ItemBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
	ObjectBase(priority, tag, kind, isTrigger),
	m_modelH(-1),
	m_pos(VGet(0,0,0)),
	m_move(VGet(0,0,0)),
	m_rot(0.0f)
{

}

ItemBase::~ItemBase()
{

}



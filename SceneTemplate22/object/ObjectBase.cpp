#include "ObjectBase.h"

using namespace MyLib;

ObjectBase::ObjectBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool IsTrigger):
		Collidable(priority, tag, kind, IsTrigger),
		m_rot(VGet(0,0,0)),
		m_modelH(-1)
{

}

ObjectBase::~ObjectBase()
{
	MV1DeleteModel(m_modelH);
}


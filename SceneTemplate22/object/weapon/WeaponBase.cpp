#include "WeaponBase.h"

WeaponBase::WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger) :
	ObjectBase(priority, tag, kind, isTrigger)
{

}

WeaponBase::~WeaponBase()
{

}

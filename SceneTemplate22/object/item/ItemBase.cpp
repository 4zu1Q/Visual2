#include "ItemBase.h"

ItemBase::ItemBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
	ObjectBase(priority, tag, kind, isTrigger)
{

}

ItemBase::~ItemBase()
{

}



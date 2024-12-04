#include "ShotBase.h"

ShotBase::ShotBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger):
ObjectBase(priority, tag, kind, isTrigger)
{

}

ShotBase::~ShotBase()
{

}

void ShotBase::Initialize(int modelH, int ownerModelH, const TCHAR* frameLeftName, const TCHAR* frameRightName, float modelSize)
{

}

void ShotBase::Finalize()
{

}

void ShotBase::Update(VECTOR moveVec)
{

}

void ShotBase::Draw()
{

}

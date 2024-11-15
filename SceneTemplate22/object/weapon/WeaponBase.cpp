#include "WeaponBase.h"

WeaponBase::WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger) :
	ObjectBase(priority, tag, kind, isTrigger)
{

}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Initialize(int modelH, int modelRightFrame, int modelLeftFrame)
{
	//代入
	m_modelH = modelH;
	m_modelRightFrame = modelRightFrame;
	m_modelLeftFrame = modelLeftFrame;
}

void WeaponBase::Finalize()
{

}

void WeaponBase::OneHandUpdate(int weaponModelH)
{

}

void WeaponBase::BothHandsUpdate(int rightWeaponModelH, int leftWeaponModelH)
{

}

void WeaponBase::OneHandDraw(int weaponModelH)
{

}

void WeaponBase::BothHandsDraw(int rightWeaponModelH, int leftWeaponModelH)
{

}

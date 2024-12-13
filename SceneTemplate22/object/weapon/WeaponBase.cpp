#include "WeaponBase.h"


WeaponBase::WeaponBase():
	ObjectBase(e_Priority::kLow, e_GameObjectTag::kSword, MyLib::ColliderData::e_Kind::kSphere, false)
{
	//当たり判定の設定
	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);
	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 3.0f;
}

WeaponBase::~WeaponBase()
{

}

void WeaponBase::Initialize(int modelH, int ownerModelH, const TCHAR* frameLeftName, const TCHAR* frameRightName , float modelSize)
{

}

void WeaponBase::Finalize()
{

}

void WeaponBase::Update()
{

}

void WeaponBase::Draw()
{

}

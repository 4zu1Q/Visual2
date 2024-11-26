#include "WeaponBase.h"



//WeaponBase::WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger) :
//	ObjectBase(priority, tag, kind, isTrigger),
//	m_weaponModelH(-1),
//	m_ownerModelH(-1),
//	m_weaponRightAttachFrame(-1),
//	m_weaponlLeftAttachFrame(-1),
//	m_modelSize(1.0f),
//	m_mat{0.0f,0.0f,0.0f}
//{
//
//
//
//}

WeaponBase::WeaponBase():
	ObjectBase(e_Priority::kLow, e_GameObjectTag::kSword, MyLib::ColliderData::e_Kind::kSphere, false),
	m_weaponModelH(-1),
	m_ownerModelH(-1),
	m_weaponRightAttachFrame(-1),
	m_weaponlLeftAttachFrame(-1),
	m_modelSize(1.0f),
	m_mat{ 0.0f,0.0f,0.0f }
{
	//当たり判定の設定
	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);
	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 3.0f;
}

WeaponBase::~WeaponBase()
{
	MV1DeleteModel(m_weaponModelH);
	MV1DeleteModel(m_ownerModelH);
}

void WeaponBase::Initialize(int modelH, int ownerModelH, const TCHAR* frameLeftName, const TCHAR* frameRightName , float modelSize)
{
	//代入
	m_weaponModelH = modelH;
	m_ownerModelH = ownerModelH;
	SetAttachFrame(frameLeftName, frameRightName);
	SetModelSize(modelSize);
}

void WeaponBase::Finalize()
{

}

void WeaponBase::Update(VECTOR moveVec)
{
}

void WeaponBase::Draw()
{
}

void WeaponBase::OneHandUpdate(VECTOR moveVec)
{
	auto positionMat = MV1GetFrameLocalWorldMatrix(m_ownerModelH, m_weaponRightAttachFrame);
	auto rotationMat = MGetRotY(DX_PI_F);
	MATRIX temp = MMult(rotationMat, positionMat);
	auto scaleMat = MGetScale(VGet(m_modelSize, m_modelSize, m_modelSize));
	m_mat = MMult(scaleMat, temp);
	MV1SetMatrix(m_weaponModelH, m_mat);

	m_rigidbody.SetPos(moveVec);
}

void WeaponBase::BothHandsUpdate(VECTOR moveVec)
{
}

void WeaponBase::OneHandDraw()
{
	MV1DrawModel(m_weaponModelH);
}

void WeaponBase::BothHandsDraw(int rightWeaponModelH, int leftWeaponModelH)
{

}

void WeaponBase::SetAttachFrame(const TCHAR* frameLeftName, const TCHAR* frameRightName)
{
	m_weaponlLeftAttachFrame = MV1SearchFrame(m_ownerModelH, frameLeftName);
	m_weaponRightAttachFrame = MV1SearchFrame(m_ownerModelH, frameRightName);
}

void WeaponBase::SetModelSize(float modelSize)
{
	m_modelSize = modelSize;
}

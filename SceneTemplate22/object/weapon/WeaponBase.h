#pragma once
#include "object/ObjectBase.h"


class WeaponBase : public ObjectBase
{
public:

	//WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	WeaponBase();
	virtual ~WeaponBase();

	void Initialize(int modelH, int ownerModelH , const TCHAR* frameLeftName, const TCHAR* frameRightName, float modelSize);
	void Finalize();


	void Update();
	void Draw();

private:

	//モデルハンドル
	int m_weaponModelH;
	int m_ownerModelH;

	int m_weaponRightAttachFrame;	//アタッチするモデルの右手のフレーム
	int m_weaponlLeftAttachFrame;	//アタッチするモデルの左手のフレーム

	float m_modelSize;

	VECTOR m_attachFramePos;	//武器をアタッチする座標


	MATRIX m_mat;
};


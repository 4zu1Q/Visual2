#pragma once
#include "object/ObjectBase.h"


class WeaponBase : public ObjectBase
{
public:

	WeaponBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~WeaponBase();


protected:

	//モデルハンドル
	int m_modelH;

	int m_modelRightFrame;	//アタッチするモデルの右手のフレーム
	int m_modelLeftFrame;	//アタッチするモデルの左手のフレーム

	VECTOR m_attachFramePos;	//武器をアタッチする座標

};


#pragma once
#include "object/ObjectBase.h"


class ItemBase : public ObjectBase
{
public:
	ItemBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~ItemBase();


protected:

	//モデルハンドル
	int m_modelH;
	//座標
	VECTOR m_pos;
	//移動
	VECTOR m_move;

	//回転
	float m_rot;

private:


};


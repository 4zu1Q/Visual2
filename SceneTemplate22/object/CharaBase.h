#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include "util/Game.h"

#include "ObjectBase.h"

class CharaBase : public ObjectBase
{
public:

	struct Status
	{
		


	};


public:

	CharaBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind colliderKind, bool isTrigger);
	virtual ~CharaBase();

	//// 衝突したとき
	//virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:

	Status m_status;

	VECTOR m_pos;
	VECTOR m_move;
	VECTOR m_collisionPos;

	//ステータス関連をここに入れておきたい
};


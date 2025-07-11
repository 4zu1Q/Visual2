#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include "util/Game.h"

#include "ObjectBase.h"

class CharaBase : public ObjectBase
{
public:

	//ステータス構造体
	struct Status
	{
		int hp_;	//体力
		int atk_;	//攻撃力
		int def_;	//防御力
	};


public:

	CharaBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~CharaBase();

	//// 衝突したとき
	//virtual void OnCollideEnter(const std::shared_ptr<Collidable>& colider) = 0;

protected:

	//角度を滑らかに変化させる関数
	void SmoothAngle(float& nowAngle, float nextAngle);

protected:

	Status m_status;

	VECTOR m_pos;
	VECTOR m_move;
	VECTOR m_collisionPos;

	//ステータス関連をここに入れておきたい
};


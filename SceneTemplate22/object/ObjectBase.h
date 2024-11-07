#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include "util/Game.h"

#include <memory>

using namespace Game;

class ObjectBase : public MyLib::Collidable
{
public:
	ObjectBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind colliderKind, bool isTrigger);
	virtual ~ObjectBase();

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};

	void OnColliderEnter(const std::shared_ptr<Collidable>& collider);


protected:
	//角度
	VECTOR m_rot;
	//モデルハンドル
	int m_modelH;

};


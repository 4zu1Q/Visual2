#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include "util/Game.h"

#include <memory>


using namespace Game;

class ObjectBase : public MyLib::Collidable
{
public:
	ObjectBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool IsTrigger);
	virtual ~ObjectBase();

	virtual void Initialize() {};
	virtual void Update() {};
	virtual void Draw() {};

	//void OnColliderEnter(const std::shared_ptr<Collidable>& collider);
	//virtual void OnCollide(const std::shared_ptr<Collidable>& colider) = 0;
			

protected:

	//角度
	VECTOR m_rot;
	//モデルハンドル
	int m_modelH;


};


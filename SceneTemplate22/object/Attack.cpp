#include "Attack.h"

Attack::Attack()
	: ObjectBase(Collidable::e_Priority::kLow, Game::e_GameObjectTag::kAttack, MyLib::ColliderData::e_Kind::kSphere, false)
{
	auto collider = Collidable::AddCollider(MyLib::ColliderData::Kind::Sphere, true);
	auto sphereCol = dynamic_cast<MyLib::ColliderDataSphere*>(collider.get());

}

Attack::~Attack()
{
}

void Attack::Initialize(std::shared_ptr<MyLib::Physics> physics)
{
}

void Attack::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
}

void Attack::Update()
{
}

void Attack::Draw()
{
}

#pragma once
#include "ObjectBase.h"

class Attack : public ObjectBase
{
public:
	Attack();
	virtual ~Attack();

	void Initialize(std::shared_ptr<MyLib::Physics> physics);
	void Finalize(std::shared_ptr<MyLib::Physics> physics);
	void Update();
	void Draw();

	void OnCollideEnter(const std::shared_ptr<Collidable>& collider) {};



	void End();

private:



};


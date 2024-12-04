#pragma once
#include "object/ObjectBase.h"

class ShotBase : public ObjectBase
{
public:

	ShotBase(Collidable::e_Priority priority, e_GameObjectTag tag, MyLib::ColliderData::e_Kind kind, bool isTrigger);
	virtual ~ShotBase();
	//ShotBase();

	void Initialize(int modelH, int ownerModelH, const TCHAR* frameLeftName, const TCHAR* frameRightName, float modelSize);
	void Finalize();


	void Update(VECTOR moveVec);
	void Draw();

protected:


};


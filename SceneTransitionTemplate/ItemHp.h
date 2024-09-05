#pragma once
#include "ItemBase.h"

class ItemHp : public ItemBase
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Update();
	void Draw();

private:

	int m_handle;

	float m_moveHeight;
	float m_rot;

};


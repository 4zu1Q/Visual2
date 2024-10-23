#pragma once
#include "ItemBase.h"

class ItemHp : public ItemBase
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

private:

	

};


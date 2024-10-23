#pragma once
#include "ItemBase.h"

class ItemMp : public ItemBase
{
public:
	ItemMp();
	virtual ~ItemMp();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

private:



};


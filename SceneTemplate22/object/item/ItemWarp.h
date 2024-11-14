#pragma once
#include "ItemBase.h"

class ItemWarp : public ItemBase
{
public:
	ItemWarp();
	virtual ~ItemWarp();

	void Initialize(std::shared_ptr<MyLib::Physics> physics);
	void Finalize(std::shared_ptr<MyLib::Physics> physics);

	void Update(std::shared_ptr<MyLib::Physics> physics);
	void Draw();

private:
};


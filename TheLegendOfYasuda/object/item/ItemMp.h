#pragma once
#include "DxLib.h"
#include <memory>

#include "ItemBase.h"

class Player;
class ItemMp : public ItemBase
{
public:
	ItemMp();
	virtual ~ItemMp();

	void Initialize(std::shared_ptr<MyLib::Physics> physics);
	void Finalize(std::shared_ptr<MyLib::Physics> physics);

	void Update(std::shared_ptr<MyLib::Physics> physics);
	void Draw();

private:

};


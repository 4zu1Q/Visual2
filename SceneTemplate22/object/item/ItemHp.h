#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include <memory>

#include "ItemBase.h"

class Colidable;

class ItemHp : public ItemBase
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Initialize(std::shared_ptr<MyLib::Physics> physics) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;

	void Update(std::shared_ptr<MyLib::Physics> physics);
	void Draw();

	// 衝突したとき
	//virtual void OnCollide(const Collidable& colider);

private:


};


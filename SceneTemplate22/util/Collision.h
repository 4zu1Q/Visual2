#pragma once
#include <memory>

class ItemBase;
class Player;
class BossBase;
class Collision
{
public:
	Collision();
	virtual ~Collision();

	//プレイヤーとアイテムの衝突判定
	bool ItemSphereFlag(std::shared_ptr<Player> pPlayer, std::shared_ptr<ItemBase> pItem);

	////
	//bool ItemSphereFlag(std::shared_ptr<Player> pPlayer, std::shared_ptr<BossBase> pBoss);
	//bool ItemSphereFlag(std::shared_ptr<Player> pPlayer);
	//bool ItemSphereFlag(std::shared_ptr<Player> pPlayer);


private:



};


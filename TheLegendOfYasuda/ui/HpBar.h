#pragma once

#include "util/Vec2.h"
#include <vector>
#include <memory>

#include "util/Game.h"

//class Player;
class BossPower;
//class BossBase;

class HpBar
{
public:

	enum class e_BossType : int
	{

	};


public:

	HpBar();
	virtual ~HpBar();

	void Update(int boss);
	void Draw();
	void DrawBossName(Game::e_BossKind bossKind);

private:

	//std::shared_ptr<Player> m_pPlayer;
	//std::shared_ptr<BossBase> m_pBoss;

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

	int m_bossHp;
	int m_bossHpH;
};


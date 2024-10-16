#pragma once
#include <memory>

class PlayerBase;
class BossBase;

class HpBar
{
public:

	enum class e_BossType : int
	{

	};


public:

	HpBar();
	virtual ~HpBar();

	void Initialize();
	void Finalize();

	void Update(PlayerBase& player);
	void Draw();

private:

	std::shared_ptr<PlayerBase> m_pPlayer;
	std::shared_ptr<BossBase> m_pBoss;

	int m_playerHp;
	int m_bossHp;

	int m_playerHpH;
	int m_bossHpH;
};


#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy
{
public:

	Enemy();
	virtual ~Enemy();

	void Load();
	void Delete();

	void Init();
	void Update();
	void Draw();

	//ƒvƒŒƒCƒ„[‚Æ“G‚Ì“–‚½‚è”»’è
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//ƒvƒŒƒCƒ„[‚ÌUŒ‚‚Æ“G‚Ì“–‚½‚è”»’è
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//“G‚ÌUŒ‚”»’è
	bool DamageSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//“G‚Ìõ“G”ÍˆÍ
	bool SearchSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//“G‚ª~‚Ü‚é”ÍˆÍ
	bool StopSphereHitFlag(std::shared_ptr<Player> pPlayer);



	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//“G‚ÌHP‚ğæ“¾
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//“G‚ª‰½‚ğ‚·‚é‚©‚ğ‚«‚ß‚é
	enum State
	{
		kIdle,	//‘Ò‹@
		kRun,	//‘–‚é
		kAttack,	 //UŒ‚
		kPowerAttack,//—­‚ßUŒ‚
	};



private:

	std::shared_ptr<Player> m_pPlayer;

	//ƒ‚ƒfƒ‹ƒnƒ“ƒhƒ‹
	//int m_modelHandle[8];
	int m_modelHandle;
	//ƒVƒF[ƒ_ƒnƒ“ƒhƒ‹
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	//•\¦î•ñ
	VECTOR m_pos;
	VECTOR m_attackPos;

	//“–‚½‚è”»’è‚Ì”¼Œa
	float m_radius;
	float m_searchRadius;
	float m_stopRadius;

	//State•Ï”
	State m_state;
};


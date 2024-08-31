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
	void Update(Player& player);
	void Draw();

	//プレイヤーと敵の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの攻撃と敵の当たり判定
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);


	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//敵のHPを取得
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//敵が何をするかをきめる
	enum State
	{
		kIdle,	//待機
		kRun,	//走る
		kAttack,	 //攻撃
		kPowerAttack,//溜め攻撃
	};



private:

	//モデルハンドル
	//int m_modelHandle[8];
	int m_modelHandle;
	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	//表示情報
	VECTOR m_pos;

	//当たり判定の半径
	float m_radius;

	//State変数
	State m_state;
};


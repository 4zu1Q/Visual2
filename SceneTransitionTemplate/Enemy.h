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
	void Update(std::shared_ptr<Player> pPlayer);
	void Draw();

	//プレイヤーと敵の当たり判定
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの攻撃と敵の当たり判定
	bool PlayerAttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//プレイヤーの必殺技と敵の当たり判定
	bool PlayerSkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//敵の攻撃判定
	bool EnemyAttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//敵の攻撃判定
	bool EnemySkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//敵の索敵範囲
	bool SearchSphereFlag(std::shared_ptr<Player> pPlayer);
	//敵が止まる範囲
	bool StopSphereFlag(std::shared_ptr<Player> pPlayer);

	const bool& GetAttackGeneration() const { return m_isAttackGeneration; }
	const bool& GetSkillGeneration() const { return m_isSkillGeneration; }

	void SetAttackGeneration(bool isAttackGeneration) { m_isAttackGeneration = isAttackGeneration; }
	void SetSkillGeneration(bool isSkillGeneration) { m_isSkillGeneration = isSkillGeneration; }

	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//敵のHPを取得
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }



	//プレイヤーダメージのフラグ取得
	void SetDamage(const bool damage) { m_isDamage = damage; }

	//敵が何をするかをきめる
	enum State
	{
		kIdle,	//待機
		kRun,	//走る
		kAttack,	 //攻撃
		kDeath,		//死ぬ
	};

	State GetState() { return m_state; }
	void SetState(State state) { m_state = state; }

private:
	//アニメーションの進行
//ループしたかどうかを返す
	bool UpdateAnim(int attachNo, bool isStayEndAnim = false);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

	void Move();

private:


	//モデルハンドル
	//int m_modelHandle[8];
	int m_modelH;
	int m_modelWeponH;
	int m_hpFlameH;
	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	int m_damageFrame;
	bool m_isDamage;
	
	int m_rand;
	int m_random;
	int m_workFrame;

	//表示情報
	VECTOR m_pos;
	VECTOR m_attackPos;
	VECTOR m_attackDir;
	VECTOR m_velocity;
	VECTOR m_direction;
	VECTOR m_dirPos;
	float m_angle;

	//アニメーション情報
	int m_animIndex;
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//当たり判定の半径
	float m_radius;
	float m_skillRadius;
	float m_searchRadius;
	float m_stopRadius;

	//移動するかどうかのフラグ
	bool m_isMove;

	//アニメーションフラグ
	bool m_isIdleAnim;
	bool m_isRunAnim;
	bool m_isAttackAnim;
	bool m_isSkillAnim;
	bool m_isDamageAnim;
	bool m_isDeadAnim;
	bool m_isDeadingAnim;

	bool m_isAttackGeneration;
	bool m_isSkillGeneration;

	bool m_isRand;

	//フレーム
	int m_frame;
	int m_attackFrame;

	//State変数
	State m_state;

	//武器に関する変数
	VECTOR m_attachFramePosition;
	//MATRIX m_transMat;
	//MATRIX m_attachFrameMat;
	//MATRIX m_mixMatrix;


#ifdef _DEBUG
	bool m_isDebugFlag = false;
#endif

};


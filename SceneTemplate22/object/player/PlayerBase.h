#pragma once
#include "DxLib.h"
#include <memory>

class PlayerWeapon;

class PlayerBase
{
public:

	//プレイヤーの特殊攻撃の種類(顔の種類)
	enum class e_PlayerKind : int
	{
		kNormalPlayer,		//通常時
		kPowerPlayer,		//パワー型
		kSpeedPlayer,		//スピード型
		kShotPlayer,		//弾を撃つ
		kHuckShotPlayer,	//フックショット
		kStrongestPlayer	//ラスボス型
	};


public:

	PlayerBase();
	virtual ~PlayerBase();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();

	//プレイヤーのモデルハンドルを取得
	const int& GetModelHandle() const { return m_modelH; }
	void SetModelHandle(const int modelH) { m_modelH = modelH; }

	//プレイヤーの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//プレイヤーの攻撃座標を取得
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }

	//プレイヤーのHPを取得
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//プレイヤーの顔の種類
	const e_PlayerKind& GetFaceKind() const { return m_playerKind; }
	void SetFaceKind(const e_PlayerKind playerKind) { m_playerKind = playerKind; }



private:

	/// <summary>
	/// プレイヤーの移動時の関数
	/// </summary>
	void Move();

	/// <summary>
	/// プレイヤーの攻撃時の関数
	/// </summary>
	void Attack();

	/// <summary>
	/// プレイヤーの特殊攻撃時の関数
	/// </summary>
	void FaceAttack();
	
	/// <summary>
	/// プレイヤーがガード時の関数
	/// </summary>
	void Gard();

	/// <summary>
	/// プレイヤーが回避時の関数
	/// </summary>
	void Avoid();

	/// <summary>
	/// プレイヤーがジャンプする時の関数
	/// </summary>
	void Jump();

	/// <summary>
	/// プレイヤーが顔を使用時の関数
	/// </summary>
	void FaceSelect();

protected:

	std::shared_ptr<PlayerWeapon> m_pWeapon;


	e_PlayerKind m_playerKind;
	
	//仮HP
	int m_hp;

	int m_modelH;

	//プレイヤの座標
	VECTOR m_pos;

	VECTOR m_attackPos;
	VECTOR m_attackDir;
	
	VECTOR m_avoid;
	VECTOR m_move;


	float m_angle;
	float m_radius;

	int m_analogX;
	int m_analogZ;

	bool m_isFaceUse;
	
	int m_frame;

};


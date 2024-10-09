#pragma once
#include "DxLib.h"

class PlayerBase
{
public:

	enum class e_PlayerKind : int
	{
		kNormalPlayer,
		kPowerPlayer,
		kSpeedPlayer,
		kHuckShotPlayer
	};


public:
	PlayerBase();
	virtual ~PlayerBase();

	void Initialize();
	void Finalize();

	void Update();
	void Draw();



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
	/// プレイヤーがガード時の関数
	/// </summary>
	void Gard();

	/// <summary>
	/// プレイヤーが回避時の関数
	/// </summary>
	void Avoid();

	/// <summary>
	/// プレイヤーが顔を使用時の関数
	/// </summary>
	void FaceSelect();



protected:

	int m_hp;

	e_PlayerKind m_playerKind;
	
	int m_modelH;

	//プレイヤの座標
	VECTOR m_pos;
	VECTOR m_pos2;
	
	VECTOR m_attackDir;
	VECTOR m_attackPos;

	VECTOR m_move;

	float m_angle;
	float m_radius;

	int m_analogX;
	int m_analogZ;

	
};


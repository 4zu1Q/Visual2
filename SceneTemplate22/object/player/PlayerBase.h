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
		kPowerPlayer,		//パワー型
		kSpeedPlayer,		//スピード型
		kShotPlayer,		//ショット型
		kStrongestPlayer	//ラスボス型
	};

	//プレイヤーのアニメーションの種類
	enum class e_AnimKind : int
	{

		kNone = -1,				//なし
		kUnknown,				//不明
		kSpawn = 74,			//セレクトシーンで最初の演出用
		kIdle = 41,				//待機
		kWalk = 90,				//歩き
		kRun = 54,				//走り
		kSuperRun = 56,			//走り : Speed型のみ
		kAvoid = 33,			//回避
		kJump = 45,				//ジャンプ
		kHit = 39,				//ダメージが入った時
		kDown = 27,				//プレイヤーのHPが無くなった状態
		kDownPause = 28,		//プレイヤーのHPがなくなった状態のポーズ


		kNormalAttack = 4,				//攻撃 : Normal型
		kNormalSkill = 12,				//スキル : Normal型

		kPowerAttack = 14,				//攻撃 : Power型
		kPowerSkill = 10,				//スキル : Power型

		kSpeedAttack = 36,				//攻撃 : Speed型
		kSpeedSkill = 38,				//スキル : Speed型

		kShotAttack = 16,				//攻撃 : Shot型
		kShotSkill = 8,					//スキル : Shot型

		kStrongestAttack = 11,			//攻撃 : Strongest型
		kStrongestSkill = 14,			//スキル : Strongest型 

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
	const float& GetHp() const { return m_hp; }
	void SetHp(const float hp) { m_hp = hp; }

	//プレイヤーの顔の種類
	const e_PlayerKind& GetFaceKind() const { return m_playerKind; }
	void SetFaceKind(const e_PlayerKind playerKind) { m_playerKind = playerKind; }


	/*プレイヤーがボスを倒したかどうかのフラグを取得する関数*/

	//Power型のBossを倒したかどうかのフラグを取得
	const bool& GetIsPowerFace() const { return m_isPowerFace; }
	void SetIsPowerFace(const bool isPowerFace) { m_isPowerFace = isPowerFace; }

	//Speed型のBossを倒したかどうかのフラグを取得
	const bool& GetIsPowerFace() const { return m_isPowerFace; }
	void SetIsPowerFace(const bool isPowerFace) { m_isPowerFace = isPowerFace; }

	//Shot型のBossを倒したかどうかのフラグを取得
	const bool& GetIsPowerFace() const { return m_isPowerFace; }
	void SetIsPowerFace(const bool isPowerFace) { m_isPowerFace = isPowerFace; }

	//Strongest型のBossを倒したかどうかのフラグを取得
	const bool& GetIsPowerFace() const { return m_isPowerFace; }
	void SetIsPowerFace(const bool isPowerFace) { m_isPowerFace = isPowerFace; }

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

	/// <summary>
	/// アニメーションの更新処理する関数
	/// </summary>
	/// <param name="attachNo">進行させたいアニメーション番号</param>
	/// <param name="startTime"></param>
	/// <returns>ループしたかどうか</returns>
	bool IsUpdateAnim(int attachNo, float startTime = 0.0f);

	/// <summary>
	/// アニメーションの変更する関数
	/// </summary>
	/// <param name="animIndex">変更後のアニメーション番号</param>
	/// <param name="animSpeed"></param>
	void ChangeAnim(int animIndex, float animSpeed = 0.8f);

protected:

	std::shared_ptr<PlayerWeapon> m_pWeapon;

	e_PlayerKind m_playerKind;
	
	//仮HP
	float m_hp;

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

	//顔の保持しているかどうかのフラグ
	bool m_isPowerFace;
	bool m_isSpeedFace;
	bool m_isShotFace;
	bool m_isStrongestFace;

};


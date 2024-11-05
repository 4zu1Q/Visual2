#pragma once
#include "DxLib.h"
#include "myLib/Collidable.h"
#include <memory>

class PlayerWeapon;
class Collidable;
class AnimController;

class Player : public MyLib::Collidable
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

public:

	Player();
	virtual ~Player();

	void Initialize(MyLib::Physics* physics);
	void Finalize();

	void Update();
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

	//プレイヤーのモデルハンドルを取得
	const int& GetModelHandle() const { return m_modelH; }
	void SetModelHandle(const int modelH) { m_modelH = modelH; }

	//プレイヤー下の座標を取得
	const VECTOR& GetPosDown() const { return m_posDown; }
	void SetPosDown(const VECTOR pos) { m_posDown = pos; }

	//プレイヤー上の座標を取得
	const VECTOR& GetPosUp() const { return m_posUp; }
	void SetPosUp(const VECTOR pos) { m_posUp = pos; }

	//プレイヤーの攻撃座標を取得
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }

	//プレイヤーのHPを取得
	const float& GetHp() const { return m_hp; }
	void SetHp(const float hp) { m_hp = hp; }

	//プレイヤーのHPを取得
	const float& GetMp() const { return m_mp; }
	void SetMp(const float mp) { m_mp = mp; }

	//プレイヤーの顔の種類
	const e_PlayerKind& GetFaceKind() const { return m_playerKind; }
	void SetFaceKind(const e_PlayerKind playerKind) { m_playerKind = playerKind; }

	//カメラの方向を取得
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	//攻撃の半径を取得
	const float& GetRadius() const { return m_radius; }

	

	/*プレイヤーがボスを倒したかどうかのフラグを取得する関数*/
	//これ多分必要ない可能性

	//Power型のBossを倒したかどうかのフラグを取得
	const bool& GetIsPowerFace() const { return m_isPowerFace; }
	void SetIsPowerFace(const bool isPowerFace) { m_isPowerFace = isPowerFace; }

	//Speed型のBossを倒したかどうかのフラグを取得
	const bool& GetIsSpeedFace() const { return m_isSpeedFace; }
	void SetIsSpeedFace(const bool isSpeedFace) { m_isSpeedFace = isSpeedFace; }

	//Shot型のBossを倒したかどうかのフラグを取得
	const bool& GetIsShotFace() const { return m_isShotFace; }
	void SetIsShotFace(const bool isShotFace) { m_isShotFace = isShotFace; }

	//Strongest型のBossを倒したかどうかのフラグを取得
	const bool& GetIsStrongestFace() const { return m_isStrongestFace; }
	void SetIsStrongestFace(const bool isStrongestFace) { m_isStrongestFace = isStrongestFace; }

private:

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void WalkUpdate();
	void DashUpdate();
	void JumpUpdate();
	void AirUpdate();
	void AttackXUpdate();
	void AttackYUpdate();
	void HitUpdate();
	void DeadUpdate();
	void DeadPoseUpadte();
	void SpawnUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnWalk();
	void OnDash();
	void OnJump();
	void OnAir();
	void OnAttackX();
	void OnAttackY();
	void OnHit();
	void OnDead();
	void OnDeadPose();
	void OnSpawn();

	//武器を描画するだけの関数
	void WeaponDraw();
	
	/// <summary>
	/// プレイヤーが顔を使用時の関数
	/// </summary>
	void FaceSelect();

private:

	std::shared_ptr<PlayerWeapon> m_pWeapon;
	std::shared_ptr<AnimController> m_pAnim;

	//プレイヤーの顔の種類
	e_PlayerKind m_playerKind;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(Player::*)(/*引数書く*/);
	UpdateFunc_t m_updaFunc;

	//仮HP
	float m_hp;
	float m_mp;
	int m_modelH;
	int m_weaponH;

	//プレイヤの座標
	VECTOR m_posDown;
	VECTOR m_posUp;

	VECTOR m_attackPos;
	VECTOR m_attackDir;

	VECTOR m_avoid;
	VECTOR m_move;
	float m_rate;

	//回転
	float m_angle;
	float m_radius;

	//カメラの回転情報
	float m_cameraAngle;

	//死んだかどうかを判定
	bool m_isDead;

	//動けるか動けないかのフラグ
	bool m_isMove;

	//アナログスティック用の変数
	int m_analogX;
	int m_analogZ;

	//顔を装着しているかしていないかの変数
	bool m_isFaceUse;

	/*アニメーションフラグ*/
	bool m_isAnimIdle;
	bool m_isAnimWalk;
	bool m_isAnimDash;
	bool m_isJump;
	bool m_isAnimAvoid;
	bool m_isAnimAttackX;
	bool m_isAnimAttackY;
	bool m_isAnimDamage;
	bool m_isAnimDown;

	//顔の保持しているかどうかのフラグ
	bool m_isPowerFace;
	bool m_isSpeedFace;
	bool m_isShotFace;
	bool m_isStrongestFace;

};
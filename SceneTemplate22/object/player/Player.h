#pragma once
#include "DxLib.h"
#include "myLib/Collidable.h"
#include <memory>

#include "object/CharaBase.h"

class PlayerWeapon;
class Collidable;
class AnimController;
class Camera;

class WeaponBase;

class Player : public CharaBase
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

	//ボタンの種類
	enum class e_ButtonKind : int
	{
		kNone,		//何もなし
		kAbutton,	//Aボタン
		kBbutton,	//Bボタン
		kXbutton,	//Xボタン
		kYbutton,	//Yボタン

	};

	//後ろのボタンの種類
	enum class e_BackButtonKind : int
	{
		kRBbutton,	//RBボタン
		kLBbutton,	//LBボタン
		kRTbutton,	//RTボタン
		kLTbutton,	//LTボタン
	};


public:

	Player();
	virtual ~Player();

	void Initialize(std::shared_ptr<MyLib::Physics> physics, VECTOR pos, PlayerWeapon& weapon);
	void Finalize(std::shared_ptr<MyLib::Physics> physics);

	void Update(std::shared_ptr<MyLib::Physics> physics, PlayerWeapon& weapon);
	void Draw(PlayerWeapon& weapon);

	// 衝突したとき
	//virtual void OnCollide(const Collidable& colider);

	//プレイヤーのモデルハンドルを取得
	const int& GetModelHandle() const { return m_modelH; }
	void SetModelHandle(const int modelH) { m_modelH = modelH; }

	//プレイヤー下の座標を取得
	const VECTOR& GetPos() const;
	void SetPosDown(const VECTOR pos);

	//プレイヤー上の座標を取得
	const VECTOR& GetPosUp() const { return m_posUp; }
	void SetPosUp(const VECTOR pos) { m_posUp = pos; }

	//プレイヤーの攻撃座標を取得
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }

	/* ステータスの取得 */
	//プレイヤーのHPを取得
	const float& GetHp() const { return m_hp; }

	//プレイヤーのMPを取得
	const float& GetMp() const { return m_mp; }

	//プレイヤーのスタミナを取得
	const float& GetStamina() const { return m_stamina; }
	const bool& GetIsStamina() const { return m_isStamina; }

	//プレイヤーの顔の種類を取得
	const e_PlayerKind& GetFaceKind() const { return m_playerKind; }
	void SetFaceKind(const e_PlayerKind playerKind) { m_playerKind = playerKind; }

	//カメラの方向を取得
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	void SetCameraDirection(VECTOR dir) { m_cameraDirection = dir; }

	//ジャンプフラグを取得
	const bool& GetIsJump() const { return m_isJump; }
	void SetIsJump(const bool isJump) { m_isJump = isJump; }

	//ボタンを押しているかどうかのフラグを取得
	const bool& GetIsButtonPush() const { return m_isButtonPush; }
	//押しているボタンの種類
	const e_ButtonKind& GetButtonKind() const { return m_buttonKind; }
	//顔を使用しているかどうかを取得
	const bool& GetIsFrame() const { return m_isFaceUse; }

	//ゲームオーバーのフラグを取得
	const bool& GetIsGameOver() const { return m_isGameOver; }


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
	void FallUpdate();
	void AttackCharge();
	void AttackXUpdate();
	void AttackYUpdate();
	void HitUpdate();
	void DeadUpdate();
	void SpawnUpdate();

	void FaceUseUpdate();
	void TalkUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnWalk();
	void OnDash();
	void OnJump();
	void OnAir();
	void OnFall();
	void OnAttackCharge();
	void OnAttackX();
	void OnAttackY();
	void OnHit();
	void OnDead();
	void OnSpawn();

	void OnFaceUse();
	void OnTalk();

	//武器を描画するだけの関数
	void WeaponDraw(PlayerWeapon& weapon);
	
	/// <summary>
	/// プレイヤーが顔を使用時の関数
	/// </summary>
	void FaceSelect();

	void CameraUpdate();

	//タイプによってアニメーションを分ける関数
	void AnimChange(const char* normal, const char* power, const char* speed, const char* shot);

private:

	//スマートポインタ
	//std::shared_ptr<PlayerWeapon> m_pWeapon;
	std::shared_ptr<WeaponBase> m_pWeaponBase;
	std::shared_ptr<AnimController> m_pAnim;
	std::shared_ptr<Camera> m_pCamera;


	//プレイヤーの顔の種類
	e_PlayerKind m_playerKind;
	e_ButtonKind m_buttonKind;

	//メンバ関数ポインタ
	using UpdateFunc_t = void(Player::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	//仮
	float m_hp;
	float m_mp;
	float m_stamina;

	bool m_isUseMp;
	bool m_isMp;
	bool m_isStamina;

	int m_weaponH;
	
	//ジャンプ量を見る
	float m_jumpPower;
	int m_jumpCount;

	int m_frame;

	//プレイヤの座標
	//	VECTOR m_posDown;
	VECTOR m_posUp;
	VECTOR m_pos;

	VECTOR m_attackPos;
	VECTOR m_attackDir;

	VECTOR m_cameraToPlayerVec;

	VECTOR m_cameraDirection;

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

	//続けて攻撃するかどうか
	int m_multiAttack;
	bool m_isNextAttackFlag;

	//強攻撃のチャージ時間
	int m_chargeTime;

	//ジャンプ力
	//float m_jumpPower;

	float m_speed;

	//顔を装着しているかしていないかの変数
	//ボタンを押しているかどうか
	bool m_isFaceUse;
	bool m_isButtonPush;

	//ゲームオーバー画面に遷移するためのフラグ
	bool m_isGameOver;

	//ジャンプしているかどうか
	bool m_isJump;

	//顔の保持しているかどうかのフラグ
	bool m_isPowerFace;
	bool m_isSpeedFace;
	bool m_isShotFace;
	bool m_isStrongestFace;

};
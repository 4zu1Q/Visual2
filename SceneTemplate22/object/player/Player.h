#pragma once
#include "DxLib.h"
#include "myLib/Collidable.h"
#include <memory>

#include "object/CharaBase.h"
#include "util/Game.h"

class PlayerWeapon;
class Collidable;
class AnimController;

class Player : public CharaBase
{

public:

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

	void Update(std::shared_ptr<MyLib::Physics> physics, PlayerWeapon& weapon, float cameraAngleX, VECTOR bossPos, bool isLockOn, Game::e_BossAttackKind bossAttackKind);
	void Draw(PlayerWeapon& weapon);

	void BossLook(VECTOR bossPos);

	void HitUpdate(VECTOR hitPos,VECTOR attackPos, VECTOR weaponPos, VECTOR shockPos,float hitRadius ,float attackRadius, float weaponRadius, float shockRadius, bool isBossAttack);

	//プレイヤーのモデルハンドルを取得
	const int& GetModelHandle() const { return m_modelH; }
	void SetModelHandle(const int modelH) { m_modelH = modelH; }

	//プレイヤー下の座標を取得
	const VECTOR& GetPos() const;

	//プレイヤー上の座標を取得
	const VECTOR& GetPosUp() const { return m_posUp; }

	//プレイヤーの攻撃座標を取得
	const VECTOR& GetAttackXPos() const { return m_attackXPos; }
	const VECTOR& GetAttackYPos() const { return m_attackYPos; }
	const VECTOR& GetShockPos() const { return m_attackYPos; }

	//プレイヤーの攻撃半径を取得
	const float& GetAttackXRadius() const { return m_attackXRadius; }
	const float& GetAttackYRadius() const { return m_attackYRadius; }
	const float& GetShockRadius() const { return m_attackShockRadius; }

	//攻撃をしたかどうかを取得
	const bool& GetIsAttack() const { return m_isAttack; }

	//プレイヤーがどの攻撃をしたかを取得
	const Game::e_PlayerAttackKind& GetAttackKind() const { return m_attackKind; }

	/* ステータスの取得 */
	//プレイヤーのHPを取得
	const float& GetHp() const { return m_hp; }

	//プレイヤーのMPを取得
	const float& GetMp() const { return m_mp; }

	//プレイヤーのスタミナを取得
	const float& GetStamina() const { return m_stamina; }
	const bool& GetIsStamina() const { return m_isStamina; }

	//プレイヤーの顔の種類を取得
	const Game::e_PlayerKind& GetFaceKind() const { return m_playerKind; }
	void SetFaceKind(const Game::e_PlayerKind playerKind) { m_playerKind = playerKind; }

	//カメラの方向を取得
	void SetCameraDirection(VECTOR dir) { m_cameraDirection = dir; }
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }

	//ジャンプフラグを取得
	const bool& GetIsJump() const { return m_pColliderData->IsGround(); }

	//ボタンを押しているかどうかのフラグを取得
	const bool& GetIsButtonPush() const { return m_isButtonPush; }
	//押しているボタンの種類
	const e_ButtonKind& GetButtonKind() const { return m_buttonKind; }
	//顔を使用しているかどうかを取得
	const bool& GetIsFaceUse() const { return m_isFaceUse; }

	//ゲームオーバーのフラグを取得
	const bool& GetIsGameOver() const { return m_isGameOver; }

	//半径の取得
	const float& GetRadius() const;
	const float& GetAngle() const { return m_angle; }


private:

	//プレイヤーの攻撃更新処理
	void CollisionPosUpdate();
	//ダメージ更新処理
	void DamageUpdate();
	//プレイヤーのステータス更新処理
	void StatusUpdate();

	//プレイヤーの回転処理
	void PlayerSetPosAndRotation(VECTOR pos, float angle);
	//プレイヤーの移動
	void Move();
	//ボスの攻撃の当たり判定
	void Hit();

	// プレイヤーが顔を使用時の関数
	void FaceSelect();
	//プレイヤーがアイテムを使うときの関数
	void FaceUse();

	//武器を描画するだけの関数
	void WeaponDraw(PlayerWeapon& weapon);
	//プレイヤーの種類で描画を変える関数
	void PlayerDraw();

	//攻撃半径と攻撃の距離をセットする関数
	void SetAttackCollision(float attackXRadius, float attackYRadius, float attackShockRadius, float attackXMoveScale, float attackYMoveScale);

	//攻撃判定
	bool IsAttackHit(VECTOR attackPos, float radius);
	bool IsWeaponHit(VECTOR attackPos, float radius);
	bool IsShockAttackHit(VECTOR attackPos, float radius);

	//プレイヤーの攻撃が当たっていたら
	bool IsPlayerAttackHit(VECTOR attackPos, float attackRadius);

	/*各々の状態のアップデート処理*/
	void IdleUpdate();
	void LockOnIdleUpdate();
	void WalkUpdate();
	void LockOnWalkUpdate();
	void DashUpdate();
	void JumpUpdate();
	void DashJumpUpdate();
	void FallUpdate();
	void DashFallUpdate();
	void AttackCharge();
	void AttackXUpdate();
	void AttackYUpdate();
	void HitOneDamageUpdate();
	void HitTwoDamageUpdate();
	void DeadUpdate();
	void FaceChangeUpdate();
	void FaceUseUpdate();

	void TalkUpdate();

	/*アップデート処理に移動させるための関数*/
	void OnIdle();
	void OnLockOnIdle();
	void OnWalk();
	void OnLockOnWalk();
	void OnDash();
	void OnJump();
	void OnDashJump();
	void OnAir();
	void OnDashAir();
	void OnFall();
	void OnDashFall();
	void OnAttackCharge();
	void OnAttackX();
	void OnAttackY();
	void OnHitOneDamage();
	void OnHitTwoDamage();
	void OnDead();
	void OnFaceChange();
	void OnFaceUse();

	/* On関数に入る時にセットする関数 */
	void SetOnReset();	//状態遷移した時に値をリセットさせる関数
	void SetOnDamage();	//ダメージを受けた時のセット関数
	void SetOnJump();		//ジャンプをした時のセット関数

private:

	//メンバ関数ポインタ
	using UpdateFunc_t = void(Player::*)(/*引数書く*/);
	UpdateFunc_t m_updateFunc;

	//スマートポインタ
	std::shared_ptr<AnimController> m_pAnim;


	/* プレイヤーの情報 */

	//モデルハンドル
	int m_modelPowerH;
	int m_modelSpeedH;
	int m_modelShotH;
	int m_modelRassH;
	int m_shadowH;

	//ステータス
	float m_hp;
	float m_mp;
	float m_stamina;

	bool m_isUseMp;	 //一回だけ使うため
	bool m_isMp;	 //MPを使い切ったかどうか
	bool m_isStamina;//スタミナを使い切ったかどうか
	bool m_isDash;	 //ダッシュしているかどうか
	bool m_isLockOn; //ロックオンしてるかどうか
	bool m_isGameOver;	//ゲームオーバー画面に遷移するためのフラグ

	//ジャンプ量を見る
	float m_jumpPower;
	int m_jumpCount;

	//プレイヤの座標
	VECTOR m_posUp;
	VECTOR m_pos;
	VECTOR m_hitPos;
	
	VECTOR m_move;
	float m_angle;		//回転

	int m_moveCount;	//動いたときの音を出すカウント
	int m_damageFrame;	//無敵時間の時の点滅時間

	int m_deadTime;
	bool m_isDead;		//死んだかどうかを判定

	//攻撃座標
	VECTOR m_attackXPos;
	VECTOR m_attackYPos;
	VECTOR m_attackDir;		//攻撃の向き
	VECTOR m_attackMove;	//プレイヤーから攻撃座標の距離

	bool m_isHitDamage;	//ボスの攻撃に当たったかどうか
	bool m_isAttack;	//攻撃したかどうか
	int m_attackFrame;

	int m_chargeTime;	//強攻撃のチャージ時間

	//続けて攻撃するかどうか
	int m_multiAttack;
	bool m_isNextAttackFlag;

	//攻撃の種類
	Game::e_PlayerAttackKind m_attackKind;

	//半径
	float m_radius;
	float m_hitRadius;			//プレイヤーが敵の攻撃にヒットする当たり判定の半径
	float m_attackXRadius;		//プレイヤーの通常攻撃半径
	float m_attackYRadius;		//プレイヤーの特殊攻撃半径
	float m_attackShockRadius;	//プレイヤーの衝撃攻撃半径
	
	//プレイヤーの顔の種類
	Game::e_PlayerKind m_playerKind;
	bool m_isFaceUse;		//仮面を使っているかどうか

	//押しているボタンの種類
	e_ButtonKind m_buttonKind;
	bool m_isButtonPush;	//ボタンを押しているかどうか

	/* ボスの情報 */

	VECTOR m_bossPos;
	VECTOR m_bossToPlayerVec; //ボスからプレイヤー

	//ボスの攻撃座標
	VECTOR m_bossHitPos;
	VECTOR m_bossAttackPos;
	VECTOR m_bossWeaponPos;
	VECTOR m_bossShockPos;

	//ボスの円の半径を代入するための変数
	float m_bossHitRadius;
	float m_bossAttackRadius;
	float m_bossWeaponRadius;
	float m_bossShockRadius;

	bool m_isBossAttack;	//ボスがアタックしたかどうか

	Game::e_BossAttackKind m_bossAttackKind;	//ボスの攻撃種類

	/* カメラの情報 */

	VECTOR m_cameraDirection;
	//カメラの回転情報
	float m_cameraAngle;

	/* Pad情報 */

	//アナログスティック用の変数
	int m_analogX;
	int m_analogZ;

	//RT用のインプット
	DINPUT_JOYSTATE m_input;

};

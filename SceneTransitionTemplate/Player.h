#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	virtual ~Player();

	void Load();
	void Delete();

	void Init();
	void Update();
	void Draw();

	//カメラの方向を取得
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }
	//半径の取得
	float GetRadius() { return m_radius; }
	//スキルの半径
	float GetSkillRadius() { return m_skillRadius; }
	//攻撃の半径
	float GetAttackRadius() { return m_attackRadius; }

	//プレイヤーの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//プレイヤーの攻撃座標を取得
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }


	//プレイヤーのHPを取得
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//プレイヤーの攻撃の当たり判定を生成するかのフラグを取得
	const bool& GetAttackGeneration() const { return m_isAttackGeneration; }
	const bool& GetSkillGeneration() const { return m_isSkillGeneration; }
	

	//プレイヤーの攻撃の座標を取得
	void OnGimmickHitUpdate();

	//プレイヤーダメージのフラグ取得
	void SetDamage(const bool damage) { m_isDamage = damage; }

	//アニメーションのセットフラグ
	void SetAnimDamage(const bool animDamage);
	void SetAnimDown(const bool animDown) { m_isAnimDown = animDown; }

	//プレイヤーのアニメーション種別
	enum class AnimKind : int
	{
		kNone = -1,	//なし
		kUnknown,	//不明
		kIdle,		//待機
		kWalk,		//歩き
		kRun,		//走り
		kAttack,	//攻撃
		kSkill,		//スキル
		kJump,		//ジャンプ
		kBigJump,	//大ジャンプ
		kFall,		//落下中
		kDamage,	//ダメージ
		kStop,		//立ち止まり
	};

private:
	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

	void ShaderLoad();
	void ShaderUpdate();

	//移動関数
	void Move();

	void ShadowMapLoad();
	void ShadowMapInit();
	void ShadowMapUpdate();
	void ShadowMapDraw(int handle);
	void ShadowMapDelete(int handle);

private:
	//モデルハンドル
	int m_modelH;

	//画像ハンドル
	int m_hpH;
	int m_losthpH;
	int m_hpFlameH;

	//サウンドハンドル
	int m_damageSeH;
	int m_axeAttackSeH;
	int m_axeSkillSeH;

	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;

	//HP
	int m_hp;
	int m_losthp;

	//スタミナ
	float m_stamina;
	bool m_isStamina;

	float m_analogX;
	float m_analogZ;

	//表示情報
	VECTOR m_pos;
	VECTOR m_attackPos;
	VECTOR m_attackDir;

	float m_angle;

	//攻撃中フラグ
	bool m_isAttack;
	bool m_isSkill;

	bool m_isWalk;
	bool m_isDash;
	bool m_isDown;
	bool m_isAvoid;

	//アニメーション情報
	int m_animIndex;
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合

	//アニメーションフラグ
	bool m_isAnimAttack;
	bool m_isAnimWalk;
	bool m_isAnimDash;
	bool m_isAnimDamage;
	bool m_isAnimDown;

	//当たり判定の半径
	float m_radius;
	float m_skillRadius;
	float m_attackRadius;

	//当たり判定の発生フラグ
	bool m_isAttackGeneration;
	bool m_isSkillGeneration;

	//カメラ情報
	float m_cameraAngle;

	int m_currentPlayAnim;
	float m_currentAnimCount;

	int m_prevPlayAnim;
	float m_prevAnimCount;
	//float m_animBlendRate;
	bool m_isMove;

	float m_hpAnimationHeight;

	//プレイヤーの点滅
	int m_damageFrame;
	bool m_isDamage;

	//State m_currentState;
	AnimKind m_animKind;

	//シャドウマップ変数
	int m_shadowMapH;
	float m_shadowAngle;
	VECTOR m_lightDirection;


};


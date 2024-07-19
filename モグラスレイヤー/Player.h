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

	//プレイヤーの座標を取得
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//プレイヤーの攻撃の座標を取得


private:
	//アニメーションの進行
	//ループしたかどうかを返す
	bool UpdateAnim(int attachNo);

	//アニメーションの変更
	void ChangeAnim(int animIndex);

	void ShaderLoad();
	void ShaderUpdate();

private:
	//モデルハンドル
	int m_modelHandle;
	//シェーダハンドル
	int m_outlineVsH;
	int m_outlinePsH;



	//表示情報
	VECTOR m_pos;
	VECTOR m_attackPos;

	float m_angle;

	//攻撃中フラグ
	bool m_isAttack;
	bool m_isJump;

	//アニメーション情報
	int m_currentAnimNo;	//現在のアニメーション
	int m_prevAnimNo;		//変更前のアニメーション
	float m_animBlendRate;	//アニメーション合成割合
	//0.0f:prevが再生される
	//1.0f:currentが再生される

	//当たり判定の半径
	float m_radius;

	//カメラ情報
	float m_cameraAngle;


};



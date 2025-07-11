#pragma once
#include "DxLib.h"

class LockOnTargetUi
{
public:
	LockOnTargetUi();
	virtual ~LockOnTargetUi();

	void Update(VECTOR pos);
	void Draw();

	//ロックオンが解除されたとき
	void CancelLockOn();

private:
	void FirstUpdate();		//ロックオンが始まってすぐの更新処理
	void NormalUpdate();	//ロックオン時の更新処理
private:
	//ロックオンした敵の座標
	VECTOR m_pos;
	//ロックオン画像
	int m_handle;
	//画像サイズ
	float m_scale;
	//上下移動のための変数
	float m_sinRate;
	//敵の高さ(身長)
	float m_height;
	//上
	float m_upVec;
	//画像のX軸の大きさ
	int m_graphX;
	//画像のY軸の大きさ
	int m_graphY;

	//メンバ関数ポインタ
	void (LockOnTargetUi::* m_updateFunc)();
};


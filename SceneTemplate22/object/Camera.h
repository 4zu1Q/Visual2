#pragma once
#include "DxLib.h"
#include <memory>
#include<map>
#include<string>
#include<unordered_map>

class LockOnTargetUi;

class Camera
{
public:

	Camera();
	virtual ~Camera();

	void Initialize(VECTOR playerPos);
	void Finalize();

	void Update(VECTOR playerPos, VECTOR enemyPos, int stageHandle, float playerAngle, bool isLockOn);
	void Draw();

	const bool GetIsLockOn() { return m_isLockOn; }

	const VECTOR GetDirection() const;

	float GetCameraAngleX() { return m_angleH; }

	void ResetToPlayerView(float playerAngle);
	void LockOn(VECTOR playerPos, VECTOR enemyPos);

private:

	void UpdateLockOnCamera();
	void UpdateCameraAngle();

private:

	//スマートポインタ
	std::shared_ptr<LockOnTargetUi> m_pLockOnUi;

	int m_lightHandle;		//ライトハンドル

	VECTOR m_pos;				//カメラ座標
	VECTOR m_targetPos;			//注視点
	VECTOR m_playerPos;       // プレイヤーの位置
	VECTOR m_enemyPos;        // 敵の位置


	float m_angleH;		//水平角度 弧度法
	float m_angleV;		//垂直角度 弧度法

	float m_cameraDistance;	//カメラとプレイヤーの距離

	VECTOR m_setEye;
	VECTOR m_setTarget;

	bool m_isLockOn; //ロックオンしている場合
	bool m_isReset;	//カメラリセットしている場合

};


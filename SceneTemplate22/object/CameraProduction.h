#pragma once
#include "DxLib.h"
#include "util/Game.h"

class CameraProduction
{
public:

	CameraProduction();
	virtual ~CameraProduction();

	void Initialize(VECTOR playerPos, Game::e_PlayerProduction playerPro);
	void Finalize();

	void Update(VECTOR playerPos, Game::e_PlayerProduction playerPro);
	void Draw();

	const VECTOR GetDirection() const;

	float GetCameraAngleX() { return m_angleH; }

private:

	VECTOR m_pos;				//座標
	VECTOR m_targetPos;			//注視点
	float m_angleH;		//水平角度 弧度法
	float m_angleV;		//垂直角度 弧度法

	float m_cameraAnimation;

	VECTOR m_setEye;
	VECTOR m_setTarget;

	int m_lightHandle;		//ライトハンドル

	float m_angleMoveScale;
};


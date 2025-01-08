#pragma once
#include "DxLib.h"
#include <memory>


class Camera2
{
public:

	Camera2();
	virtual ~Camera2();

	void Initialize(VECTOR playerPos);
	void Finalize();

	void Update(VECTOR playerPos, int stageHandle);
	void Draw();

	const VECTOR GetDirection() const;

	float GetCameraAngleX() { return m_angleH; }

private:

	VECTOR m_pos;				//座標
	VECTOR m_targetPos;			//注視点
	float m_angleH;		//水平角度 弧度法
	float m_angleV;		//垂直角度 弧度法

	VECTOR m_setEye;
	VECTOR m_setTarget;

	int m_lightHandle;		//ライトハンドル

	float m_angleMoveScale;

};


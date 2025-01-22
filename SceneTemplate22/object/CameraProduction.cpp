#include "DxLib.h"

#include "object/CameraProduction.h"

#include <memory>

#define _USE_MATH_DEFINES
#include <math.h>
#define USE_LERP_CAMERA 1

namespace
{
	constexpr float kCameraAngleSpeed = 0.05f;				//旋回速度
	constexpr float kCameraPlayerTargetHeight = 10.0f;		// プレイヤー座標からどれだけ高い位置を注視点とするか

	constexpr float kTitleCameraPlayerBackwardDistance = 25.0f;	// プレイヤーとの奥行方向の距離
	constexpr float kGameOverCameraPlayerBackwardDistance = 20.0f;	// プレイヤーとの奥行方向の距離
	constexpr float kGameClearCameraPlayerBackwardDistance = 23.0f;	// プレイヤーとの奥行方向の距離

	constexpr float kTitleCameraPlayerRightDistance = -18.0f;		// プレイヤーとの水平方向の距離
	constexpr float kGameOverCameraPlayerRightDistance = 0.0f;		// プレイヤーとの水平方向の距離
	constexpr float kGameClearCameraPlayerRightDistance = -0.0;	// プレイヤーとの水平方向の距離

	constexpr float kCameraCollisionSize = 3.0f;			// カメラの当たり判定サイズ

	constexpr float kSelectSpeed = 0.01f;
	constexpr float kSelectAnimationSize = 1.0f;

	constexpr float kCameraNear = 5.0f;
	constexpr float kCameraFar = 5000.0f;
}

CameraProduction::CameraProduction():
	m_pos(VGet(0, 0, 0)),
	m_targetPos(VGet(0, 0, 0)),
	m_setEye(VGet(0, 0, 0)),
	m_setTarget(VGet(0, 0, 0)),
	m_angleH(0.0f),
	m_angleV(0.0f),
	m_lightHandle(0),
	m_angleMoveScale(0.0f),
	m_cameraAnimation(0.0f)
{
	//カメラ
	SetCameraNearFar(0.1f, 1000.0f);

	// カメラの初期水平角度は１８０度
	m_angleH = DX_PI_F;

	// 垂直角度は０度
	m_angleV = 0.0f;

	// FOV(視野角)を60度に
	SetupCamera_Perspective(60.0f * (static_cast<float>(M_PI) / 180.0f));
}

CameraProduction::~CameraProduction()
{
	DeleteLightHandle(m_lightHandle);
}

void CameraProduction::Initialize(VECTOR playerPos, Game::e_PlayerProduction playerPro)
{
	m_lightHandle = CreateDirLightHandle(VSub(m_targetPos, m_pos));
	SetCameraNearFar(kCameraNear, kCameraFar);

}

void CameraProduction::Finalize()
{


}

void CameraProduction::Update(VECTOR playerPos, Game::e_PlayerProduction playerPro)
{


	// カメラの初期水平角度は１８０度
	m_angleH = DX_PI_F;

	// 垂直角度は０度
	m_angleV = 0.0f;

	// カメラの注視点はプレイヤー座標から規定値分高い座標
	m_targetPos = VAdd(playerPos, VGet(0.0f, kCameraPlayerTargetHeight, 0.0f));

	VECTOR rightVector;
	VECTOR forwardVector;
	VECTOR camPosBase;

	// カメラの奥行方向を算出
	forwardVector.x = cos(m_angleV) * cos(-m_angleH);
	forwardVector.y = sin(m_angleV);
	forwardVector.z = cos(m_angleV) * sin(-m_angleH);

	// カメラの水平方向を算出
	rightVector.x = cos(-m_angleH - DX_PI_F / 2.0f);
	rightVector.y = 0.0f;
	rightVector.z = sin(-m_angleH - DX_PI_F / 2.0f);

	// カメラの座標を算出
	if (playerPro == Game::e_PlayerProduction::kTitle)
	{
		m_pos = VAdd(VAdd(playerPos, VAdd(VScale(rightVector, kTitleCameraPlayerRightDistance), VGet(0,0, m_cameraAnimation))), VScale(forwardVector, -kTitleCameraPlayerBackwardDistance));
	}
	else if (playerPro == Game::e_PlayerProduction::kGameOver)
	{
		m_pos = VAdd(VAdd(playerPos, VScale(rightVector, kGameOverCameraPlayerRightDistance)), VScale(forwardVector, -kGameOverCameraPlayerBackwardDistance));
	}
	else if (playerPro == Game::e_PlayerProduction::kGameClear)
	{
		m_pos = VAdd(VAdd(playerPos, VScale(rightVector, kGameClearCameraPlayerRightDistance)), VScale(forwardVector, -kGameClearCameraPlayerBackwardDistance));
	}

	m_pos.y += kCameraPlayerTargetHeight;

	// カメラの注視点を算出
	if (playerPro == Game::e_PlayerProduction::kTitle)
	{
		m_targetPos = VAdd(playerPos, VScale(rightVector, kTitleCameraPlayerRightDistance));
	}
	else if (playerPro == Game::e_PlayerProduction::kGameOver)
	{
		m_targetPos = VAdd(playerPos, VScale(rightVector, kGameOverCameraPlayerRightDistance));
	}
	else if (playerPro == Game::e_PlayerProduction::kGameClear)
	{
		m_targetPos = VAdd(playerPos, VScale(rightVector, kGameClearCameraPlayerRightDistance));
	}
	m_targetPos.y += kCameraPlayerTargetHeight;

	// カメラの情報をライブラリのカメラに反映させる
	m_setEye = VAdd(m_setEye, VScale(VSub(m_pos, m_setEye), 1.0f));
	m_setTarget = VAdd(m_setTarget, VScale(VSub(m_targetPos, m_setTarget), 1.0f));
	SetLightDirectionHandle(m_lightHandle, VSub(playerPos, m_pos));
	SetCameraPositionAndTarget_UpVecY(m_setEye, m_setTarget);

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_cameraAnimation = sinf(SinCount) * kSelectAnimationSize;
}

void CameraProduction::Draw()
{
}

const VECTOR CameraProduction::GetDirection() const
{
	return VECTOR();
}

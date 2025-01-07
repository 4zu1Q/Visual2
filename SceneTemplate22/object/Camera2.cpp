#include "Camera2.h"


namespace 
{
	constexpr float kCameraAngleSpeed = 0.05f;				//旋回速度
	constexpr float kCameraPlayerTargetHeight = 15.0f;		// プレイヤー座標からどれだけ高い位置を注視点とするか
	constexpr float kCameraPlayerBackwardDistance = 25.0f;	// プレイヤーとの奥行方向の距離
	constexpr float kCameraPlayerRightDistance = 0.0f;	// プレイヤーとの水平方向の距離
	constexpr float kCameraCollisionSize = 5.0f;			// カメラの当たり判定サイズ

	constexpr float kCameraNear = 5.0f;
	constexpr float kCameraFar = 5000.0f;
}

Camera2::Camera2():
	m_pos(VGet(0, 0, 0)),
	m_targetPos(VGet(0, 0, 0)), 
	m_setEye(VGet(0, 0, 0)),
	m_setTarget(VGet(0, 0, 0)),
	m_angleH(0.0f),
	m_angleV(0.0f),
	m_lightHandle(0)
{
}

Camera2::~Camera2()
{
}

void Camera2::Initialize(VECTOR playerPos)
{
	m_lightHandle = CreateDirLightHandle(VSub(m_targetPos, m_pos));
	SetCameraNearFar(kCameraNear, kCameraFar);

	// カメラの初期水平角度は１８０度
	m_angleH = DX_PI_F;

	// 垂直角度は０度
	m_angleV = 0.0f;

}

void Camera2::Finalize()
{
}

void Camera2::Update(VECTOR playerPos, int stageHandle)
{
	// DirectInput の入力を取得
	DINPUT_JOYSTATE DInputState;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &DInputState);

	// 右スティックの入力に沿ってカメラを旋回させる( Xbox360 コントローラ用 )
	m_angleH += DInputState.Rx / 1000.0f * kCameraAngleSpeed;
	if (m_angleH < -DX_PI_F)
	{
		m_angleH += DX_TWO_PI_F;
	}
	if (m_angleH > DX_PI_F)
	{
		m_angleH -= DX_TWO_PI_F;
	}

	m_angleV += DInputState.Ry / 1000.0f * 0.02f;
	if (m_angleV < -DX_PI_F / 2.0f + 0.6f)
	{
		m_angleV = -DX_PI_F / 2.0f + 0.6f;
	}
	if (m_angleV > DX_PI_F / 2.0f - 0.6f)
	{
		m_angleV = DX_PI_F / 2.0f - 0.6f;
	}

	// カメラの注視点はプレイヤー座標から規定値分高い座標
	m_targetPos = VAdd(playerPos, VGet(0.0f, kCameraPlayerTargetHeight, 0.0f));

	// カメラの座標を決定する
	{
		MV1_COLL_RESULT_POLY_DIM hRes;
		int hitNum;
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
		m_pos = VAdd(VAdd(playerPos, VScale(rightVector, kCameraPlayerRightDistance)), VScale(forwardVector, -kCameraPlayerBackwardDistance));
		m_pos.y += kCameraPlayerTargetHeight;

		// カメラの注視点を算出
		m_targetPos = VAdd(playerPos, VScale(rightVector, kCameraPlayerRightDistance));
		m_targetPos.y += kCameraPlayerTargetHeight;

		// カメラの座標からプレイヤーの間にステージのポリゴンがあるか調べる
		camPosBase = playerPos;
		camPosBase.y += kCameraPlayerTargetHeight;
		hRes = MV1CollCheck_Capsule(stageHandle, -1, camPosBase, m_pos, kCameraCollisionSize);
		hitNum = hRes.HitNum;
		MV1CollResultPolyDimTerminate(hRes);
		if (hitNum != 0)
		{
			float notHitLength;
			float hitLength;
			float testLength;
			VECTOR TestPosition;
			VECTOR Direction;

			// あったら無い位置までプレイヤーに近づく

			// ポリゴンに当たらない距離をセット
			notHitLength = 0.0f;

			// ポリゴンに当たる距離をセット
			hitLength = VSize(VSub(m_pos, camPosBase));

			// プレイヤーからカメラへの方向を算出
			Direction = VNorm(VSub(m_pos, camPosBase));
			do
			{
				// 当たるかどうかテストする距離をセット( 当たらない距離と当たる距離の中間 )
				testLength = notHitLength + (hitLength - notHitLength) / 2.0f;

				// テスト用のカメラ座標を算出
				TestPosition = VAdd(VScale(Direction, testLength), camPosBase);

				// 新しい座標で壁に当たるかテスト
				hRes = MV1CollCheck_Capsule(stageHandle, -1, camPosBase, TestPosition, kCameraCollisionSize);
				hitNum = hRes.HitNum;
				MV1CollResultPolyDimTerminate(hRes);
				if (hitNum != 0)
				{
					// 当たったら当たる距離を testLength に変更する
					hitLength = testLength;
				}
				else
				{
					// 当たらなかったら当たらない距離を testLength に変更する
					notHitLength = testLength;
				}

				// HitLength と NoHitLength が十分に近づいていなかったらループ
			} while (hitLength - notHitLength > 0.1f);

			// カメラの座標をセット
			m_pos = TestPosition;
		}
	}

	// カメラの情報をライブラリのカメラに反映させる
	m_setEye = VAdd(m_setEye, VScale(VSub(m_pos, m_setEye), 0.2f));
	m_setTarget = VAdd(m_setTarget, VScale(VSub(m_targetPos, m_setTarget), 0.2f));
	SetLightDirectionHandle(m_lightHandle, VSub(playerPos, m_pos));
	SetCameraPositionAndTarget_UpVecY(m_setEye, m_setTarget);


}

void Camera2::Draw()
{
}

const VECTOR Camera2::GetDirection() const
{
	return VNorm(VSub(m_targetPos, m_pos));
}

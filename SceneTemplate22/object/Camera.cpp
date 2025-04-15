#include "Camera.h"

#include "ui/LockOnTargetUi.h"

#include "util/Setting.h"
#include "util/Pad.h"
#include "util/SoundManager.h"

namespace 
{
	constexpr float kCameraAngleSpeed = 0.05f;				//旋回速度
	constexpr float kCameraPlayerTargetHeight = 10.0f;		// プレイヤー座標からどれだけ高い位置を注視点とするか
	constexpr float kCameraPlayerBackwardDistance = 25.0f;	// プレイヤーとの奥行方向の距離
	constexpr float kCameraTargetBackwardDistance = 25.0f;	// プレイヤーとの奥行方向の距離
	constexpr float kCameraPlayerRightDistance = 0.0f;		// プレイヤーとの水平方向の距離
	constexpr float kCameraCollisionSize = 2.0f;			// カメラの当たり判定サイズ

	constexpr float kCameraNear = 5.0f;
	constexpr float kCameraFar = 5000.0f;

	constexpr int kTriggerDeadZone = 60;

	//LST押し込みの情報
	constexpr int kPadButtonLStick = 0x00001000;


	constexpr float kInitAngle = 80.0f;
	constexpr float kHalf = 0.5f;

	constexpr float kCameraUpPos = 20.0f;
	
	constexpr float kSmashNum = 2.0f;

	constexpr float kInputNum = 10000.0f;
	constexpr float kAngleNum = 0.6f;

	constexpr float kEasing = 0.4f;
	constexpr float kAddAngle = 1.6f;

	constexpr float kLength = 0.1f;

	constexpr float kAngleVMax = -0.9f;
	constexpr float kAngleVMin = 0.6f;
	constexpr float kSetAngleVMax = -0.89f;
	constexpr float kSetAngleVMin = 0.59f;

	//アナログスティック関連
	constexpr float kAnalogRangeMin = 0.2f;		//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8f;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

}

Camera::Camera():
	m_pos(VGet(0, 0, 0)),
	m_playerPos(VGet(0, 0, 0)),
	m_enemyPos(VGet(0, 0, 0)),
	m_targetPos(VGet(0, 0, 0)), 
	m_setEye(VGet(0, 0, 0)),
	m_setTarget(VGet(0, 0, 0)),
	m_angleH(0.0f),
	m_angleV(0.0f),
	m_lightHandle(0),
	m_cameraDistance(35.0f),
	m_isLockOn(false),
	m_isReset(false)
{
	m_pLockOnUi = std::make_shared<LockOnTargetUi>();
}

Camera::~Camera()
{
	DeleteLightHandle(m_lightHandle);
}

void Camera::Initialize(VECTOR playerPos)
{
	m_lightHandle = CreateDirLightHandle(VSub(m_targetPos, m_pos));
	SetCameraNearFar(kCameraNear, kCameraFar);

	// カメラの初期水平角度は180度
	m_angleH = 0.0f;

	// 垂直角度は0度
	m_angleV = 0.0f;
}

void Camera::Finalize()
{
}

void Camera::Update(VECTOR playerPos, VECTOR enemyPos, int stageHandle, float playerAngle, bool isLockOn)
{
	// DirectInput の入力を取得
	DINPUT_JOYSTATE dInputState;
	GetJoypadDirectInputState(DX_INPUT_PAD1, &dInputState);

	m_playerPos = playerPos;
	m_enemyPos = enemyPos;

	m_pLockOnUi->Update(m_enemyPos);

	//プレイヤーとボスの距離を距離を求める
	VECTOR toPlayer = VSub(m_enemyPos,m_playerPos);
	float length = VSize(toPlayer);

	float leftTriggerValue = Pad::GetLeftTrigger();

	//RSTを押し込んだ場合
	if (Pad::IsTrigger(PAD_INPUT_10) && !m_isLockOn)
	{
		SoundManager::GetInstance().PlaySe("cameraResetSe");
		ResetToPlayerView(playerAngle);
	}

	//LSTを押し込んだ場合
	if (Pad::IsTrigger(kPadButtonLStick) && isLockOn)
	{
		SoundManager::GetInstance().PlaySe("lockOnSe");
		m_isLockOn = !m_isLockOn;
	}

	//ロックオンが解除された場合
	if (!isLockOn)
	{
		m_isLockOn = false;
	}

	//ロックオンが解除された場合
	if (!m_isLockOn)
	{
		m_pLockOnUi->CancelLockOn();
	}

	//ロックオンされた場合のカメラ処理
	if (m_isLockOn)			
	{
		// プレイヤーと敵の中間点を計算
		VECTOR midPoint = VGet(
			(m_playerPos.x + m_enemyPos.x) / kSmashNum,
			(m_playerPos.y + m_enemyPos.y) / kSmashNum,
			(m_playerPos.z + m_enemyPos.z) / kSmashNum);

		// カメラの位置を更新
		m_pos = VGet(
			m_playerPos.x - m_cameraDistance * cosf(m_angleV) * sinf(m_angleH),
			m_playerPos.y * kHalf + kCameraUpPos,
			m_playerPos.z - m_cameraDistance * cosf(m_angleV) * cosf(m_angleH));

		// カメラの注視点を更新
		m_targetPos = midPoint;

		VECTOR direction = VSub(m_enemyPos, m_playerPos);
		m_angleH = atan2f(direction.x, direction.z);
		m_angleV = atan2f(direction.y, VSize(VGet(direction.x, 0.0f, direction.z)));

	}
	else if (!m_isLockOn)	//ロックオンされていない場合のカメラ処理
	{


		// 右スティックの入力に沿ってカメラを旋回させる( Xbox360 コントローラ用 )
		m_angleH += dInputState.Rx / kInputNum * Setting::GetInstance().GetSensitivity();

		//設定で上下反転変えられるようにする
		if (Setting::GetInstance().GetIsFlipSideUp())
		{
			m_angleV -= dInputState.Ry / kInputNum * Setting::GetInstance().GetSensitivity() * kHalf;
		}
		else if (!Setting::GetInstance().GetIsFlipSideUp())
		{
			m_angleV += dInputState.Ry / kInputNum * Setting::GetInstance().GetSensitivity() * kHalf;
		}

		if (m_angleV < -DX_PI_F / kSmashNum + kAngleNum)
		{
			m_angleV = -DX_PI_F / kSmashNum - kAngleNum;
		}
		if (m_angleV > DX_PI_F / kSmashNum - kAngleNum)
		{
			m_angleV = DX_PI_F / kSmashNum + kAngleNum;
		}

		// カメラの水平角度を制限
		if (m_angleV < kAngleVMax)
		{
			m_angleV = kSetAngleVMax;
		}
		// カメラの水平角度を制限
		if (m_angleV > kAngleVMin)
		{
			m_angleV = kSetAngleVMin;
		}

		// カメラの注視点はプレイヤー座標から規定値分高い座標
		m_targetPos = VAdd(playerPos, VGet(0.0f, kCameraPlayerTargetHeight, 0.0f));
	}

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
		rightVector.x = cos(-m_angleH - DX_PI_F / kSmashNum);
		rightVector.y = 0.0f;
		rightVector.z = sin(-m_angleH - DX_PI_F / kSmashNum);

		if (!m_isLockOn)
		{
			// カメラの座標を算出
			m_pos = VAdd(VAdd(playerPos, VScale(rightVector, kCameraPlayerRightDistance)), VScale(forwardVector, -kCameraPlayerBackwardDistance));
			m_pos.y += kCameraPlayerTargetHeight;

			// カメラの注視点を算出
			m_targetPos = VAdd(playerPos, VScale(rightVector, kCameraPlayerRightDistance));
			m_targetPos.y += kCameraPlayerTargetHeight;
		}

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
				testLength = notHitLength + (hitLength - notHitLength) / kSmashNum;

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
			} while (hitLength - notHitLength > kLength);

			// カメラの座標をセット
			m_pos = TestPosition;
		}
	}

	// カメラの情報をライブラリのカメラに反映させる
	if (!m_isLockOn)
	{
		m_setEye = VAdd(m_setEye, VScale(VSub(m_pos, m_setEye), kEasing));
		m_setTarget = VAdd(m_setTarget, VScale(VSub(m_targetPos, m_setTarget), kEasing));
		SetCameraPositionAndTarget_UpVecY(m_setEye, m_setTarget);
	}
	else if (m_isLockOn)
	{
		// カメラの位置と注視点を設定
		SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	}

	SetLightDirectionHandle(m_lightHandle, VSub(playerPos, m_pos));

}

void Camera::Draw()
{
	if (m_isLockOn)
	{
		m_pLockOnUi->Draw();
	}

#if _DEBUG
	// カメラの座標を描画
	DrawFormatString(0, 550, GetColor(255, 255, 255), "CameraAngleV : %f", m_angleV);
	DrawFormatString(0, 566, GetColor(255, 255, 255), "CameraAngleH : %f", m_angleH);
#endif
}

const VECTOR Camera::GetDirection() const
{
	return VNorm(VSub(m_targetPos, m_pos));
}

void Camera::ResetToPlayerView(float playerAngle)
{
	// プレイヤーの角度をカメラの水平角度に合わせる
	m_angleH = playerAngle + kAddAngle;
	m_angleV = 0.0f;

}

void Camera::LockOn(VECTOR playerPos, VECTOR enemyPos)
{
	// カメラの位置と角度を更新
	UpdateCameraAngle();
}

void Camera::UpdateLockOnCamera()
{
	VECTOR direction = VSub(m_enemyPos, m_playerPos);
	m_angleV = atan2f(direction.x, direction.z);
	m_angleH = atan2f(direction.y, VSize(VGet(direction.x, 0.0f, direction.z)));
}

void Camera::UpdateCameraAngle()
{
	// プレイヤーと敵の中間点を計算
	VECTOR midPoint = VGet(
		(m_playerPos.x + m_enemyPos.x) / kSmashNum,
		(m_playerPos.y + m_enemyPos.y) / kSmashNum,
		(m_playerPos.z + m_enemyPos.z) / kSmashNum);

	// カメラの位置を更新
	m_pos = VGet(
		midPoint.x - m_cameraDistance * cosf(m_angleH) * sinf(m_angleV),
		midPoint.y + m_cameraDistance * sinf(m_angleH),
		midPoint.z - m_cameraDistance * cosf(m_angleH) * cosf(m_angleV));

	// カメラの注視点を更新
	m_targetPos = midPoint;
}


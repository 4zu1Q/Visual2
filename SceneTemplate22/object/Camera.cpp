#include "Camera.h"
#include "object/player/Player.h"

#include "util/Pad.h"

namespace
{

	//カメラの距離
	constexpr float kCameraDist = 30;
	//カメラの高さ
	constexpr float kCameraHeight = 10;

	//constexpr float kAngleSpeed = 0.05f;	//旋回速度
	//constexpr float kCameraPlayerTargetHeight = 400.0f;	//プレイヤー座標からどれだけ高い位置を注視点とするか
	//constexpr float kToPlayerLength = 1600.0f;	//プレイヤーとの距離
	//constexpr float kCollisionSize = 50.0f;		//カメラの当たり判定サイズ

	//カメラのnearとfarの設定
	constexpr float kNear = 10.0f;
	constexpr float kFar = 6000.0f;

	//カメラの角度制限
	constexpr float kCameraAngleVLimitMin = -DX_PI_F / 2.0f + 0.6f;
	constexpr float kCameraAngleVLimitMax = DX_PI_F / 2.0f - 0.6f;

	//カメラのついてくる速度
	constexpr float kCameraFollowSpeed = 0.2f;
	constexpr float kPrevCameraFollowSpeed = 1.0f - kCameraFollowSpeed;
	constexpr float kCameraLockOnFollowSpeed = 0.5f;
	constexpr float kPrevCameraLockOnFollowSpeed = 1.0f - kCameraLockOnFollowSpeed;

	//カメラの注視点を追いかける速度
	constexpr float kCameraTargetFollowSpeed = 0.2f;
	constexpr float kPrevCameraTargetFollowSpeed = 1.0f - kCameraTargetFollowSpeed;

	// カメラの旋回スピード
	constexpr float kCameraAngleSpeedX = 0.1f;
	constexpr float kCameraAngleSpeedY = 0.05f;

	// カメラの最小角度と最大角度を求める
	constexpr float kCameraAngleVMax = DX_PI_F / 2.0f - 0.8f;
	constexpr float kCameraAngleVMin = -DX_PI_F / 2.0f + 0.6f;	

	// カメラの注視点(プレイヤー座標からの高さ)
	constexpr float kCameraPlayerTargetHeight = 65.0f;		


	///*アナログスティックによる移動関連*/
	//constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	//constexpr float kAnalogRangeMax = 0.8;
	//constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

}

Camera::Camera() :
	m_pos(VGet(0,0,0)),
	m_prevPos(VGet(0, 0, 0)),
	m_nextPos(VGet(0, 0, 0)),
	m_targetPos(VGet(0, 0, 0)),
	m_cameraangle(VGet(0, 0, 0)),
	m_angleV(0.0f),
	m_angleH(0.0f),
	m_rotX(MGetRotX(m_angleV)),
	m_rotY(MGetRotX(m_angleH)),
	m_angle(-DX_PI_F / 2)
{
	SetCameraNearFar(kNear, kFar);
}

Camera::~Camera()
{

}

void Camera::Initialize()
{

}

void Camera::Finalize()
{

}

//void Camera::Update(Player& player)
//{

	////アナログスティックを使ってカメラ回転
	//int analogX = 0;
	//int analogZ = 0;

	////アナログスティックを取得
	//GetJoypadAnalogInputRight(&analogX, &analogZ, DX_INPUT_PAD1);

	//// カメラに位置を反映
	////注視点の座標
	//VECTOR playerAimPos = VGet(player.GetPosDown().x , player.GetPosDown().y, player.GetPosDown().z);
	////ベクトルの方向(注視点-カメラのポジション)
	//VECTOR posToAim = VSub(playerAimPos, m_prevPos);

	////右スティックを右に押した場合
	//if (analogX >= 10)
	//{
	//	m_angle -= 0.05f;
	//}
	//else if (analogX <= -10)
	//{	
	//	m_angle += 0.05f;
	//}

	////カメラの回転
	//m_prevPos.x += cosf(m_angle) * kCameraDist;
	//m_prevPos.y += kCameraHeight;
	//m_prevPos.z += sinf(m_angle) * kCameraDist;

	////現在位置に設定したポジションを足す
	//m_prevPos = VAdd(m_prevPos, posToAim);


	//SetCameraPositionAndTarget_UpVecY(m_prevPos, player.GetPosDown());
//}

void Camera::Draw()
{

#ifdef _DEBUG


	DrawFormatString(0, 80, 0x000000, "カメラ座標：%f,%f,%f", m_prevPos.x, m_prevPos.y, m_prevPos.z);
	DrawFormatString(0, 100, 0x000000, "ターゲット座標：%f,%f,%f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
	DrawFormatString(0, 120, 0x000000, "GetTargetPos:%f,%f,%f", GetCameraTarget().x, GetCameraTarget().y, GetCameraTarget().z);

	//DrawGrid();

#endif
}

void Camera::Update(VECTOR playerPos/*, bool isLockOn*/)
{
	// 更新前の座標の設定
	m_prevPos = m_nextPos;

	// ターゲットの座標の設定
	VECTOR targetPos;
	targetPos = playerPos;
	targetPos.y += kCameraPlayerTargetHeight;

	// 角度更新
	AngleUpdate();

	// 座標更新
	NormalUpdate(targetPos);

	// 座標の確定
	CameraPosUpdate();


	//m_pos = VAdd(VScale(m_prevPos, kPrevCameraFollowSpeed), VScale(m_nextPos, kCameraFollowSpeed));
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::ResetCamera()
{
	SetCameraNearFar(kNear, kFar);
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::DebugUpdate()
{

	//アナログスティックを使ってカメラ回転
	int analogX = 0;
	int analogZ = 0;

	//アナログスティックを取得
	GetJoypadAnalogInputRight(&analogX, &analogZ, DX_INPUT_PAD1);

	//右スティックを右に押した場合
	if (analogX >= 10)
	{
		m_angle -= 0.05f;
	}
	else if (analogX <= -10)
	{	
		m_angle += 0.05f;
	}

	//カメラの回転
	m_prevPos.x += cosf(m_angle) * kCameraDist;
	m_prevPos.y += kCameraHeight;
	m_prevPos.z += sinf(m_angle) * kCameraDist;
}

void Camera::CameraPosUpdate()
{

	//垂直方向の回転はX軸回転
	MATRIX RotX = MGetRotY(m_angleV);
	//水平方向の回転はY軸回転
	MATRIX RotY = MGetRotY(m_angleH);

	/// <summary>
	/// カメラの座標を計算
	/// (X軸にカメラとプレイヤーとの距離分だけ伸びたベクトルを
	/// 垂直方向回転させた後に水平方向回転してそれに注視点の座標を足す)
	/// </summary>
	m_nextPos = VAdd(VTransform(VTransform(VGet(0.0f, 0.0f, m_cameraToTargetLength), RotX), RotY), m_targetPos);

}

void Camera::NormalUpdate(VECTOR targetPos)
{
	m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (targetPos.x * kCameraTargetFollowSpeed);
	m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (targetPos.y * kCameraTargetFollowSpeed);
	m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (targetPos.z * kCameraTargetFollowSpeed);
}

void Camera::AngleUpdate(/*bool isLookOn*/)
{
	//if (isLookOn)
	//{
	//	return;
	//}

	//パッドのアナログ情報の取得
	DINPUT_JOYSTATE input;

	//入力情報の初期化
	input.Rx = 0;
	input.Ry = 0;

	// 入力情報の取得
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input);

	// 回転を1～1000から0.001～1の範囲にする
	// 後でカメラ感度も用意する(一旦3にしておく)
	float rotX = 0.00020f * input.Rx * 3;
	float rotY = 0.00017f * input.Ry * 3;

	// カメラの回転スピードをかける
	rotX *= kCameraAngleSpeedX;
	rotY *= kCameraAngleSpeedY;

	m_angleH += rotX;
	if (input.Rx < 0.0f)
	{
		// -180°以下になったら角度地が大きくなりすぎないように360°を足す
		if (m_angleH < -DX_PI_F)
		{
			m_angleH += DX_TWO_PI_F;
		}
	}
	if (input.Rx > 0.0f)
	{
		// 180°以上になったら角度地が大きくなりすぎないように360°を引く
		if (m_angleH > DX_PI_F)
		{
			m_angleH -= DX_TWO_PI_F;
		}
	}

	m_angleV -= rotY;
	// 一定角度以下ににはならないようにする
	if (m_angleV < kCameraAngleVMin)
	{
		m_angleV = kCameraAngleVMin;
	}
	// 一定角度以上にはならないようにする
	if (m_angleV > kCameraAngleVMax)
	{
		m_angleV = kCameraAngleVMax;
	}


}

void Camera::LockOnUpdate(VECTOR targetPos)
{
}

//void Camera::DrawGrid()
//{
//	for (int x = -350; x <= 350; x += 5)
//	{
//		DrawLine3D(VGet(static_cast<float>(x), 0, -350), VGet(static_cast<float>(x), 0, 350), 0xffff00);
//	}
//	for (int z = -350; z <= 350; z += 5)
//	{
//		DrawLine3D(VGet(-350, 0, static_cast<float>(z)), VGet(350, 0, static_cast<float>(z)), 0xff0000);
//	}
//
//	// X+-,Z+-の方向が分かりやすいように表示を追加する
//	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
//	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
//	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
//	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
//	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
//	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
//	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
//	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
//}

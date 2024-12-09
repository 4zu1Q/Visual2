#include "Camera.h"
#include "object/player/Player.h"

#include <cmath>
#include "util/Pad.h"

namespace
{

	////カメラの距離
	//constexpr float kCameraDist = 30;
	////カメラの高さ
	//constexpr float kCameraHeight = 10;

	////constexpr float kAngleSpeed = 0.05f;	//旋回速度
	////constexpr float kCameraPlayerTargetHeight = 400.0f;	//プレイヤー座標からどれだけ高い位置を注視点とするか
	////constexpr float kToPlayerLength = 1600.0f;	//プレイヤーとの距離
	////constexpr float kCollisionSize = 50.0f;		//カメラの当たり判定サイズ

	////カメラのnearとfarの設定
	constexpr float kNear = 0.0f;
	constexpr float kFar = 6000.0f;

	////カメラの角度制限
	//constexpr float kCameraAngleVLimitMin = -DX_PI_F / 2.0f + 0.6f;
	//constexpr float kCameraAngleVLimitMax = DX_PI_F / 2.0f - 0.6f;

	////カメラのついてくる速度
	//constexpr float kCameraFollowSpeed = 0.2f;
	//constexpr float kPrevCameraFollowSpeed = 1.0f - kCameraFollowSpeed;
	//constexpr float kCameraLockOnFollowSpeed = 0.5f;
	//constexpr float kPrevCameraLockOnFollowSpeed = 1.0f - kCameraLockOnFollowSpeed;

	////カメラの注視点を追いかける速度
	//constexpr float kCameraTargetFollowSpeed = 0.2f;
	//constexpr float kPrevCameraTargetFollowSpeed = 1.0f - kCameraTargetFollowSpeed;

	//// カメラの旋回スピード
	//constexpr float kCameraAngleSpeedX = 0.1f;
	//constexpr float kCameraAngleSpeedY = 0.05f;

	//// カメラの最小角度と最大角度を求める
	//constexpr float kCameraAngleVMax = DX_PI_F / 2.0f - 0.8f;
	//constexpr float kCameraAngleVMin = -DX_PI_F / 2.0f + 0.6f;	

	//// カメラの注視点(プレイヤー座標からの高さ)
	//constexpr float kCameraPlayerTargetHeight = 65.0f;		

	//// カメラからプレイヤーまでの最大距離と最小距離
	//constexpr float kCameraToPlayerLenghtMax = 175.0f;		
	//constexpr float kCameraToPlayerLenghtMin = 0.0f;		

	//////////////////////////////////////////////////////////////

	constexpr float kCameraDist = 24.0f;
	constexpr float kCameraHeight = 2.0f;

	constexpr float kDashCameraDist = 90.0f;


	constexpr float kCameraNear = 3.0f;
	constexpr float kCameraFar = 6000.0f;

	constexpr float kCameraRadius = kCameraNear + 0.2f;

	constexpr float kAngleMoveScaleMax = 0.01f;

	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大

	// カメラの旋回スピード
	constexpr float kCameraAngleSpeedX = 0.1f;
	constexpr float kCameraAngleSpeedY = 0.05f;

	//カメラの角度制限
	constexpr float kCameraAngleVLimitMin = -DX_PI_F / 2.0f + 0.6f;
	constexpr float kCameraAngleVLimitMax = DX_PI_F / 2.0f - 0.6f;

	// カメラからプレイヤーまでの最大距離
	constexpr float kCameraToPlayerLenghtMax = 175.0f;



	///*アナログスティックによる移動関連*/
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	//constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

}

Camera::Camera() :
	m_cameraAngleX(0.0f),
	m_cameraAngleY(0.0f),
	m_cameraPos{ 0.0f,0.0f,0.0f },
	m_aimPos{ 0.0f,0.0f,0.0f },
	m_playerPos{ 0.0f,0.0f,0.0f },
	m_testPositon{ 0.0f,0.0f,0.0f },
	m_lightHandle(-1),
	m_angleMoveScale(0.0f),
	//////////////////////////////////////
	m_nextPos(VGet(0, 0, 0)),
	m_pos(VGet(0, 0, 0)),
	m_prevPos(VGet(0, 0, 0)),
	m_targetPos(VGet(0, 0, 0)),
	m_cameraangle(VGet(0, 0, 0)),
	m_cameraToTargetLength(kCameraToPlayerLenghtMax),
	m_angleV(0.0f),
	m_angleH(0.0f),
	m_rotX(MGetRotX(m_angleV)),
	m_rotY(MGetRotX(m_angleH)),
	m_angle(-DX_PI_F / 2)
{
	m_cameraDist = kCameraDist;

	m_isDash = false;
	m_isTarget = false;
	m_isMovie = false;
	m_isClear = false;

	//SetCameraNearFar(kNear, kFar);
	m_lightHandle = CreateDirLightHandle(VSub(m_aimPos, m_cameraPos));
}

Camera::~Camera()
{
	DeleteLightHandle(m_lightHandle);

}

void Camera::Initialize()
{
	m_cameraAngleX = 0.0f;
	m_cameraAngleY = 12.0f;
	SetCameraNearFar(kCameraNear, kCameraFar);

	m_updateFunc = &Camera::NormalUpdate;
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

void Camera::Update(int stageHandle ,VECTOR targetPos)
{
	//アップデート
	m_targetPos = targetPos;

	(this->*m_updateFunc)(m_targetPos);




	// 最初はステージ自体と判定
	//m_hitDim = MV1CollCheck_Capsule(stageHandle, -1, m_aimPos, m_cameraPos, kCameraRadius);

	//// 検出した周囲のポリゴン情報を開放する
	//MV1CollResultPolyDimTerminate(m_hitDim);

	//// ステージのポリゴンは周囲に無かったら今度はコリジョンオブジェクトのポリゴンが周囲にあるか調べる
	//if (m_hitDim.HitNum == 0)
	//{
	//	// 検出した周囲のポリゴン情報を開放する
	//	MV1CollResultPolyDimTerminate(m_hitDim);
	//}
	//else if (m_hitDim.HitNum != 0)
	//{
	//	bool doCheck = true;

	//	while (doCheck)
	//	{

	//		doCheck = false;

	//		//プレイヤーの座標からカメラの移動予定後座標の方向ベクトルを計算する
	//		auto playerToCamera = VSub(m_cameraPos , m_aimPos);

	//		//向きと大きさに分ける
	//		auto vec = VNorm(playerToCamera);
	//		auto length = VSize(playerToCamera);

	//		//距離を縮める
	//		length *= 0.998f;

	//		auto checkPos = VScale(VAdd(m_aimPos, vec), length);

	//		// 最初はステージ自体と判定
	//		m_hitDim = MV1CollCheck_Capsule(stageHandle, -1, m_aimPos, checkPos, kCameraRadius);
	//		MV1CollResultPolyDimTerminate(m_hitDim);

	//		if (m_hitDim.HitNum != 0)
	//		{
	//			m_cameraPos = checkPos;
	//			doCheck = true;
	//		}
	//		else
	//		{
	//			doCheck = false;
	//		}

	//		// HitLength と NoHitLength が十分に近づいていなかったらループ
	//	}

	//	nextPos = VGet(m_testPositon.x, m_testPositon.y, m_testPositon.z);
	//}

	SetLightDirectionHandle(m_lightHandle, VSub(m_aimPos, m_cameraPos));

	SetCameraPositionAndTarget_UpVecY(m_cameraPos, m_aimPos);

	//// 更新前の座標の設定
	//m_prevPos = m_nextPos;

	//// ターゲットの座標の設定
	//VECTOR targetPos;
	//targetPos = playerPos;
	//targetPos.y += kCameraPlayerTargetHeight;

	//// 角度更新
	//AngleUpdate();

	//// 座標更新
	//NormalUpdate(targetPos);

	//// 座標の確定
	//CameraPosUpdate();

	////m_pos = VAdd(VScale(m_prevPos, kPrevCameraFollowSpeed), VScale(m_nextPos, kCameraFollowSpeed));
	//SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::Draw()
{

#ifdef _DEBUG


	//DrawFormatString(0, 80, 0x000000, "カメラ座標：%f,%f,%f", m_prevPos.x, m_prevPos.y, m_prevPos.z);
	//DrawFormatString(0, 100, 0x000000, "ターゲット座標：%f,%f,%f", m_targetPos.x, m_targetPos.y, m_targetPos.z);
	//DrawFormatString(0, 120, 0x000000, "GetTargetPos:%f,%f,%f", GetCameraTarget().x, GetCameraTarget().y, GetCameraTarget().z);

	//DrawGrid();

#endif
}

void Camera::ResetCamera()
{
	SetCameraNearFar(kNear, kFar);
	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
}

void Camera::DebugUpdate(VECTOR playerPos)
{
	//アナログスティックを使ってカメラ回転
	int analogX = 0;
	int analogZ = 0;

	//アナログスティックを取得
	GetJoypadAnalogInputRight(&analogX, &analogZ, DX_INPUT_PAD1);

	// カメラに位置を反映
	//注視点の座標
	VECTOR playerAimPos = VGet(playerPos.x, playerPos.y, playerPos.z);
	//ベクトルの方向(注視点-カメラのポジション)
	VECTOR posToAim = VSub(playerAimPos, m_prevPos);

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
	m_prevPos.x += cosf(m_angle) * m_cameraDist;
	m_prevPos.y += kCameraHeight;
	m_prevPos.z += sinf(m_angle) * m_cameraDist;

	//現在位置に設定したポジションを足す
	m_prevPos = VAdd(m_prevPos, posToAim);


	SetLightDirectionHandle(m_lightHandle, VSub(m_aimPos, m_cameraPos));
	SetCameraPositionAndTarget_UpVecY(m_prevPos, playerPos);

	////アナログスティックを使ってカメラ回転
	//int analogX = 0;
	//int analogZ = 0;

	////アナログスティックを取得
	//GetJoypadAnalogInputRight(&analogX, &analogZ, DX_INPUT_PAD1);

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
}

const VECTOR Camera::GetDirection()
{
	return VNorm(VSub(m_aimPos,m_cameraPos));
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
	if (m_isDash)
	{
		OnDash();
	}
	else if (m_isTarget)
	{
		OnTarget();
	}
	else if (m_isMovie)
	{
		OnMovie();
	}
	else if (m_isClear)
	{
		OnClear();
	}


	m_angleMoveScale = kAngleMoveScaleMax * 1.0f;

	//アナログスティックを使ってカメラ回転
	int analogX = 0;
	int analogY = 0;

	//アナログスティックを取得
	GetJoypadAnalogInputRight(&analogX, &analogY, DX_INPUT_PAD1);

	//ベクトルの方向(注視点-カメラのポジション)
	VECTOR posToAim = VSub(m_playerPos, m_prevPos);

	//入力から角度を計算する
	if (analogX > 100.0f)
	{
		m_cameraAngleX -= m_angleMoveScale * std::abs(analogX);
	}
	else if (analogX < -100.0f)
	{
		m_cameraAngleX += m_angleMoveScale * std::abs(analogX);
	}

	if (analogY > 100.0f)
	{
		m_cameraAngleY += m_angleMoveScale * std::abs(analogY);
		//角度が90度超えてしまった場合
		if (m_cameraAngleY > 90)
		{
			m_cameraAngleY = 88;
		}
	}
	else if (analogY < -100.0f)
	{
		m_cameraAngleY -= m_angleMoveScale * std::abs(analogY);
		//角度が-90度超えてしまった場合
		if (m_cameraAngleY < -90)
		{
			m_cameraAngleY = -88;
		}
	}

	// カメラの位置はカメラの水平角度と垂直角度から算出
	// 最初に垂直角度を反映した位置を算出
	VECTOR tempPos1;
	float sinParam = sinf(m_cameraAngleY / 180.0f * DX_PI_F);
	float cosParam = cosf(m_cameraAngleY / 180.0f * DX_PI_F);
	tempPos1.x = 0.0f;
	tempPos1.y = sinParam * m_cameraDist;
	tempPos1.z = -cosParam * m_cameraDist;

	// 次に水平角度を反映した位置を算出
	VECTOR tempPos2;
	sinParam = sinf(m_cameraAngleX / 180.0f * DX_PI_F);
	cosParam = cosf(m_cameraAngleX / 180.0f * DX_PI_F);
	tempPos2.x = cosParam * tempPos1.x - sinParam * tempPos1.z;
	tempPos2.y = tempPos1.y;
	tempPos2.z = sinParam * tempPos1.x + cosParam * tempPos1.z;

	//
	m_aimPos = VGet(m_playerPos.x, m_playerPos.y + 4.0f, m_playerPos.z - 10.0f);

	// 算出した座標に注視点の位置を加算したものがカメラの位置になる
	m_cameraPos = VAdd(tempPos2, m_aimPos);

	auto nextPos = m_cameraPos;

	//m_targetPos.x = (m_targetPos.x * kPrevCameraTargetFollowSpeed) + (targetPos.x * kCameraTargetFollowSpeed);
	//m_targetPos.y = (m_targetPos.y * kPrevCameraTargetFollowSpeed) + (targetPos.y * kCameraTargetFollowSpeed);
	//m_targetPos.z = (m_targetPos.z * kPrevCameraTargetFollowSpeed) + (targetPos.z * kCameraTargetFollowSpeed);
}

void Camera::DashUpdate(VECTOR targetPos)
{
	if (!m_isDash)
	{
		OnNormal();
	}
	else if (!m_isTarget)
	{
		OnTarget();
	}

	m_angleMoveScale = kAngleMoveScaleMax * 1.0f;

	//アナログスティックを使ってカメラ回転
	int analogX = 0;
	int analogY = 0;

	//アナログスティックを取得
	GetJoypadAnalogInputRight(&analogX, &analogY, DX_INPUT_PAD1);

	//ベクトルの方向(注視点-カメラのポジション)
	VECTOR posToAim = VSub(m_playerPos, m_prevPos);

	//入力から角度を計算する
	if (analogX > 50.0f)
	{
		m_cameraAngleX -= m_angleMoveScale * std::abs(analogX);
	}
	else if (analogX < -50.0f)
	{
		m_cameraAngleX += m_angleMoveScale * std::abs(analogX);
	}

	if (analogY > 50.0f)
	{
		m_cameraAngleY += m_angleMoveScale * std::abs(analogY);
		//角度が90度超えてしまった場合
		if (m_cameraAngleY > 90)
		{
			m_cameraAngleY = 88;
		}
	}
	else if (analogY < -50.0f)
	{
		m_cameraAngleY -= m_angleMoveScale * std::abs(analogY);
		//角度が-90度超えてしまった場合
		if (m_cameraAngleY < -90)
		{
			m_cameraAngleY = -88;
		}
	}

	// カメラの位置はカメラの水平角度と垂直角度から算出
	// 最初に垂直角度を反映した位置を算出
	VECTOR tempPos1;
	float sinParam = sinf(m_cameraAngleY / 180.0f * DX_PI_F);
	float cosParam = cosf(m_cameraAngleY / 180.0f * DX_PI_F);
	tempPos1.x = 0.0f;
	tempPos1.y = sinParam * kDashCameraDist;
	tempPos1.z = -cosParam * kDashCameraDist;

	// 次に水平角度を反映した位置を算出
	VECTOR tempPos2;
	sinParam = sinf(m_cameraAngleX / 180.0f * DX_PI_F);
	cosParam = cosf(m_cameraAngleX / 180.0f * DX_PI_F);
	tempPos2.x = cosParam * tempPos1.x - sinParam * tempPos1.z;
	tempPos2.y = tempPos1.y;
	tempPos2.z = sinParam * tempPos1.x + cosParam * tempPos1.z;

	//
	m_aimPos = VGet(m_playerPos.x, m_playerPos.y + 4.0f, m_playerPos.z - 10.0f);

	// 算出した座標に注視点の位置を加算したものがカメラの位置になる
	m_cameraPos = VAdd(tempPos2, m_aimPos);

	auto nextPos = m_cameraPos;
}

void Camera::TargetUpdate(VECTOR targetPos)
{
}

void Camera::MovieUpdate(VECTOR targetPos)
{
}

void Camera::ClearUpdate(VECTOR targetPos)
{
}

void Camera::OnNormal()
{
	m_cameraDist = kCameraDist;
	m_updateFunc = &Camera::NormalUpdate;
}

void Camera::OnDash()
{
	m_cameraDist = kDashCameraDist;
	m_updateFunc = &Camera::DashUpdate;
}

void Camera::OnTarget()
{
	m_updateFunc = &Camera::TargetUpdate;
}

void Camera::OnMovie()
{
	m_updateFunc = &Camera::MovieUpdate;
}

void Camera::OnClear()
{
	m_updateFunc = &Camera::ClearUpdate;
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
	//if (m_angleV < kCameraAngleVMin)
	//{
	//	m_angleV = kCameraAngleVMin;
	//}
	//// 一定角度以上にはならないようにする
	//if (m_angleV > kCameraAngleVMax)
	//{
	//	m_angleV = kCameraAngleVMax;
	//}


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

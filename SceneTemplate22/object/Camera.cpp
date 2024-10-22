#include "Camera.h"
#include "object/player/PlayerBase.h"

#include "util/Pad.h"

namespace
{
	constexpr float kCameraDist = 66;
	constexpr float kCameraHeight = 5;

	constexpr float kAngleSpeed = 0.05f;	//旋回速度
	constexpr float kCameraPlayerTargetHeight = 400.0f;	//プレイヤー座標からどれだけ高い位置を注視点とするか
	constexpr float kToPlayerLength = 1600.0f;	//プレイヤーとの距離
	constexpr float kCollisionSize = 50.0f;		//カメラの当たり判定サイズ

	//アナログスティックによる移動関連
	constexpr float kMaxSpeed = 0.5f;		//プレイヤーの最大移動速度
	constexpr float kAnalogRangeMin = 0.1;	//アナログスティックの入力判定範囲
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//アナログスティックから入力されるベクトルの最大値

}

Camera::Camera() :
	m_pos(VGet(0, 10, 0)),
	m_targetPos(VGet(0, 0, 0)),
	m_cameraangle(VGet(0, 0, 0)),
	m_angle(-DX_PI_F / 2)
{
	SetCameraNearFar(0.1f, 1180.0f);
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

void Camera::Update(PlayerBase& player)
{
	//アナログスティックを使ってカメラ回転
	int analogX = 0;
	int analogZ = 0;

	//アナログスティックを取得
	GetJoypadAnalogInputRight(&analogX, &analogZ, DX_INPUT_PAD1);

	// カメラに位置を反映
	//注視点の座標
	VECTOR playerAimPos = VGet(player.GetPos().x, player.GetPos().y + 20.0f, player.GetPos().z);
	//ベクトルの方向(注視点-カメラのポジション)
	VECTOR posToAim = VSub(playerAimPos, m_pos);

	//右スティックを右に押した場合
	if (analogX >= 10)
	{
		//回転する
		m_angle -= 0.05f;
	}
	else if (analogX <= -10)
	{
		//回転する
		m_angle += 0.05f;
	}

	m_pos.x += cosf(m_angle) * kCameraDist;
	m_pos.y += kCameraHeight;
	m_pos.z += sinf(m_angle) * kCameraDist;

	//現在位置に設定したポジションを足す
	m_pos = VAdd(m_pos, posToAim);


	SetCameraPositionAndTarget_UpVecY(m_pos, player.GetPos());
}

void Camera::Draw()
{

#ifdef _DEBUG

	DrawGrid();

#endif
}

void Camera::DrawGrid()
{
	for (int x = -350; x <= 350; x += 5)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -350), VGet(static_cast<float>(x), 0, 350), 0xffff00);
	}
	for (int z = -350; z <= 350; z += 5)
	{
		DrawLine3D(VGet(-350, 0, static_cast<float>(z)), VGet(350, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
}

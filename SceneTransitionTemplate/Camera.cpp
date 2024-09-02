#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Pad.h"

#include "DxLib.h"
#include <cmath>

#define D2R(deg) ((deg)*DX_PI_F/180.0f)

/// <summary>
/// 定数
/// </summary>
namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 20;

	constexpr float kAngleSpeed = 0.05f;	//旋回速度
	constexpr float kCameraPlayerTargetHeight = 400.0f;	//プレイヤー座標からどれだけ高い位置を注視点とするか
	constexpr float kToPlayerLength = 1600.0f;	//プレイヤーとの距離
	constexpr float kCollisionSize = 50.0f;		//カメラの当たり判定サイズ


}

/// <summary>
/// コンストラクタ
/// </summary>
Camera::Camera() :
	m_pos(VGet(0, 10, -25)),
	m_targetPos(VGet(0, 0, 0)),
	m_cameraangle(VGet(0, 0, 0)),
	m_angle(-DX_PI_F / 2)
{
	SetCameraNearFar(20, 600);
}

/// <summary>
/// デストラクタ
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// 初期化
/// </summary>
void Camera::Init()
{
	m_cameraangle = VGet(D2R(-20.0f), 0.0f, 0.0f);

}

/// <summary>
/// プレイシーンのアップデート
/// </summary>
void Camera::PlayCameraUpdate(Player& player)
{
	Pad::Update();

#ifdef _DEBUG

	DrawGrid();

#endif

	// カメラに位置を反映
	//注視点の座標
	VECTOR playerAimPos = VGet(player.GetPos().x, player.GetPos().y + 20.0f, player.GetPos().z - 50.0f);
	//ベクトルの方向(注視点-カメラのポジション)
	VECTOR posToAim = VSub(playerAimPos, m_pos);

	//現在位置に設定したポジションを足す
	m_pos = VAdd(m_pos, posToAim);

	//VECTOR pos = VSub(m_pos,playerAimPos);
	//pos = VNorm(pos);
	//float size = VSize(pos);


	if (Pad::IsPress PAD_INPUT_6)
	{
		m_cameraangle.y += D2R(1.0f);
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_cameraangle.y -= D2R(1.0f);
		//m_angle -= 0.05f;
	}

	//注視点の座標をプレイヤーの座標に代入
	m_targetPos = player.GetPos();

	//基準ベクトル
	VECTOR Direction = VGet(0.0f, 0.0f, 10.0f);

	// Ｘ軸回転行列
	MATRIX MatrixX = MGetRotX(m_cameraangle.x);
	// Ｙ軸回転行列
	MATRIX MatrixY = MGetRotY(m_cameraangle.y);
	// 行列の合成
	MATRIX Matrix = MMult(MatrixX, MatrixY);
	// 基準ベクトルを行列で変換
	Direction = VTransform(Direction, Matrix);


	//m_pos.y = cosf(m_angle) * player.GetPos().y;
	//m_pos.y = sinf(m_angle) * player.GetPos().y;









	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	//SetCameraPositionAndTarget_UpVecY(m_pos, player.GetPos());

	//VECTOR pPos;
//pPos.x = cosf(m_angle) * size;
//pPos.y = m_pos.y;
//pPos.z = sinf(m_angle) * size;
//player.SetPos(pPos);
}

/// <summary>
/// タイトルシーンのアップデート
/// </summary>
void Camera::TitleCameraUpdate()
{
#ifdef _DEBUG

	DrawGrid();

#endif
	Pad::Update();


	if (Pad::IsPress PAD_INPUT_6)
	{
		m_angle += 0.05f;
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_angle -= 0.05f;
	}

	m_pos.x = cosf(m_angle) * kCameraDist;
	m_pos.y = kCameraHeight;
	m_pos.z = sinf(m_angle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(m_pos, VGet(0, 0, 0));
}

void Camera::TargetCameraUpadate()
{

}

void Camera::DrawGrid()
{
	for (int x = -200; x <= 200; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -200), VGet(static_cast<float>(x), 0, 200), 0xffff00);
	}
	for (int z = -200; z <= 200; z += 10)
	{
		DrawLine3D(VGet(-200, 0, static_cast<float>(z)), VGet(200, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(200, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-200, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 200));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -200));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);

	DrawFormatString(0, 64, 0xffffff, "Camera(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);


}

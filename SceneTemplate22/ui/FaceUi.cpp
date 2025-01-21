#include "FaceUi.h"

#include "object/player/Player.h"

namespace
{
	//顔の画像ファイル名
	const char* const kPowerFaceFileName = "Data/Image/Face01.png";
	const char* const kSpeedFaceFileName = "Data/Image/Face02.png";
	const char* const kShotFaceFileName = "Data/Image/Face03.png";
	const char* const kRastFaceFileName = "Data/Image/Face04.png";

	//顔の座標
	const Vec2 kFacePos = { 1128.0f , 40.0f };

}

FaceUi::FaceUi():
	m_PowerFaceHandle(LoadGraph(kPowerFaceFileName)),
	m_SpeedFaceHandle(LoadGraph(kSpeedFaceFileName)),
	m_ShotFaceHandle(LoadGraph(kShotFaceFileName)),
	m_RastFaceHandle(LoadGraph(kRastFaceFileName))
{

}

FaceUi::~FaceUi()
{

}

void FaceUi::Update()
{
}

void FaceUi::Draw(Player& player)
{
	//プレイヤーの仮面UI
	if (player.GetFaceKind() == e_PlayerKind::kPowerPlayer)
	{
		DrawGraph(kFacePos.x, kFacePos.y, m_PowerFaceHandle, true);

	}
	else if (player.GetFaceKind() == e_PlayerKind::kSpeedPlayer)
	{
		DrawGraph(kFacePos.x, kFacePos.y, m_SpeedFaceHandle, true);

	}
	else if (player.GetFaceKind() == e_PlayerKind::kShotPlayer)
	{
		DrawGraph(kFacePos.x, kFacePos.y, m_ShotFaceHandle, true);

	}
	else if (player.GetFaceKind() == e_PlayerKind::kStrongestPlayer)
	{
		DrawGraph(kFacePos.x, kFacePos.y, m_RastFaceHandle, true);

	}

}

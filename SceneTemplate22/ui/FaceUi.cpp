#include "FaceUi.h"

#include "object/player/Player.h"

namespace
{
	//顔のUIの位置
	constexpr int kFacePosX = 32;
	constexpr int kFacePosY = 150;

	//デバッグ用の円の半径
	constexpr int kRadius = 16;
}

FaceUi::FaceUi()
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
	if (player.GetFaceKind() == Player::e_PlayerKind::kPowerPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xffff0f, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kSpeedPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xfff0ff, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kShotPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0xff0fff, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kStrongestPlayer)
	{
		DrawCircle(kFacePosX, kFacePosY, kRadius, 0x0fffff, true, true);

	}

}

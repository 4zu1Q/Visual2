#include "FaceUi.h"

#include "object/player/Player.h"

namespace
{
	//顔のUIの位置
	constexpr int kFacePosX = 32;
	constexpr int kFacePosY = 150;

	//デバッグ用の円の半径
	constexpr int kRadius = 32;
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


	//顔のUIのフレーム
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	//DrawBox(1160, 20, 1260, 120, 0x000000, true);
	//DrawCircle(1110, 35, 32, 0xff0000, true, true);
	//DrawCircle(1210, 70, kRadius, 0x000000, true, true);
	//DrawCircle(1210, 70, kRadius, 0x000000, true, true);
	//DrawCircle(1210, 70, kRadius, 0x000000, true, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//DrawBox(1160, 20, 1260, 120, 0xffffff, false);

	//A
	//DrawCircle(1110, 35, 32, 0xffffff,false);
	////B
	//DrawCircle(1210, 70, kRadius, 0xffff0f, false);
	////X
	//DrawCircle(1210, 70, kRadius, 0xffff0f, false);
	////Y
	//DrawCircle(1210, 70, kRadius, 0xffff0f, false);

	//プレイヤーの仮面UI
	if (player.GetFaceKind() == Player::e_PlayerKind::kPowerPlayer)
	{
		DrawCircle(1160, 70, kRadius, 0xffff0f, true, true);
		//DrawCircle(kFacePosX, kFacePosY, kRadius, 0xffff0f, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kSpeedPlayer)
	{
		DrawCircle(1160, 70, kRadius, 0xfff0ff, true, true);
		//DrawCircle(kFacePosX, kFacePosY, kRadius, 0xfff0ff, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kShotPlayer)
	{
		DrawCircle(1160, 70, kRadius, 0xff0fff, true, true);
		//DrawCircle(kFacePosX, kFacePosY, kRadius, 0xff0fff, true, true);

	}
	else if (player.GetFaceKind() == Player::e_PlayerKind::kStrongestPlayer)
	{
		DrawCircle(1160, 70, kRadius, 0x0fffff, true, true);
		//DrawCircle(kFacePosX, kFacePosY, kRadius, 0x0fffff, true, true);

	}

}

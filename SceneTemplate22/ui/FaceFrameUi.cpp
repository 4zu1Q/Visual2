#include "FaceFrameUi.h"
#include "object/player/Player.h"
#include "FaceFrameUi.h"

namespace
{
	//顔選択の画像ファイル名
	const char* const kFaceFileName = "Data/Image/FaceFrame1.png";
	const char* const kFaceDecideFileName = "Data/Image/FaceFrameLock1.png";

	//顔選択の枠の座標
	const Vec2 kFaceFramePos = { 1100.0f , 6.0f };
}

FaceFrameUi::FaceFrameUi() :
	m_faceFrameHandle(LoadGraph(kFaceFileName)),
	m_faceFrameLockHandle(LoadGraph(kFaceDecideFileName))
{

}

FaceFrameUi::~FaceFrameUi()
{

}

void FaceFrameUi::Update()
{

}

void FaceFrameUi::Draw(Player& player)
{
	if (!player.GetIsFaceUse())
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_faceFrameHandle, true);
	}
	else
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_faceFrameLockHandle, true);
	}

}

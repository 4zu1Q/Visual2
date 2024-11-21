#include "FaceFrameUi.h"
#include "object/player/Player.h"

namespace
{
	//顔選択の画像ファイル名
	const char* const kFaceFileName = "Data/Image/FaceFrame.png";
	const char* const kFaceDecideFileName = "Data/Image/FaceFrameLock.png";

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
	if (!player.GetIsFrame())
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_faceFrameHandle, true);
	}
	else
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_faceFrameLockHandle, true);
	}

}

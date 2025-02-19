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

	//使う画像の種類
	enum e_Ui
	{
		kFrameH,
		kFrameLockH,
	};
}

FaceFrameUi::FaceFrameUi()
{
	//画像のロード
	m_handles.push_back(LoadGraph(kFaceFileName));
	m_handles.push_back(LoadGraph(kFaceDecideFileName));
}

FaceFrameUi::~FaceFrameUi()
{
	//画像の削除
	for (int i = 0; i < m_handles.size(); i++)
	{
		DeleteGraph(m_handles[i]);
	}

	m_handles.clear();
}

void FaceFrameUi::Update()
{
}

void FaceFrameUi::Draw(Player& player)
{
	if (!player.GetIsFaceUse())
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_handles[kFrameH], true);
	}
	else
	{
		DrawGraph(kFaceFramePos.x, kFaceFramePos.y, m_handles[kFrameLockH], true);
	}
}

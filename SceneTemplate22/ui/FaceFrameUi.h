#pragma once
#include "DxLib.h"
#include "util/Vec2.h"

class Player;
class FaceFrameUi
{
public:

	FaceFrameUi();
	virtual ~FaceFrameUi();

	void Update();
	void Draw(Player& player);

private:

	//顔の枠のハンドル
	int m_faceFrameHandle;
	int m_faceFrameLockHandle;


};


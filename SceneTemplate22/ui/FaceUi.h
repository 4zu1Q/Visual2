#pragma once
#include "DxLib.h"
#include "util/Vec2.h"

class Player;

class FaceUi
{
public:

	FaceUi();
	virtual ~FaceUi();

	void Update();
	void Draw(Player& player);

private:
	
	//顔のハンドル
	int m_PowerFaceHandle;
	int m_SpeedFaceHandle;
	int m_ShotFaceHandle;
	int m_RastFaceHandle;

};


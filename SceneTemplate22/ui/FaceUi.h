#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <memory>

class Player;

class FaceUi
{
public:

	FaceUi();
	virtual ~FaceUi();

	void Update();
	void Draw(Player& player);

private:

	int m_hpHandle;
	int m_hpLostHandle;
	

};


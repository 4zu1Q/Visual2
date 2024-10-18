#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <memory>

class PlayerBase;

class FaceUi
{
public:

	FaceUi();
	virtual ~FaceUi();

	void Update();
	void Draw(PlayerBase& player);

private:


	

};


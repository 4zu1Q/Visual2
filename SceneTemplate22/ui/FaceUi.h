#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <vector>

class Player;

class FaceUi
{
public:

	FaceUi();
	virtual ~FaceUi();

	void Update();
	void Draw(Player& player);

private:
	
	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;
};


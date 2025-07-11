#pragma once
#include "DxLib.h"
#include "util/Vec2.h"
#include <vector>

class Player;
class FaceFrameUi
{
public:

	FaceFrameUi();
	virtual ~FaceFrameUi();

	void Update();
	void Draw(Player& player);

private:

	//画像ハンドルこれで全ての画像をロードする
	std::vector<int> m_handles;

};


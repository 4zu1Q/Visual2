#pragma once
#include "DxLib.h"
#include <memory>

class PlayerBase;

class FaceUi
{
public:

	FaceUi();
	virtual ~FaceUi();

	void Update();
	void Draw();

private:

	std::shared_ptr<PlayerBase> m_pPlayer;

	

};


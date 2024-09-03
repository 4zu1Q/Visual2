#pragma once
#include <memory>
#include "DxLib.h"

class Player;
class SkyDome
{
public:
	SkyDome();
	virtual ~SkyDome();

	void Update(Player& player);
	void Draw();

private:
	//�X�J�C�h�[���̉�]��
	float m_rot;
	//�X�J�C�h�[���̃��f��
	int m_handle = -1;

	VECTOR m_pos = VGet(0, 0, 0);

};


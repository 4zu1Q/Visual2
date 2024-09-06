#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class ItemBase
{
public:
	ItemBase();
	virtual ~ItemBase() {};

	//�����o�[�ϐ��ɃA�N�Z�X����
	void SetHandle(int handle) { m_handle = handle; }
	
	void Start(VECTOR pos) { m_pos = pos; }
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�A�C�e���������鏈��
	void ItemLost();

	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	bool ItemSphereFlag(std::shared_ptr<Player> pPlayer);


protected:

	//std::shared_ptr<Player> m_pPlayer;

	//�n���h��
	int m_handle;

	//���W
	VECTOR m_pos;

	//���a
	float m_radius;
};


#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class EnemyBase
{
public:
	EnemyBase();
	virtual ~EnemyBase() {};

	//�����o�[�ϐ��ɃA�N�Z�X����
	void SetHandle(int handle) { m_handle = handle; }

	void Start(VECTOR pos) { m_pos = pos; }
	virtual void Update() = 0;
	virtual void Draw() = 0;

	//�G�������鏈��
	void EnemyLost();

	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }


	//�v���C���[�ƓG�̓����蔻��
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�v���C���[�̍U���ƓG�̓����蔻��
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�v���C���[�̕K�E�Z�ƓG�̓����蔻��
	bool SkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍U������
	bool DamageSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍��G�͈�
	bool SearchSphereFlag(std::shared_ptr<Player> pPlayer);
	//�G���~�܂�͈�
	bool StopSphereFlag(std::shared_ptr<Player> pPlayer);

protected:


	//�n���h��
	int m_handle;

	//���W
	VECTOR m_pos;
	VECTOR m_attackPos;

	//Hp
	int m_hp;

	//�����蔻��̔��a
	float m_radius;
	float m_searchRadius;
	float m_stopRadius;


};


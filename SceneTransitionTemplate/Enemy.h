#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Enemy
{
public:

	Enemy();
	virtual ~Enemy();

	void Load();
	void Delete();

	void Init();
	void Update();
	void Draw();

	//�v���C���[�ƓG�̓����蔻��
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�v���C���[�̍U���ƓG�̓����蔻��
	bool AttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍U������
	bool DamageSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍��G�͈�
	bool SearchSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G���~�܂�͈�
	bool StopSphereHitFlag(std::shared_ptr<Player> pPlayer);



	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//�G��HP���擾
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//�G���������邩�����߂�
	enum State
	{
		kIdle,	//�ҋ@
		kRun,	//����
		kAttack,	 //�U��
		kPowerAttack,//���ߍU��
	};



private:

	std::shared_ptr<Player> m_pPlayer;

	//���f���n���h��
	//int m_modelHandle[8];
	int m_modelHandle;
	//�V�F�[�_�n���h��
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	//�\�����
	VECTOR m_pos;
	VECTOR m_attackPos;

	//�����蔻��̔��a
	float m_radius;
	float m_searchRadius;
	float m_stopRadius;

	//State�ϐ�
	State m_state;
};


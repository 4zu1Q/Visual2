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
	void Update(std::shared_ptr<Player> pPlayer);
	void Draw();

	//�v���C���[�ƓG�̓����蔻��
	bool SphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�v���C���[�̍U���ƓG�̓����蔻��
	bool PlayerAttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�v���C���[�̕K�E�Z�ƓG�̓����蔻��
	bool PlayerSkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍U������
	bool EnemyAttackSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍U������
	bool EnemySkillSphereHitFlag(std::shared_ptr<Player> pPlayer);
	//�G�̍��G�͈�
	bool SearchSphereFlag(std::shared_ptr<Player> pPlayer);
	//�G���~�܂�͈�
	bool StopSphereFlag(std::shared_ptr<Player> pPlayer);

	const bool& GetAttackGeneration() const { return m_isAttackGeneration; }
	const bool& GetSkillGeneration() const { return m_isSkillGeneration; }

	void SetAttackGeneration(bool isAttackGeneration) { m_isAttackGeneration = isAttackGeneration; }
	void SetSkillGeneration(bool isSkillGeneration) { m_isSkillGeneration = isSkillGeneration; }

	void SetAttackAnim(bool isAttackAnim) { m_isAttackAnim = isAttackAnim; }
	void SetSkillAnim(bool isSkillAnim) { m_isSkillAnim = isSkillAnim; }


	float GetRadius() { return m_radius; }
	VECTOR GetPos() { return m_pos; }
	void SetPos(VECTOR pos) { m_pos = pos; }

	//�G��HP���擾
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }



	//�v���C���[�_���[�W�̃t���O�擾
	void SetDamage(const bool damage) { m_isDamage = damage; }

	//�G���������邩�����߂�
	enum State
	{
		kIdle,	//�ҋ@
		kRun,	//����
		kAttack,	 //�U��
		kDeath,		//����
	};

	State GetState() { return m_state; }
	void SetState(State state) { m_state = state; }

private:
	//�A�j���[�V�����̐i�s
//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo, bool isStayEndAnim = false);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	void Move();

private:


	//���f���n���h��
	//int m_modelHandle[8];
	int m_modelH;
	int m_modelWeponH;
	int m_hpFlameH;

	int m_sH;
	int m_pH;

	//�V�F�[�_�n���h��
	int m_outlineVsH;
	int m_outlinePsH;

	//Hp
	int m_hp;

	int m_damageFrame;
	bool m_isDamage;
	
	int m_rand;
	int m_random;
	int m_workFrame;

	//�\�����
	VECTOR m_pos;
	VECTOR m_attackPos;
	VECTOR m_attackDir;
	VECTOR m_velocity;
	VECTOR m_direction;
	VECTOR m_dirPos;
	float m_angle;

	//�A�j���[�V�������
	int m_animIndex;
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�����蔻��̔��a
	float m_radius;
	float m_skillRadius;
	float m_searchRadius;
	float m_stopRadius;

	//�ړ����邩�ǂ����̃t���O
	bool m_isMove;

	//�A�j���[�V�����t���O
	bool m_isIdleAnim;
	bool m_isRunAnim;
	bool m_isAttackAnim;
	bool m_isSkillAnim;
	bool m_isDamageAnim;
	bool m_isDeadAnim;
	bool m_isDeadingAnim;

	bool m_isAttackGeneration;
	bool m_isSkillGeneration;

	bool m_isRand;

	bool m_isEffect;

	//�t���[��
	int m_frame;
	int m_attackFrame;

	//State�ϐ�
	State m_state;

	//����Ɋւ���ϐ�
	VECTOR m_attachFramePosition;
	//MATRIX m_transMat;
	//MATRIX m_attachFrameMat;
	//MATRIX m_mixMatrix;


#ifdef _DEBUG
	bool m_isDebugFlag = false;
#endif

};


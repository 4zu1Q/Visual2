#pragma once
#include "DxLib.h"

class Player
{
public:
	Player();
	virtual ~Player();

	void Load();
	void Delete();

	void Init();
	void Update();
	void Draw();

	//�J�����̕������擾
	void SetCameraAngle(float angle) { m_cameraAngle = angle; }
	//���a�̎擾
	float GetRadius() { return m_radius; }

	//�v���C���[�̍��W���擾
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//�v���C���[��HP���擾
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//�v���C���[�̍U���̍��W���擾
	void OnGimmickHitUpdate();

	////�v���C���[�̏��
	//enum class State : int
	//{
	//	kStand,		//�����~�܂�
	//	kWalk,		//����
	//	kRun,		//����
	//	kAttack,	//�U��
	//	kJump,		//�W�����v
	//	kBigJump,	//��W�����v
	//};

	//�v���C���[�̃A�j���[�V�������
	enum class AnimKind : int
	{
		kNone = -1,	//�Ȃ�
		kUnknown,	//�s��
		kIdle = 1,		//�ҋ@
		kWalk = 2,		//����
		kRun,		//����
		kAttack = 30,	//�U��
		kJump,		//�W�����v
		kBigJump,	//��W�����v
		kFall,		//������
		kDamage,	//�_���[�W
		kStop,		//�����~�܂�
	};

private:
	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	void ShaderLoad();
	void ShaderUpdate();

	void PlayAnim(AnimKind playAnim);	//�V���ȃA�j���[�V�������Đ�

	void AttackCol(VECTOR pos);

private:
	//���f���n���h��
	int m_modelH;

	//�摜�n���h��
	int m_hpH;
	int m_losthpH;

	//�V�F�[�_�n���h��
	int m_outlineVsH;
	int m_outlinePsH;

	//HP
	int m_hp;
	int m_losthp;

	//�\�����
	VECTOR m_pos;
	VECTOR m_attackPos;


	float m_angle;

	//�U�����t���O
	bool m_isAttack;
	bool m_isJump;

	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������


	//�����蔻��̔��a
	float m_radius;

	//�J�������
	float m_cameraAngle;

	int m_currentPlayAnim;
	float m_currentAnimCount;

	int m_prevPlayAnim;
	float m_prevAnimCount;
	//float m_animBlendRate;
	bool m_isMove;

	float m_hpAnimationHeight;

	//State m_currentState;
	AnimKind m_animIndex;
	int m_animationIndex;
};


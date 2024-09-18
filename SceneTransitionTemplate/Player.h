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
	//�X�L���̔��a
	float GetSkillRadius() { return m_skillRadius; }
	//�U���̔��a
	float GetAttackRadius() { return m_attackRadius; }

	//�v���C���[�̍��W���擾
	const VECTOR& GetPos() const { return m_pos; }
	void SetPos(const VECTOR pos) { m_pos = pos; }

	//�v���C���[�̍U�����W���擾
	const VECTOR& GetAttackPos() const { return m_attackPos; }
	void SetAttackPos(const VECTOR attackPos) { m_attackPos = attackPos; }


	//�v���C���[��HP���擾
	const int& GetHp() const { return m_hp; }
	void SetHp(const int hp) { m_hp = hp; }

	//�v���C���[�̍U���̓����蔻��𐶐����邩�̃t���O���擾
	const bool& GetAttackGeneration() const { return m_isAttackGeneration; }
	const bool& GetSkillGeneration() const { return m_isSkillGeneration; }
	
	void SetCommand(const bool isCommand) { m_isCommand = isCommand; }

	//�v���C���[�̍U���̍��W���擾
	void OnGimmickHitUpdate();

	//�v���C���[�_���[�W�̃t���O�擾
	void SetDamage(const bool damage) { m_isDamage = damage; }

	//�A�j���[�V�����̃Z�b�g�t���O
	void SetAnimDamage(const bool animDamage);

	//���񂾂Ƃ��̔���
	void SetIsDown(const bool isDown) { m_isDown = isDown; }

	//�v���C���[�̃A�j���[�V�������
	enum class AnimKind : int
	{
		kNone = -1,	//�Ȃ�
		kUnknown,	//�s��
		kIdle,		//�ҋ@
		kWalk,		//����
		kRun,		//����
		kAttack,	//�U��
		kSkill,		//�X�L��
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

	//�ړ��֐�
	void Move();

	void ShadowMapLoad();
	void ShadowMapInit();
	void ShadowMapUpdate();
	void ShadowMapDraw(int handle);
	void ShadowMapDelete(int handle);

private:
	//���f���n���h��
	int m_modelH;

	//�摜�n���h��
	int m_hpH;
	int m_losthpH;
	int m_hpFlameH;

	//�G�t�F�N�g�n���h��
	int m_effectPH;
	int m_effectSH;

	//�T�E���h�n���h��
	int m_damageSeH;
	int m_axeAttackSeH;
	int m_axeSkillSeH;

	//�V�F�[�_�n���h��
	int m_outlineVsH;
	int m_outlinePsH;

	//HP
	int m_hp;
	int m_losthp;

	//�X�^�~�i
	float m_stamina;
	bool m_isStamina;

	float m_analogX;
	float m_analogZ;

	//�\�����
	VECTOR m_pos;
	VECTOR m_attackPos;
	VECTOR m_attackDir;

	float m_angle;

	//�U�����t���O
	bool m_isAttack;
	bool m_isSkill;

	bool m_isWalk;
	bool m_isDash;
	bool m_isDown;
	bool m_isAvoid;

	bool m_isCommand;

	//�A�j���[�V�������
	int m_animIndex;
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������

	//�A�j���[�V�����t���O
	bool m_isAnimIdle;
	bool m_isAnimAttack;
	bool m_isAnimWalk;
	bool m_isAnimDash;
	bool m_isAnimDamage;
	bool m_isAnimDown;

	//�����蔻��̔��a
	float m_radius;
	float m_skillRadius;
	float m_attackRadius;

	//�����蔻��̔����t���O
	bool m_isAttackGeneration;
	bool m_isSkillGeneration;

	//�J�������
	float m_cameraAngle;

	int m_currentPlayAnim;
	float m_currentAnimCount;

	int m_prevPlayAnim;
	float m_prevAnimCount;
	//float m_animBlendRate;
	bool m_isMove;

	float m_hpAnimationHeight;

	//�v���C���[�̓_��
	int m_damageFrame;
	bool m_isDamage;

	//State m_currentState;
	AnimKind m_animKind;

	//�V���h�E�}�b�v�ϐ�
	int m_shadowMapH;
	float m_shadowAngle;
	VECTOR m_lightDirection;


};


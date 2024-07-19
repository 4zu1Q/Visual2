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

	//�v���C���[�̍U���̍��W���擾


private:
	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex);

	void ShaderLoad();
	void ShaderUpdate();

private:
	//���f���n���h��
	int m_modelHandle;
	//�V�F�[�_�n���h��
	int m_outlineVsH;
	int m_outlinePsH;



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
	//0.0f:prev���Đ������
	//1.0f:current���Đ������

	//�����蔻��̔��a
	float m_radius;

	//�J�������
	float m_cameraAngle;


};



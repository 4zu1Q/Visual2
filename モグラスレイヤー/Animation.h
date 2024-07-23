#pragma once
#include "DxLib.h"

class Animation
{
public:
	Animation();
	virtual ~Animation();

	void Load();
	void Delete();

	void Init();
	void Update();

	//�A�j���[�V�����̐i�s
	//���[�v�������ǂ�����Ԃ�
	bool UpdateAnim(int attachNo ,int modelHandle);

	//�A�j���[�V�����̕ύX
	void ChangeAnim(int animIndex,int modelHandle);

private:
	
	//�A�j���[�V�������
	int m_currentAnimNo;	//���݂̃A�j���[�V����
	int m_prevAnimNo;		//�ύX�O�̃A�j���[�V����
	float m_animBlendRate;	//�A�j���[�V������������
							//0.0f:prev���Đ������
							//1.0f:current���Đ������



};


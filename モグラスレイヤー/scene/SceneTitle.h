#pragma once
#include "SceneBase.h"

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    virtual ~SceneTitle();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���
    
private:

	//�J�[�\���I��
	enum Select
	{
		kStart,		//�X�^�[�g
		kOption,	//�I�v�V����
		kGameEnd,	//�I��
	};

	Select m_select;

	//�O���t�B�b�N�n���h��
	int m_logoH;
	int m_startH;
	int m_optionH;
	int m_endH;

	int m_frameScene;
	int m_frameCount;
	bool m_isInterval;

};


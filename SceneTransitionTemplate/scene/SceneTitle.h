#pragma once
#include "SceneBase.h"

class SceneManager;
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

	bool GetisClose() { return m_isClose; }

private:
	//�V�[�����Ǘ�����|�C���^
	std::shared_ptr<SceneManager> m_pManager = std::make_shared<SceneManager>();


	//�J�[�\���I��
	enum Select
	{
		kStart,		//�X�^�[�g
		kOperation,	//�I�v�V����
		kGameEnd,	//�I��
	};

	//�J�[�\���I��
	enum Option
	{
		kOperator,		//�������
		kSound,			//�T�E���h
		kFullScreen,	//�t���X�N���[��
	};

	//enum�ϐ�
	Select m_select;
	Option m_option;
	int m_selectPosY;

	float m_selectAnimation;

	//�O���t�B�b�N�n���h��
	int m_logoH;
	int m_startH;
	int m_optionH;
	int m_endH;
	int m_operationH;
	int m_selectH;
	int m_backgroundH;
	

	//�t���[���ϐ�
	int m_frameScene;
	int m_frameCount;

	//�t���O�ϐ�
	bool m_isInterval;	//�t�F�[�h�p
	bool m_isOption;	//�I�v�V������ʗp
	bool m_isCommand;	//���肵����ɓ������Ȃ��p
	bool m_isFullScreen;//�t���X�N���[���p
	bool m_isSound;		//�T�E���h�ݒ�p
	bool m_isOperator;	//������ʕ\���p

	//�T�E���h�n���h��
	int m_soundBgmH;
	int m_soundSelectH;
	int m_soundCancelH;
	int m_soundDecsionH;

	bool m_isClose;
};


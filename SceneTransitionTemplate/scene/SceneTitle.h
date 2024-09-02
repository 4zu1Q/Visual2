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
	//�V�[�����Ǘ�����|�C���^
	//std::shared_ptr<SceneManager> m_pManager = std::make_shared<SceneManager>();


	//�J�[�\���I��
	enum Select
	{
		kStart,		//�X�^�[�g
		kOption,	//�I�v�V����
		kGameEnd,	//�I��
	};

	//enum�ϐ�
	Select m_select;
	int m_selectPosY;

	//�O���t�B�b�N�n���h��
	int m_logoH;
	int m_startH;
	int m_optionH;
	int m_endH;
	int m_selectH;
	int m_backgroundH;

	//�t���[���ϐ�
	int m_frameScene;
	int m_frameCount;

	//�t���O�ϐ�
	bool m_isInterval;
	bool m_isCommand;
};


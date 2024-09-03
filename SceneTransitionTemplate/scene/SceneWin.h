#pragma once
#include "SceneBase.h"
#include "DxLib.h"


class SceneManager;
class SceneWin :
    public SceneBase
{
public:
	SceneWin();
	virtual ~SceneWin();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:

	//�n���h���ϐ�
	int m_gameclearH;
	int m_retryH;
	int m_titleH;
	int m_selectH;

	int m_frameScene;

	//�J�[�\���I��
	enum Select
	{
		kRetry,		//�X�^�[�g
		kTitle,		//�I��
	};

	Select m_select;

	//�t���O�ϐ�
	bool m_isInterval;
	bool m_isCommand;

};


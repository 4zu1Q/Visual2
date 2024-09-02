#pragma once
#include "SceneBase.h"
class SceneDebug :
    public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:

	//�J�[�\���I��

	enum Select
	{
		kSceneDebug,
		kSceneTitle,
		kScenePlaying,
		kSceneWin,
		kSceneLose,
	};

	Select m_select;	//�I��ϐ�
};


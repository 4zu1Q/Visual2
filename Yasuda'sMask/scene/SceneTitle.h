#pragma once
#include "SceneBase.h"
#include <memory>

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

#ifdef _DEBUG

	enum Select
	{
		kSceneSelect,
		kScenePlaying,
		kSceneWin,
		kSceneLose,
	};
	Select m_select;	//�I��ϐ�

#endif

	int m_frameCount;


};


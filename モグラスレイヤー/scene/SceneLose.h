#pragma once
#include "SceneBase.h"
class SceneLose :
    public SceneBase
{
public:
	SceneLose();
	virtual ~SceneLose();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:

#ifdef _DEBUG



#endif

};


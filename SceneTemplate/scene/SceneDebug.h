#pragma once
#include "SceneBase.h"

class SceneDebug :
	public SceneBase
{
public:
	SceneDebug();
	virtual ~SceneDebug();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:

};


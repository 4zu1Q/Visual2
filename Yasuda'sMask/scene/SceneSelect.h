#pragma once
#include "SceneBase.h"
class SceneSelect :
	public SceneBase
{
public:
	SceneSelect();
	virtual ~SceneSelect();

	virtual void Init();
	virtual std::shared_ptr<SceneBase> Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:
	

};


#pragma once
#include "SceneBase.h"

class SceneTitle :
    public SceneBase
{
public:
    SceneTitle();
    virtual ~SceneTitle();

	virtual void Init()  override final;	//�V�[���ɓ���Ƃ��̏���������
	virtual void End() override final;	//�V�[���𔲂���Ƃ��̏���

	virtual std::shared_ptr<SceneBase> Update() override final;	//���t���[���s���X�V����
	virtual void Draw() override final;	//���t���[���s���`�揈��
    
private:
	int m_frameCount;

	

};


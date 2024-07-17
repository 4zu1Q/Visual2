#pragma once
#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual void Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��
	virtual void End();	//�V�[���𔲂���Ƃ��̏���

private:
	std::shared_ptr<SceneBase> m_pScene;

};


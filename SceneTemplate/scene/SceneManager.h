#pragma once
#include <memory>

class SceneBase;
class SceneManager
{
public:
	SceneManager();
	virtual ~SceneManager();

	virtual void Init();	//�V�[���ɓ���Ƃ��̏���������
	virtual void End();		//�V�[���𔲂���Ƃ��̏���

	virtual void Update();	//���t���[���s���X�V����
	virtual void Draw();	//���t���[���s���`�揈��

private:
	//�V�[���̃X�}�[�g�|�C���^
	std::shared_ptr<SceneBase> m_pScene;

	float m_updateTime;	//�X�V�ɂ����鎞��
	float m_drawTime;	//�`��ɂ����鎞��
};


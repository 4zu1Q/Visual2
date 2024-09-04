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

	//�Q�[���I���̃t���O�擾����
	virtual bool& GetClose() { return m_isClose; }
	virtual void SetClose(bool close) { m_isClose = close; }


private:
	std::shared_ptr<SceneBase> m_pScene;

	//�Q�[�����I��������t���O
	bool m_isClose = false;

};


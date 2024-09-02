#pragma once
#include <memory>
#include "SceneBase.h"

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

	/// <summary>
	/// ���C��while�����I��������
	/// </summary>
	/// <returns>�Q�[�����I�������邩 true:�I�� false:�I�����Ȃ�</returns>
	bool Close() const { return m_isClose; };

	// �I���t���O��ύX����(�I�����邩)
	void SetCloseFlag(const bool flag) { m_isClose = flag; }

private:
	std::shared_ptr<SceneBase> m_pScene;

	bool m_isClose = false;		//�I���t���O

};


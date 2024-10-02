#pragma once
#include <memory>


class SceneBase :public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase() {}
	virtual ~SceneBase() {}

	
	virtual void Init() = 0;	//�V�[���ɓ���Ƃ��̏���������
	virtual void End() = 0;		//�V�[���𔲂���Ƃ��̏���

														//���t���[���s���X�V����
	virtual std::shared_ptr<SceneBase> Update() = 0;	//�V�[����ύX����ꍇ�͑J�ڐ�̃V�[���̃|�C���^
														//�V�[���ύX���Ȃ��ꍇ�͎��g�̃|�C���^
										
	virtual void Draw() = 0;	//���t���[���s���`�揈��

	/*�t�F�[�h�֘A*/
	//void UpdateFade();
	//void DrawFade() const;


	//�V�[���̎��
	enum SceneKind
	{
		kSceneDebug,		//�f�o�b�O�V�[��
		kSceneTitle,		//�^�C�g���V�[��
		kSceneSelect,		//�Z���N�g�V�[��
		kSceneGamePlay,		//�Q�[���v���C�V�[��
		kSceneGameOver,		//�Q�[���I�[�o�[�V�[��
		kSceneGameClear,	//�Q�[���N���A�V�[��
	};

	SceneKind m_sceneKind;

private:

	//�t�F�[�h�֘A����
	int m_fadeColor;
	int m_fadeBright;
	int m_fadeSpeed;

};


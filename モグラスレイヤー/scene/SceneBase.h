#pragma once
#include <memory>

class SceneManager;
class SceneBase :public std::enable_shared_from_this<SceneBase>
{
public:
	SceneBase();
	virtual ~SceneBase() {}

	
	virtual void Init() = 0;	//�V�[���ɓ���Ƃ��̏���������

														//���t���[���s���X�V����
	virtual std::shared_ptr<SceneBase> Update() = 0;	//�V�[����ύX����ꍇ�͑J�ڐ�̃V�[���̃|�C���^
														//�V�[���ύX���Ȃ��ꍇ�͎��g�̃|�C���^
										
	virtual void Draw() = 0;	//���t���[���s���`�揈��
	virtual void End() = 0;	//�V�[���𔲂���Ƃ��̏���

	//�t�F�[�h�֌W
	void UpdateFade();
	void DrawFade();

	void SkipFadeIn();
	void SkipFadeOut();

	bool IsFadeingIn() const;
	bool IsFadeingOut() const;

	bool IsFadeing() const { return IsFadeingIn() || IsFadeingOut(); }	//�t�F�[�h�C��or�A�E�g��

	void StartFadeOut();	//�t�F�[�h�A�E�g�J�n



private:
	//�t�F�[�h�֘A
	int m_fadeColor;
	int m_fadeBright;
	int m_fadeSpeed;

protected:
	SceneManager& m_pManager;

};


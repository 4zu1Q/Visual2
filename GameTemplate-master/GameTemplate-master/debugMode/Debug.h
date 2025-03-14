#ifdef _DEBUG
#pragma once
#include "DxLib.h"

class Debug
{
public:

private:
	Debug() = default;
	virtual ~Debug() = default;

	static Debug* m_instance;

public:
	// �R�s�[�R���X�g���N�^�̋֎~
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;

	static Debug& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new Debug;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	void update();
	void draw();

	// �Q�[����ʕ`���̏���	�X�N�V���̎B�e�Ɏg�p����
	void postDraw();

	// �f�o�b�O��ʂ��J���Ă��邩�ǂ���
	bool isActive() const { return m_isActive; }

	// �Q�[����Update���s���Ă悢��
	// �f�o�b�O���j���[���͊�{�I�ɒ�~���āA�R�}����̓{�^���ŉ\
	bool isGameUpdate() const;

	// �X�N���[���V���b�g�̎B�e
	// ���Ă��Ȃ��ꍇ�t�@�C������������
	void screenShot(const char* fileName = nullptr);

	// ���Ԍv��
	void onStartUpdate();	// update�J�n���ɌĂ�
	void onEndUpdate();	// update�I�����ɌĂ�
	void onStartDraw();	// draw�J�n���ɌĂ�
	void onEndDraw();		// draw�I�����ɌĂ�

private:
	// �e�p�[�c�\��
	void drawBg() const;
	void drawFps() const;
	void drawProcessBar() const;	// �����o�[�̕\��
private:

	// �f�o�b�O�\���ݒ�
	enum class DispType
	{
		Always,			// ��ɕ\��
		DebugActive,	// �f�o�b�O���j���[���J���Ă���ԕ\��
		None,			// �\�����Ȃ�
	};

	// �f�o�b�O���j���[�\���ݒ�
	struct DebugSetting
	{
		int bgAlpha = 7;	// �f�o�b�O���j���[�̔w�i�����x	0~10
		DispType dispFps = DispType::DebugActive;	// FPS�\��
		DispType dispProcessBar = DispType::Always;	// �����o�[�\��
		bool isOpenTrg = false;		// �g���K�[�ŊJ��	�����Ă���ԊJ��	

		// �f�o�b�O�\���̃N���A
		
		// �f�o�b�O�ݒ�̕ۑ�
	};

private:
	// �f�o�b�O�@�\�J���Ă���
	bool m_isActive = false;
	// �X�N���[���V���b�g�B�e
	bool m_isSaveScreenShot = false;	// ���̃t���[���̃X�N���[���V���b�g���B�e

	// �����o�[
	LONGLONG	m_updateTime = 0;	// update�ɂ�����������
	LONGLONG	m_drawTime = 0;		// draw�ɂ�����������

	// �f�o�b�O�ݒ�
	DebugSetting m_setting;
};

#endif


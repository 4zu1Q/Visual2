#include "DxLib.h"

#include <memory>
#include "game.h"
#include "SceneManager.h"

#include "ConfigData.h"
#include "FontManager.h"

#include "Debug.h"

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �R���t�B�O�ݒ�	�E�C���h�E���[�h�Ȃǂ���̂ōŏ��ɓǂݍ���
	ConfigData::getInstance().load();

	// window���[�h�ݒ�
	ChangeWindowMode(ConfigData::getInstance().isWindowMode());
	// �E�C���h�E���ݒ�
	SetMainWindowText(Game::kTitleText);
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// �E�C���h�E�A�t���X�N���[���؂�ւ��ɑΉ�
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

#ifdef _DEBUG
	LONGLONG setupStart = GetNowHiPerformanceCount();
#endif
	// �t�H���g
	FontManager::getInstance().init();
	FontManager::getInstance().createFont();

#ifdef _DEBUG
	LONGLONG setupTime = GetNowHiPerformanceCount() - setupStart;
	//printfDx("�������ɂ����������� %d.%d ms\n", setupTime / 1000, setupTime % 1000);
#endif

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* pScene = new SceneManager;
	pScene->init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

#ifdef _DEBUG
		Debug::getInstance().onStartUpdate();
		Debug::getInstance().update();
		if (Debug::getInstance().isGameUpdate())
		{
			if (!pScene->update())	break;
		}
		Debug::getInstance().onEndUpdate();
#else
		if (!pScene->update())	break;
#endif

#ifdef _DEBUG
		Debug::getInstance().onStartDraw();
		pScene->draw();
		Debug::getInstance().onEndDraw();
		Debug::getInstance().postDraw();	// �Q�[����ʂ̃X�N���[���V���b�g���B�e���Ă���f�o�b�O��ʂ�`��
		Debug::getInstance().draw();
#else
		pScene->draw();
#endif

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps�Œ�
		while (GetNowHiPerformanceCount() - time < Game::kFrameMicroSec)
		{
		}
	}

	pScene->end();

	FontManager::getInstance().end();


	// �V���O���g���N���X�̔j��
	FontManager::destroy();
	ConfigData::destroy();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

#ifdef _DEBUG
	// �f�o�b�O�@�\�̏I��
	Debug::destroy();
#endif // _DEBUG


	return 0;				// �\�t�g�̏I�� 
}
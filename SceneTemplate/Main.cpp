#include "DxLib.h"
#include "util/Game.h"
#include "SceneManager.h"
#include <memory>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	//�E�B���h�E���[�h�̐ݒ�
#ifdef _DEBUG
	ChangeWindowMode(true);
#else
	ChangeWindowMode(false);
#endif


	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	//�E�B���h�E���̐ݒ�
	SetMainWindowText(Game::kTitleText);

	//��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight , Game::kColorDepth);


	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	//Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);

	//Z�o�b�t�@�ւ̏������݂��s��
	SetWriteZBuffer3D(true);

	//�|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(true);

	std::shared_ptr<SceneManager> pSceneManager = std::make_shared<SceneManager>();

	pSceneManager->Init();

	// �Q�[�����[�v
	while (ProcessMessage() == 0)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG time = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���
		pSceneManager->Update();
		pSceneManager->Draw();


		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	pSceneManager->End();



	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
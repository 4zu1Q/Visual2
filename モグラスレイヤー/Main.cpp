#include "DxLib.h"
#include "Game.h"
#include "SceneManager.h"
#include <memory>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
	ChangeWindowMode(true);
	SetGraphMode(1280,720,32);

	SetWindowText("���l�̗��H");

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	//�V�[�����Ǘ�����|�C���^
	std::shared_ptr<SceneManager> pManager = std::make_shared<SceneManager>();

	//SceneManager* pScene = new SceneManager;
	pManager->Init();
	
	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���
		pManager->Update();
		pManager->Draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

#ifdef _DEBUG
		DrawFormatString(Game::kScreenWidth - 200, 0, 0x448844, "FPS : %2.2f", fps);
		DrawFormatString(Game::kScreenWidth - 200, 16, 0x448844, "DC : %d", DC);
#endif

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}

		if (pManager->Close()) 
		{
			break;
		}
	}
	
	pManager->End();


	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I��
}
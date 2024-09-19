#include "DxLib.h"
#include <EffekseerForDXLib.h>
#include "SceneManager.h"
#include <memory>

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// �ꕔ�̊֐���DxLib_Init()�̑O�Ɏ��s����K�v������
#ifdef _DEBUG
	ChangeWindowMode(true);
	
#else
	ChangeWindowMode(false);
#endif

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

	Effekseer_Init(8000);
	Effekseer_InitDistortion();
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	//�V�[�����Ǘ�����|�C���^
	std::shared_ptr<SceneManager> pSceneManager = std::make_shared<SceneManager>();

	pSceneManager->Init();

	//int sH = LoadEffekseerEffect(L"path");
	//int pH = PlayEffekseer3DEffect(sH);
	//SetPosPlayingEffekseer3DEffect(pH, x, y, z);
	//StopEffekseer3DEffect(pH);
	//DeleteEffekseerEffect(sH);
	
	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		// �Q�[���̏���
		pSceneManager->Update();
		UpdateEffekseer3D();
		pSceneManager->Draw();
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// ����p�̃t���O��true�̏ꍇ
		if (pSceneManager->m_isClose)
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}
	}
	
	pSceneManager->End();

	Effkseer_End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I��
}
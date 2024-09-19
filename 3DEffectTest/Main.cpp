#include <cmath>
#include "DxLib.h"
#include <EffekseerForDXLib.h>
#include <list>

namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;
}

void DrawGrid()
{
	for (int x = -50; x <= 50; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -50), VGet(static_cast<float>(x), 0, 50), 0xffff00);
	}
	for (int z = -50; z <= 50; z += 10)
	{
		DrawLine3D(VGet(-50, 0, static_cast<float>(z)), VGet(50, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);



}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	Effekseer_Init(8000);
	Effekseer_InitDistortion();
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	struct EffData
	{
		int pH;
		VECTOR pos = VGet(0, 0, 0);
		int frame = 0;
		bool isEnd = false;
	};

	

	int sH = LoadEffekseerEffect("data/SyoumetuEffect2.efkefc");
//	int pH = PlayEffekseer3DEffect(sH);
	std::list<EffData> effList;

	int frame = 0;

	float cameraAngle = -DX_PI_F / 2;

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();

		frame++;


		UpdateEffekseer3D();

		if (frame > 30)
		{
			frame = 0;
			EffData data;
			data.pH = PlayEffekseer3DEffect(sH);
			effList.emplace_back(data);
		}
		for (auto& data : effList)
		{
			data.frame++;
			if (data.frame > 240)
			{
				data.isEnd = true;
				StopEffekseer3DEffect(data.pH);
				continue;
			}
			data.pos = VAdd(data.pos, VGet(0.5f, 0, 0));
			SetPosPlayingEffekseer3DEffect(data.pH, data.pos.x, data.pos.y, data.pos.z);
		}
		effList.remove_if(
			[](const auto& data)
			{
				return data.isEnd;
			});

		if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_LEFT))
		{
			cameraAngle += 0.05f;
		}
		if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_RIGHT))
		{
			cameraAngle -= 0.05f;
		}
		
		// �J�����̂RD�������킹�Ă���
		Effekseer_Sync3DSetting();
		// �G�t�F�N�V�A�̏����X�V���Ă���
		UpdateEffekseer3D();
		// �G�t�F�N�V�A��`�悵�Ă���
		DrawEffekseer3D();


		SetCameraNearFar(1.0f, 180.0f);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));

		//StopEffekseer3DEffect(pH);


		DrawGrid();
		DrawFormatString(0, 16, 0xffffff, "frame:%d", frame);

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}
	DeleteEffekseerEffect(sH);

	Effkseer_End();
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
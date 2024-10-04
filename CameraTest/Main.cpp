#include "DxLib.h"
#include "Pad.h"
#include <cmath>

namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 32;

	constexpr float kMaxSpeed = 0.9f;

	constexpr float kAnalogRangeMin = 0.1f;
	constexpr float kAnalogRangeMax = 0.8f;

	constexpr float kAnalogInputMax = 1000.0f;
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

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	VECTOR playerPos = VGet(0,0,0);
	VECTOR enemyPos = VGet(0, 0, 20);
	VECTOR cameraPos2 = VGet(0, 0, 10);

	//�A�i���O�X�e�B�b�N���g���Ĉړ�
	int analogX = 0;
	int analogZ = 0;
	float radius = 5.0f;

	float angle = -DX_PI_F / 2;

	float cameraAngle = -DX_PI_F / 2;

	bool isTarget = false;

	while (ProcessMessage() == 0)
	{
		Pad::Update();

		LONGLONG  time = GetNowHiPerformanceCount();
		// ��ʂ̃N���A
		ClearDrawScreen();


		if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_6))
		{
			cameraAngle += 0.05f;
		}
		if ((GetJoypadInputState(DX_INPUT_KEY_PAD1) & PAD_INPUT_5))
		{
			cameraAngle -= 0.05f;
		}

		if (Pad::IsTrigger(PAD_INPUT_7) && isTarget == false) isTarget = true;
		if (Pad::IsTrigger(PAD_INPUT_7) && isTarget == true) isTarget = false;


		/*�v���C���[�̈ړ�*/

		GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
		VECTOR move = VGet(analogX, 0.0f, -analogZ);
		float len = VSize(move);
		float rate = len / kAnalogInputMax;

		//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
		rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
		rate = min(rate, 1.0f);
		rate = max(rate, 0.0f);

		//���x������ł���̂ňړ��x�N�g���ɔ��f����
		move = VNorm(move);
		float speed = kMaxSpeed * rate;
		move = VScale(move, speed);

		playerPos = VAdd(playerPos, move);

		/*�e�X�g���̃J�����ړ�*/

		if (!isTarget)
		{
			VECTOR aimPos = VGet(playerPos.x, playerPos.y, playerPos.z);
			//�x�N�g���̕���(�����_-�J�����̃|�W�V����)
			VECTOR posToAim = VSub(aimPos, cameraPos2);

			VECTOR scalePosToAim = VScale(posToAim, 0.9f);

			//�A�i���O�X�e�B�b�N���g���Ĉړ�
			int analogCameraX = 0;
			int analogCameraY = 0;

			GetJoypadAnalogInputRight(&analogCameraX, &analogCameraY, DX_INPUT_PAD1);

			if (analogCameraX >= 10) angle -= 0.05f;
			if (analogCameraX <= -10) angle += 0.05f;

			cameraPos2.x += cosf(angle) * 36;
			cameraPos2.y += 0;
			cameraPos2.z += sinf(angle) * 36;


			cameraPos2 = VAdd(cameraPos2, posToAim);
		}
		else
		{

		}


		



		/*�J�����̏���*/
		SetCameraNearFar(1.0f, 500.0f);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));


		/*�`�揈��*/

		DrawGrid();

		DrawSphere3D(playerPos, radius, 8, 0xffffff, 0xffffff, false);
		DrawSphere3D(enemyPos, radius, 8, 0x00ff00, 0x00ff00, false);
		DrawSphere3D(cameraPos2, radius, 8, 0x00ffff, 0x00ffff, false);

		//����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
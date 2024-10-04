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

	// X+-,Z+-の方向が分かりやすいように表示を追加する
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);
}

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(true);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	VECTOR playerPos = VGet(0,0,0);
	VECTOR enemyPos = VGet(0, 0, 20);
	VECTOR cameraPos2 = VGet(0, 0, 10);

	//アナログスティックを使って移動
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
		// 画面のクリア
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


		/*プレイヤーの移動*/

		GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
		VECTOR move = VGet(analogX, 0.0f, -analogZ);
		float len = VSize(move);
		float rate = len / kAnalogInputMax;

		//アナログスティック無効な範囲を除外する
		rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
		rate = min(rate, 1.0f);
		rate = max(rate, 0.0f);

		//速度が決定できるので移動ベクトルに反映する
		move = VNorm(move);
		float speed = kMaxSpeed * rate;
		move = VScale(move, speed);

		playerPos = VAdd(playerPos, move);

		/*テスト中のカメラ移動*/

		if (!isTarget)
		{
			VECTOR aimPos = VGet(playerPos.x, playerPos.y, playerPos.z);
			//ベクトルの方向(注視点-カメラのポジション)
			VECTOR posToAim = VSub(aimPos, cameraPos2);

			VECTOR scalePosToAim = VScale(posToAim, 0.9f);

			//アナログスティックを使って移動
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


		



		/*カメラの処理*/
		SetCameraNearFar(1.0f, 500.0f);
		VECTOR cameraPos;
		cameraPos.x = cosf(cameraAngle) * kCameraDist;
		cameraPos.y = kCameraHeight;
		cameraPos.z = sinf(cameraAngle) * kCameraDist;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));


		/*描画処理*/

		DrawGrid();

		DrawSphere3D(playerPos, radius, 8, 0xffffff, 0xffffff, false);
		DrawSphere3D(enemyPos, radius, 8, 0x00ff00, 0x00ff00, false);
		DrawSphere3D(cameraPos2, radius, 8, 0x00ffff, 0x00ffff, false);

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
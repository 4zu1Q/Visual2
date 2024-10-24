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

	Effekseer_Init(8000);
	Effekseer_InitDistortion();
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();

	// ダブルバッファモード
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
		// 画面のクリア
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
		
		// カメラの３D情報を合わせている
		Effekseer_Sync3DSetting();
		// エフェクシアの情報を更新している
		UpdateEffekseer3D();
		// エフェクシアを描画している
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

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}
	DeleteEffekseerEffect(sH);

	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
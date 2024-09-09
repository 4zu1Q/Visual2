#include "DxLib.h"
#include <EffekseerForDXLib.h>
#include "SceneManager.h"
#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
#ifdef _DEBUG
	ChangeWindowMode(true);
#endif
	ChangeWindowMode(false);

	SetGraphMode(1280,720,32);


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	Effekseer_Init(8000);
	Effekseer_InitDistortion();
	SetChangeScreenModeGraphicsSystemResetFlag(false);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	//シーンを管理するポインタ
	std::shared_ptr<SceneManager> pSceneManager = std::make_shared<SceneManager>();

	pSceneManager->Init();

	//int sH = LoadEffekseerEffect(L"path");
	//int pH = PlayEffekseer3DEffect(sH);
	//SetPosPlayingEffekseer3DEffect(pH, x, y, z);
	//StopEffekseer3DEffect(pH);
	//DeleteEffekseerEffect(sH);
	
	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		pSceneManager->Update();
		UpdateEffekseer3D();
		pSceneManager->Draw();
		Effekseer_Sync3DSetting();
		DrawEffekseer3D();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// 閉じる用のフラグがtrueの場合
		if (pSceneManager->m_isClose)
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}
	
	pSceneManager->End();

	Effkseer_End();
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了
}
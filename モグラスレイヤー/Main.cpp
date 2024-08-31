#include "DxLib.h"
#include "Game.h"
#include "SceneManager.h"
#include <memory>

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	ChangeWindowMode(true);
	SetGraphMode(1280,720,32);

	SetWindowText("小人の旅路");

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetUseBackCulling(true);

	//シーンを管理するポインタ
	std::shared_ptr<SceneManager> pManager = std::make_shared<SceneManager>();

	//SceneManager* pScene = new SceneManager;
	pManager->Init();
	
	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		pManager->Update();
		pManager->Draw();

		auto fps = GetFPS();
		auto DC = GetDrawCallCount();

#ifdef _DEBUG
		DrawFormatString(Game::kScreenWidth - 200, 0, 0x448844, "FPS : %2.2f", fps);
		DrawFormatString(Game::kScreenWidth - 200, 16, 0x448844, "DC : %d", DC);
#endif

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}

		if (pManager->Close()) 
		{
			break;
		}
	}
	
	pManager->End();


	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了
}
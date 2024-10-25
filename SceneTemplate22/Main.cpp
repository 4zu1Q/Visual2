#include "DxLib.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "util/Game.h"
#include "util/Pad.h"

#include <memory>

//デバッグ用include
#include "SceneDebug.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// 一部の関数はDxLib_Init()の前に実行する必要がある
	//ウィンドウモードの設定
#ifdef _DEBUG
	ChangeWindowMode(true);
#else
	ChangeWindowMode(false);
#endif


	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	//ウィンドウ名の設定
	SetMainWindowText(Game::kTitleText);

	//画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight , Game::kColorDepth);


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//Zバッファを使用する
	SetUseZBuffer3D(true);

	//Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);

	//ポリゴンの裏面を描画しない
	SetUseBackCulling(true);

	//std::shared_ptr<SceneManager> pSceneManager = std::make_shared<SceneManager>();
	SceneManager pSceneManager;

#ifdef _DEBUG
	//pSceneManager.ChangeScene(std::make_shared<SceneTitle>(pSceneManager));
	pSceneManager.ChangeScene(std::make_shared<SceneDebug>(pSceneManager));
#else
	pSceneManager.ChangeScene(std::make_shared<SceneTitle>(pSceneManager));
#endif


	// ゲームループ
	while (ProcessMessage() == 0)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG time = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		// ゲームの処理
		pSceneManager.Update();
		pSceneManager.Draw();


		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
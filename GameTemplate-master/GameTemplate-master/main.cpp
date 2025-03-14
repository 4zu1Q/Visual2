#include "DxLib.h"

#include <memory>
#include "game.h"
#include "SceneManager.h"

#include "ConfigData.h"
#include "FontManager.h"

#include "Debug.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// コンフィグ設定	ウインドウモードなどあるので最初に読み込み
	ConfigData::getInstance().load();

	// windowモード設定
	ChangeWindowMode(ConfigData::getInstance().isWindowMode());
	// ウインドウ名設定
	SetMainWindowText(Game::kTitleText);
	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	// ウインドウ、フルスクリーン切り替えに対応
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

#ifdef _DEBUG
	LONGLONG setupStart = GetNowHiPerformanceCount();
#endif
	// フォント
	FontManager::getInstance().init();
	FontManager::getInstance().createFont();

#ifdef _DEBUG
	LONGLONG setupTime = GetNowHiPerformanceCount() - setupStart;
	//printfDx("初期化にかかった時間 %d.%d ms\n", setupTime / 1000, setupTime % 1000);
#endif

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager* pScene = new SceneManager;
	pScene->init();

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();
		// 画面のクリア
		ClearDrawScreen();

#ifdef _DEBUG
		Debug::getInstance().onStartUpdate();
		Debug::getInstance().update();
		if (Debug::getInstance().isGameUpdate())
		{
			if (!pScene->update())	break;
		}
		Debug::getInstance().onEndUpdate();
#else
		if (!pScene->update())	break;
#endif

#ifdef _DEBUG
		Debug::getInstance().onStartDraw();
		pScene->draw();
		Debug::getInstance().onEndDraw();
		Debug::getInstance().postDraw();	// ゲーム画面のスクリーンショットを撮影してからデバッグ画面を描画
		Debug::getInstance().draw();
#else
		pScene->draw();
#endif

		//裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps固定
		while (GetNowHiPerformanceCount() - time < Game::kFrameMicroSec)
		{
		}
	}

	pScene->end();

	FontManager::getInstance().end();


	// シングルトンクラスの破棄
	FontManager::destroy();
	ConfigData::destroy();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

#ifdef _DEBUG
	// デバッグ機能の終了
	Debug::destroy();
#endif // _DEBUG


	return 0;				// ソフトの終了 
}
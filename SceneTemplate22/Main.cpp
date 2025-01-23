#include "DxLib.h"

#include "myLib/MyLib.h"

//#include "EffekseerForDXLib.h"

#include "SceneManager.h"
#include "SceneTitle.h"

#include "util/Game.h"
#include "util/Pad.h"
#include "util/SoundManager.h"
#include "util/EffectManager.h"
#include "util/Font.h"
#include  "util/Setting.h"

#include <memory>

//デバッグ用include
#include "SceneDebug.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	ChangeWindowMode(true);
	//画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	// 一部の関数はDxLib_Init()の前に実行する必要がある
	Setting::GetInstance().Load();

	//ウィンドウモードの設定
	ChangeWindowMode(Setting::GetInstance().GetIsFullScreen());

	//ウィンドウ名の設定
	SetMainWindowText(Game::kTitleText);
	
	// 画面のフルスクリーンアンチエイリアスモードの設定を行う( DxLib_Init の前でのみ使用可能 )
	SetFullSceneAntiAliasingMode(4, 4);


	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	//Zバッファを使用する
	SetUseZBuffer3D(true);
	//Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);
	//ポリゴンの裏面を描画しない
	SetUseBackCulling(true);

	//標準ライトの無効化
	//SetLightEnable(false);

	//エフェクシアを初期化
	//Effekseer_Init(800);
	//エフェクシアの歪み機能を初期化する
	//Effekseer_InitDistortion();
	// 画面モード変更時( とウインドウモード変更時 )にグラフィックスシステムの設定やグラフィックハンドルをリセットするかどうかを設定する( TRUE:リセットする( デフォルト )  FALSE:リセットしない )
	//SetChangeScreenModeGraphicsSystemResetFlag(false);
	//デバイスがロストした時のコールバックを設定
	//Effekseer_SetGraphicsDeviceLostCallbackFunctions();


	//サウンドのボリュームをここで変える
	SoundManager::GetInstance().ChangeBGMVolume(Setting::GetInstance().GetBGMVolume());
	SoundManager::GetInstance().ChangeSEVolume(Setting::GetInstance().GetSEVolume());

	//std::shared_ptr<SceneManager> pSceneManager = std::make_shared<SceneManager>();
	SceneManager pSceneManager;

#ifdef _DEBUG
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

		//Effekseer_Sync3DSetting();

		// ゲームの処理
		pSceneManager.Update();
		//UpdateEffekseer3D();
		pSceneManager.Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();

#ifdef _DEBUG

		// escキーでゲーム終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}

#endif

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	Font::GetInstance().Destroy();
	Setting::GetInstance().Destroy();
	SoundManager::GetInstance().Destroy();
	EffectManager::GetInstance().Destroy();


	//Effkseer_End();				// エフェクシア使用の終了処理
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
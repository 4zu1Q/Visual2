#pragma once
#include "DxLib.h"

// ゲーム全体で使用する定数の定義
namespace Game
{

	//ウィンドウモード設定
#ifdef _DEBUG 
	constexpr bool kWindowMode = false;
#else
	constexpr bool kWindowMode = false;
#endif

	//ウィンドウ名
	const char* const kTitleText = "ゲーム名";


	//ウィンドウサイズ
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;

	//constexpr int kScreenWidth = 1920;
	//constexpr int kScreenHeight = 1080;


	//ウィンドウカラーモード
	constexpr int kColorDepth = 32; //16 or 32

}

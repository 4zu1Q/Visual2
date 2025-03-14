#pragma once

namespace Game
{
	// ウインドウモード設定
#ifdef _DEBUG
	constexpr bool kDefaultWindowMode = true;
#else
	constexpr bool kDefaultWindowMode = false;
#endif
	// ウインドウ名
	const char* const kTitleText = "強化版Scene処理のサンプル";
	// ウインドウサイズ
	constexpr int kScreenWidth = 1280;
	constexpr int kScreenHeight = 720;
	// カラーモード
	constexpr int kColorDepth = 32;		// 32 or 16

	// フレームレート
	constexpr int kFps = 60;
	// 1フレームの時間(マイクロ秒)
	constexpr int kFrameMicroSec = 1000000 / kFps;
};
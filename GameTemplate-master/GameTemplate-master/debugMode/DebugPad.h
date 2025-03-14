#ifdef _DEBUG
#pragma once

// デバッグ機能表示中に使用するパッド処理	1コン&キーボードのみ対応
// (デバッグ機能表示中は通常のループを回らないため)
namespace DebugPad
{
	// パッドの入力状態取得
	void update();

	// 押し下げ判定
	bool isPress(int button);
	// トリガー判定
	bool isTrigger(int button);
};
#endif
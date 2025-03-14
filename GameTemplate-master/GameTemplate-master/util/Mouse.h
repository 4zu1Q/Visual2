#pragma once

#include "Vec2.h"

// マウスの入力状態を取得する
namespace Mouse
{
	// マウスログ情報のクリア
	void clear();
	// マウスの入力状態取得
	void update();

	// マウスログの数を取得
	int getLogNum();

	// 現在のマウス位置取得
	// 過去の位置情報を取得できる
	// logNo = 0で現在の位置情報	大きくすると過去フレームの情報
	// getLogNum()未満を指定する
	Vec2 getPos(int logNo = 0);

	// 押し下げ判定
	bool isPressLeft();
	bool isPressRight();
	// トリガー判定
	bool isTriggerLeft();
	bool isTriggerRight();
	// 離した判定
	bool isRelaseLeft();
	bool isRelaseRight();
}

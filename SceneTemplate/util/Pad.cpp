#include "Pad.h"
#include "DxLib.h"

namespace
{
	// 前のフレームのパッド押し下げ状態
	int lastPad = 0;
	// このフレームのパッド押し下げ状態
	int nowPad = 0;
}

namespace Pad
{
	void Update()
	{
		// 前のフレームに取得したパッド情報を一つ古い情報にする
		lastPad = nowPad;
		// 現在のパッドの情報を取得する
		nowPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	}

	bool IsPress(int key)
	{
		return (nowPad & key);
	}

	bool IsTrigger(int key)
	{
		if (nowPad)
		{
			int i = 0;
		}

		bool isLast = (lastPad & key);	// 前のフレーム
		bool isNow = (nowPad & key);	// このフレーム
		//return !isLast && isNow;		// これでいいけど理解しにくいので分かりやすく



		if (isNow &&		// このフレームに押されていて
			!isLast)	// 前回のフレームに押されていない
		{
			return true;
		}
		return false;
	}
	bool IsRelase(int key)
	{
		bool isNow = (nowPad & key);	// このフレーム
		bool isLast = (lastPad & key);	// 前のフレーム
		//return isLast && !isNow;

		if (!isNow &&	// このフレームには押されていなくて
			isLast)		// 前回のフレームは押されていた
		{
			return true;
		}

		return false;
	}
}

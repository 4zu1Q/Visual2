#ifdef _DEBUG
#include "DebugPad.h"
#include "DxLib.h"
#include <deque>

namespace
{
	// ログ記録フレーム数
	constexpr int kLogNum = 4;

	std::deque<int>	padLog;
}

namespace DebugPad
{
	void update()
	{
		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		padLog.push_front(pad);
		if (padLog.size() > kLogNum)
		{
			padLog.pop_back();
		}
	}

	bool isPress(int button)
	{
		return (padLog[0] & button);
	}

	bool isTrigger(int button)
	{
		if (padLog.size() <= 1)	return false;

		bool isNow = (padLog[0] & button);	// 現在の状態
		bool isLast = (padLog[1] & button);	// １フレーム前の状態
		return (isNow && !isLast);
	}

}
#endif
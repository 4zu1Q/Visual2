#include "Mouse.h"
#include "DxLib.h"
#include <deque>
#include <cassert>

namespace
{
	constexpr int kLogNum = 16;
	// 毎フレーム収集するマウス情報
	struct MouseLog
	{
		int input;	// ボタンのONOFF情報
		Vec2 pos;	// 座標
	};
	std::deque<MouseLog>	mouseLog;	// 軌跡を表示するため過去のを残す	0が最新
}

namespace Mouse
{
	void clear()
	{
		mouseLog.clear();
	}
	// マウスの入力状態取得
	void update()
	{
		// 現在のマウス情報を収集
		MouseLog tempLog;
		tempLog.input = GetMouseInput();

		int mouseX = 0;
		int mouseY = 0;
		if (GetMousePoint(&mouseX, &mouseY) == -1)
		{
			// ログが1フレーム分以上ある場合は前回と同じ位置に設定
			if (mouseLog.size() > 0)
			{
				tempLog.pos = mouseLog[0].pos;
			}
			else
			{
				tempLog.pos.x = 0.0f;
				tempLog.pos.y = 0.0f;
			}
		}
		else
		{
			tempLog.pos.x = static_cast<float>(mouseX);
			tempLog.pos.y = static_cast<float>(mouseY);
		}
		mouseLog.push_front(tempLog);
		if (mouseLog.size() > kLogNum)
		{
			mouseLog.pop_back();
		}
	}

	int getLogNum()
	{
		return static_cast<int>(mouseLog.size());
	}

	// 現在のマウス位置取得
	Vec2 getPos(int logNo)
	{
		assert(logNo >= 0);
		assert(logNo < kLogNum);
		if (mouseLog.size() > logNo)
		{
			return mouseLog[logNo].pos;
		}
		return Vec2(0, 0);
	}

	// GetMouseInput()の値を渡して各ボタンが押されているかを取得
	bool isLeft(int input) { return (input & MOUSE_INPUT_LEFT); }
	bool isRight(int input) { return (input & MOUSE_INPUT_RIGHT); }
	bool isMiddle(int input) { return (input & MOUSE_INPUT_MIDDLE); }
		
	// 押し下げ判定
	bool isPressLeft()
	{
		if (mouseLog.size() <= 0)	return false;
		return isLeft(mouseLog[0].input);
	}
	bool isPressRight()
	{
		if (mouseLog.size() <= 0)	return false;
		return isRight(mouseLog[0].input);
	}
	// トリガー判定
	bool isTriggerLeft()
	{
		if (mouseLog.size() <= 1)	return false;
		bool isNow = isLeft(mouseLog[0].input);	// 現在の状態
		bool isLast = isLeft(mouseLog[1].input);	// １フレーム前の状態
		return (isNow && !isLast);
	}
	bool isTriggerRight()
	{
		if (mouseLog.size() <= 1)	return false;
		bool isNow = isRight(mouseLog[0].input);	// 現在の状態
		bool isLast = isRight(mouseLog[1].input);	// １フレーム前の状態
		return (isNow && !isLast);
	}
	// 離した判定
	bool isRelaseLeft()
	{
		if (mouseLog.size() <= 1)	return false;
		bool isNow = isLeft(mouseLog[0].input);	// 現在の状態
		bool isLast = isLeft(mouseLog[1].input);	// １フレーム前の状態
		return (!isNow && isLast);
	}
	bool isRelaseRight()
	{
		if (mouseLog.size() <= 1)	return false;
		bool isNow = isRight(mouseLog[0].input);	// 現在の状態
		bool isLast = isRight(mouseLog[1].input);	// １フレーム前の状態
		return (!isNow && isLast);
	}
}
#include "DxLib.h"
#include "Pad.h"
#include <vector>

namespace
{
	// ログ記録フレーム数
	constexpr int kLogNum = 16;
	// パッド最大数
	constexpr int kMaxPad = 2;
	// 
	constexpr int kUsePadData[kMaxPad] =
	{
		DX_INPUT_KEY_PAD1,
		DX_INPUT_PAD2
	};

	// 入力ログ	0が最新の状態
	int padLog[kMaxPad][kLogNum];

	// キーログ
	bool				isRecordLog = false;
	int					playLogNo = -1;	// キーログ再生フレーム数
	std::vector<int>	keyLog;

	//コントローラーの入力情報
	XINPUT_STATE* XInputState;

	//トリガーボタンのデッドゾーン
	constexpr int kTriggerDeadZone = 60;

}

namespace Pad
{
	// パッドの入力状態取得
	void Update()
	{
		for (int padNo = 0; padNo < kMaxPad; padNo++)
		{
			// 現在のパッドの状態を取得
			int padState = GetJoypadInputState(kUsePadData[padNo]);
			if ((playLogNo >= 0) && (padNo == 0))
			{
				if (keyLog.size() > playLogNo)
				{
					padState = keyLog[playLogNo];
					playLogNo++;
				}
				else
				{
					playLogNo = -1;
				}
			}

			// ログの更新
			for (int i = kLogNum - 1; i >= 1; i--)
			{
				padLog[padNo][i] = padLog[padNo][i - 1];
			}
			// 最新の状態
			padLog[padNo][0] = padState;

			// キーログ
			if (isRecordLog)
			{
				if (padNo == 0)
				{
					keyLog.push_back(padState);
				}
			}
		}

		// DirectInput の入力を取得
		GetJoypadDirectInputState(DX_INPUT_PAD1, &m_dInputState);

		// トリガーの入力を更新
		m_rightTrigger = m_dInputState.Z / 1000.0f;
		m_leftTrigger = m_dInputState.Rz / 1000.0f;

	}

	// 押し下げ判定
	bool IsPress(int button, int padNo)
	{
		return (padLog[padNo][0] & button);
	}

	// トリガー判定
	bool IsTrigger(int button, int padNo)
	{
		bool isNow = (padLog[padNo][0] & button);	// 現在の状態
		bool isLast = (padLog[padNo][1] & button);	// １フレーム前の状態
		return (isNow && !isLast);
	}

	//離した判定
	bool IsRelase(int button, int padNo) {
		bool isNow = (padLog[padNo][0] & button);	//現在の状態
		bool isLast = (padLog[padNo][1] & button);	//1フレーム前の状態
		return (!isNow && isLast);
	}

	// 右トリガーの値を取得(失敗)
	float GetRightTrigger(int padNo)
	{
		return m_rightTrigger;
	}

	// 左トリガーの値を取得(失敗)
	float GetLeftTrigger(int padNo)
	{
		return m_leftTrigger;
	}
	//bool GetIsPushedTriggerButton(bool isRight)
	//{
	//	if (isRight)
	//	{
	//		//ZRの入力情報を返す
	//		if (XInputState->RightTrigger > kTriggerDeadZone)
	//		{
	//			return true;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	}
	//	else
	//	{
	//		//ZRの入力情報を返す
	//		if (XInputState->LeftTrigger > kTriggerDeadZone)
	//		{
	//			return true;
	//		}
	//		else
	//		{
	//			return false;
	//		}
	//	}
	//}
	void startRecordLog()
	{
		isRecordLog = true;
		keyLog.clear();
	}

	void endRecordLog()
	{
		isRecordLog = false;
	}
	void startPlayLog()
	{
		playLogNo = 0;
	}
	void endPlayLog()
	{
		playLogNo = -1;
	}
}
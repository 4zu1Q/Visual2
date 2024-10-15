#pragma once

namespace Pad
{

	// パッドの入力状態取得
	void Update();

	// 押し下げ判定
	bool IsPress(int button, int padNo = 0);
	// トリガー判定
	bool IsTrigger(int button, int padNo = 0);
	//離した判定
	bool IsRelase(int button, int padNo = 0);

	// ログ記録開始、終了
	void startRecordLog();
	void endRecordLog();

	// ログ再生開始、終了
	void startPlayLog();
	void endPlayLog();

}
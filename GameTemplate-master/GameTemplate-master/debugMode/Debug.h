#ifdef _DEBUG
#pragma once
#include "DxLib.h"

class Debug
{
public:

private:
	Debug() = default;
	virtual ~Debug() = default;

	static Debug* m_instance;

public:
	// コピーコンストラクタの禁止
	Debug(const Debug&) = delete;
	Debug& operator=(const Debug&) = delete;
	Debug(Debug&&) = delete;
	Debug& operator=(Debug&&) = delete;

	static Debug& getInstance()
	{
		if (!m_instance)
		{
			m_instance = new Debug;
		}
		return *m_instance;
	}
	static void destroy()
	{
		delete m_instance;
		m_instance = nullptr;
	}

public:
	void update();
	void draw();

	// ゲーム画面描画後の処理	スクショの撮影に使用する
	void postDraw();

	// デバッグ画面を開いているかどうか
	bool isActive() const { return m_isActive; }

	// ゲームのUpdateを行ってよいか
	// デバッグメニュー中は基本的に停止して、コマ送りはボタンで可能
	bool isGameUpdate() const;

	// スクリーンショットの撮影
	// していない場合ファイル名自動生成
	void screenShot(const char* fileName = nullptr);

	// 時間計測
	void onStartUpdate();	// update開始時に呼ぶ
	void onEndUpdate();	// update終了時に呼ぶ
	void onStartDraw();	// draw開始時に呼ぶ
	void onEndDraw();		// draw終了時に呼ぶ

private:
	// 各パーツ表示
	void drawBg() const;
	void drawFps() const;
	void drawProcessBar() const;	// 処理バーの表示
private:

	// デバッグ表示設定
	enum class DispType
	{
		Always,			// 常に表示
		DebugActive,	// デバッグメニューを開いている間表示
		None,			// 表示しない
	};

	// デバッグメニュー表示設定
	struct DebugSetting
	{
		int bgAlpha = 7;	// デバッグメニューの背景透明度	0~10
		DispType dispFps = DispType::DebugActive;	// FPS表示
		DispType dispProcessBar = DispType::Always;	// 処理バー表示
		bool isOpenTrg = false;		// トリガーで開閉	押している間開く	

		// デバッグ表示のクリア
		
		// デバッグ設定の保存
	};

private:
	// デバッグ機能開いている
	bool m_isActive = false;
	// スクリーンショット撮影
	bool m_isSaveScreenShot = false;	// このフレームのスクリーンショットを撮影

	// 処理バー
	LONGLONG	m_updateTime = 0;	// updateにかかった時間
	LONGLONG	m_drawTime = 0;		// drawにかかった時間

	// デバッグ設定
	DebugSetting m_setting;
};

#endif


#ifdef _DEBUG
#include "Debug.h"
#include "game.h"

#include "DebugPad.h"	// SceneのUpdateでパッド処理を行っているので"Pad.h"は使用不可


#include <string>

Debug* Debug::m_instance = nullptr;

namespace
{
	const char* const kTitle = "デバッグメニュー";

	constexpr int kScreenShotInterval = 60;
}

void Debug::update()
{
	// パッド状態の更新
	DebugPad::update();

	if (m_setting.isOpenTrg)
	{
		if (DebugPad::isTrigger(PAD_INPUT_5))
		{
			m_isActive = !m_isActive;
		}
	}
	else
	{
		m_isActive = DebugPad::isPress(PAD_INPUT_5);
	}
	

	// 非アクティブ時にも実行する処理


	// 非アクティブ時にも実行する処理 ここまで

	if (!m_isActive)	return;

	// アクティブ字のみ有効な処理
	m_isSaveScreenShot = false;
	if (DebugPad::isTrigger(PAD_INPUT_6))
	{
		// ここではスクショを取らず、ゲーム画面描画後に撮影する
		m_isSaveScreenShot = true;
	}
}

void Debug::draw()
{
	drawBg();
	drawFps();
	drawProcessBar();

	if (!m_isActive)	return;

	DrawString(32, 16 + 16, "R:スクリーンショット撮影", 0xffffff);
	DrawString(32, 32 + 16, "→:コマ送り", 0xffffff);

	DrawString(32, 48 + 16, "FPS表示:　  常に  [デバッグメニューを開いている間]  表示しない", 0xffffff);
	DrawString(32, 64 + 16, "処理バー:　[常に]  デバッグメニューを開いている間   表示しない", 0xffffff);
	DrawString(32, 80 + 16, "デバッグメニュー開閉:　[Lボタンを押している間開く]  Lボタンで開閉", 0xffffff);
//	DrawString(32, 96 + 16, "背景の濃さ:　[Lボタンを押している間開く]  Lボタンで開閉", 0xffffff);
	DrawFormatString(32, 96 + 16, 0xffffff, "背景の濃さ:　%3d%%", m_setting.bgAlpha*10);

	DrawString(32, 112 + 16, "デバッグ設定の保存", 0xffffff);
}

void Debug::postDraw()
{
	if (m_isSaveScreenShot)
	{
		screenShot();
	}
}

bool Debug::isGameUpdate() const
{
	if (!m_isActive)	return true;
	if( DebugPad::isTrigger(PAD_INPUT_RIGHT) )	return true;

	return false;
}

void Debug::screenShot(const char* fileName)
{
	std::string name = "screenShot/";
	if (fileName)
	{
		name += fileName;
	}
	else
	{
		// ファイル名自動生成
		DATEDATA buf;
		GetDateTime(&buf);

		name += std::to_string(buf.Year);
		if (buf.Mon < 10)	name += "0";
		name += std::to_string(buf.Mon);
		if (buf.Day < 10)	name += "0";
		name += std::to_string(buf.Day);
		if (buf.Hour < 10)	name += "0";
		name += std::to_string(buf.Hour);
		if (buf.Min < 10)	name += "0";
		name += std::to_string(buf.Min);
		if (buf.Sec < 10)	name += "0";
		name += std::to_string(buf.Sec);
		name += ".bmp";
	}

	SaveDrawScreen(0, 0, Game::kScreenWidth, Game::kScreenHeight, name.c_str());
	printfDx("スクリーンショットを保存しました! %s\n", name.c_str());
}

void Debug::onStartUpdate()
{
	m_updateTime = GetNowHiPerformanceCount();
}

void Debug::onEndUpdate()
{
	m_updateTime = GetNowHiPerformanceCount() - m_updateTime;
}

void Debug::onStartDraw()
{
	m_drawTime = GetNowHiPerformanceCount();
}

void Debug::onEndDraw()
{
	m_drawTime = GetNowHiPerformanceCount() - m_drawTime;
}

void Debug::drawBg() const
{
	if (!m_isActive)	return;

	float rate = static_cast<float>(m_setting.bgAlpha) / 10;
	int alpha = static_cast<int>(rate * 255);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	int width = GetDrawStringWidth(kTitle,static_cast<int>(strlen(kTitle)));
	DrawString(Game::kScreenWidth / 2 - width /2, 8, kTitle, 0xffffff);
}

void Debug::drawFps() const
{
	if (m_setting.dispFps == DispType::None)	return;
	if (m_setting.dispFps == DispType::DebugActive && !m_isActive)	return;

	int width = GetDrawFormatStringWidth("FPS:%.4f", GetFPS());
	DrawFormatString(Game::kScreenWidth - width - 32, 32, 0xffffff, "FPS:%.4f", GetFPS());
}

void Debug::drawProcessBar() const
{
	if (m_setting.dispProcessBar == DispType::None)	return;
	if (m_setting.dispProcessBar == DispType::DebugActive && !m_isActive)	return;

	float rate = static_cast<float>(m_updateTime + m_drawTime) / Game::kFrameMicroSec;
	int width = static_cast<int>(Game::kScreenWidth * rate);
	// Update+Draw
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0xff0000, true);

	rate = static_cast<float>(m_updateTime) / Game::kFrameMicroSec;
	// Update
	DrawBox(0, Game::kScreenHeight - 16, width, Game::kScreenHeight, 0x0000ff, true);
}


#endif // _DEBUG

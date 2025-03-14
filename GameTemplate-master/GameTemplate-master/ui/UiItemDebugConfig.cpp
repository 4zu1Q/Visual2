#include "UiItemDebugConfig.h"
#include "DxLib.h"
#include <string>

#include "ConfigData.h"
#include "Pad.h"

// ==============================================================================================

void UiItemDebugConfigBase::drawItemText(std::string text)
{
	unsigned int color = 0xffffff;
	if (m_isSelect)
	{
		color = 0xffff00;
	}
	DrawString(getDispX(), getDispY(), text.c_str(), color);
}

// ==============================================================================================

void UiItemDebugConfig_BgmVol::update()
{
	if (!m_isSelect)	return;
	if (Pad::isTrigger(PAD_INPUT_LEFT))
	{
		ConfigData::getInstance().decBgmVol(true);
	}
	if (Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		ConfigData::getInstance().incBgmVol(true);
	}
}

void UiItemDebugConfig_BgmVol::draw()
{
	std::string text = "BGM音量 ";
	text += std::to_string(ConfigData::getInstance().getBgmVol());
	text += " / ";
	text += std::to_string(ConfigData::getInstance().getMaxBgmVol());
	drawItemText(text);
}

// ==============================================================================================

void UiItemDebugConfig_SeVol::update()
{
	if (!m_isSelect)	return;
	if (Pad::isTrigger(PAD_INPUT_LEFT))
	{
		ConfigData::getInstance().decSeVol(true);
	}
	if (Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		ConfigData::getInstance().incSeVol(true);
	}
}

void UiItemDebugConfig_SeVol::draw()
{
	std::string text = "SE音量  ";
	text += std::to_string(ConfigData::getInstance().getSeVol());
	text += " / ";
	text += std::to_string(ConfigData::getInstance().getMaxSeVol());
	drawItemText(text);
}

// ==============================================================================================

void UiItemDebugConfig_WindowMode::update()
{
	if (!m_isSelect)	return;
	if (Pad::isTrigger(PAD_INPUT_LEFT))
	{
		ConfigData::getInstance().setWindowMode(true);
	}
	if (Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		ConfigData::getInstance().setWindowMode(false);
	}
}

void UiItemDebugConfig_WindowMode::draw()
{
	std::string text;
	if (ConfigData::getInstance().isWindowMode())
	{
		text = "ウインドウモード  ";
	}
	else
	{
		text = "フルスクリーン    ";
	}
	if (isDifferMode())
	{
		text += "A:変更する";
	}
	drawItemText(text);
}

void UiItemDebugConfig_WindowMode::onDecide()
{
	// ウインドウモードの変更を行う
	if (isDifferMode())
	{
		ChangeWindowMode(ConfigData::getInstance().isWindowMode());
	}
}

void UiItemDebugConfig_WindowMode::onUnselect()
{
	m_isSelect = false;
	// 設定が違う状態で別の選択に移動したときはコンフィグの設定を現在の状態に合わせる
	if (isDifferMode())
	{
		ConfigData::getInstance().setWindowMode(GetWindowModeFlag());
	}
}

bool UiItemDebugConfig_WindowMode::isDifferMode()
{
	bool configFlag = ConfigData::getInstance().isWindowMode();
	bool isWindowMode = GetWindowModeFlag();
	return (configFlag != isWindowMode);
}

// ==============================================================================================

void UiItemDebugConfig_Vibration::update()
{
	if (!m_isSelect)	return;
	if (Pad::isTrigger(PAD_INPUT_LEFT))
	{
		ConfigData::getInstance().setVibration(true);
	}
	if (Pad::isTrigger(PAD_INPUT_RIGHT))
	{
		ConfigData::getInstance().setVibration(false);
	}
}

void UiItemDebugConfig_Vibration::draw()
{
	std::string text = "振動           ";
	if (ConfigData::getInstance().isVibration())
	{
		text += "ON";
	}
	else
	{
		text += "OFF";
	}
	drawItemText(text);
}

// ==============================================================================================

void UiItemDebugConfig_Save::update()
{
	if (!m_isSelect)	return;
}

void UiItemDebugConfig_Save::draw()
{
	drawItemText("コンフィグデータを保存する");
}

void UiItemDebugConfig_Save::onDecide()
{
	ConfigData::getInstance().write();
	printfDx("コンフィグ設定を保存しました\n");
}


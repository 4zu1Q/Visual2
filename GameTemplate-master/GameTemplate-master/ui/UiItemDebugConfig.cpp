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
	std::string text = "BGM���� ";
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
	std::string text = "SE����  ";
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
		text = "�E�C���h�E���[�h  ";
	}
	else
	{
		text = "�t���X�N���[��    ";
	}
	if (isDifferMode())
	{
		text += "A:�ύX����";
	}
	drawItemText(text);
}

void UiItemDebugConfig_WindowMode::onDecide()
{
	// �E�C���h�E���[�h�̕ύX���s��
	if (isDifferMode())
	{
		ChangeWindowMode(ConfigData::getInstance().isWindowMode());
	}
}

void UiItemDebugConfig_WindowMode::onUnselect()
{
	m_isSelect = false;
	// �ݒ肪�Ⴄ��Ԃŕʂ̑I���Ɉړ������Ƃ��̓R���t�B�O�̐ݒ�����݂̏�Ԃɍ��킹��
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
	std::string text = "�U��           ";
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
	drawItemText("�R���t�B�O�f�[�^��ۑ�����");
}

void UiItemDebugConfig_Save::onDecide()
{
	ConfigData::getInstance().write();
	printfDx("�R���t�B�O�ݒ��ۑ����܂���\n");
}


#include "UiItemSimpleText.h"
#include "DxLib.h"

namespace
{
	constexpr unsigned int kDefaultSelectColor = 0xffff00;
	constexpr unsigned int kDefaultUnselectColor = 0xffffff;
}

UiItemSimpleText::UiItemSimpleText():
	m_selectColor(kDefaultSelectColor),
	m_unselectColor(kDefaultUnselectColor)
{
}

UiItemSimpleText::~UiItemSimpleText()
{
}

void UiItemSimpleText::setup(std::string text, int x, int y)
{
	m_text = text;
	setPos(x, y);
}

void UiItemSimpleText::setText(std::string text)
{
	m_text = text;
}

void UiItemSimpleText::init()
{
}

void UiItemSimpleText::update()
{
}

void UiItemSimpleText::draw()
{
	unsigned int color = m_unselectColor;

	if (m_isSelect)
	{
		color = m_selectColor;
	}
	DrawString(getDispX(), getDispY(), m_text.c_str(), color);
}

void UiItemSimpleText::onDecide()
{
}

void UiItemSimpleText::onSelect()
{
	m_isSelect = true;
}

void UiItemSimpleText::onUnselect()
{
	m_isSelect = false;
}

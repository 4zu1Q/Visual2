#include "UiItemSimpleBox.h"
#include "DxLib.h"

namespace
{
	// 非選択から選択状態のカラーに代わるのにかかるカウント数
	constexpr int kColorChangeCount = 64;
	constexpr int kColorOnCountSpeed = 16;
	constexpr int kColorOffCountSpeed = 4;

	// 非選択状態の各カラー成分
	constexpr int kInactiveColorElement = 32;
}

UiItemSimpleBox::UiItemSimpleBox():
	m_fontHandle(-1),
	m_strOffsetY(0),
	m_selectFrame(0)
{
}

UiItemSimpleBox::~UiItemSimpleBox()
{
}

void UiItemSimpleBox::setup(std::string text, int x, int y, int w, int h)
{
	m_text = text;
	setRect(x, y, w, h);
}

void UiItemSimpleBox::init()
{
}

void UiItemSimpleBox::update()
{
	if (m_isSelect)
	{
		m_selectFrame += kColorOnCountSpeed;
		if (m_selectFrame > kColorChangeCount)	m_selectFrame = kColorChangeCount;
	}
	else
	{
		m_selectFrame -= kColorOffCountSpeed;
		if (m_selectFrame < 0)	m_selectFrame = 0;
	}
}

void UiItemSimpleBox::draw()
{
	float rate = static_cast<float>(m_selectFrame) / kColorChangeCount;
	int elem = kInactiveColorElement + static_cast<int>((255- kInactiveColorElement) * rate);
	unsigned int color = GetColor(elem, elem, elem);

	// スクロールを反映させる
	int x = getDispX();
	int y = getDispY();

//	if (m_isSelect)
//	{
//		DrawRoundRect(x+2, y + 2, x + m_w-2, y + m_h - 2, 8, 8, 0xf0f000, true);
//	}

	DrawRoundRect(x, y, static_cast<int>(x + m_w), static_cast<int>(y + m_h), 8, 8, color, false);

	int centerX = static_cast<int>(x + m_w / 2);
	int centerY = static_cast<int>(y + m_h / 2);
	int textLen = static_cast<int>(m_text.length());
	if(m_fontHandle == -1)
	{
		int sizeX;
		int sizeY;
		GetDrawStringSize(&sizeX, &sizeY, nullptr, m_text.c_str(), textLen);
		DrawString(centerX - sizeX / 2, centerY - sizeY / 2, m_text.c_str(), color);
	}
	else
	{
		int sizeX;
		int sizeY;
		GetDrawStringSizeToHandle(&sizeX, &sizeY, nullptr, m_text.c_str(), textLen, m_fontHandle);
		DrawStringToHandle(centerX - sizeX / 2, centerY - sizeY / 2+ m_strOffsetY, m_text.c_str(), color, m_fontHandle);

	//	DrawExtendStringToHandle(centerX - sizeX / 2*0.5, 
	//		centerY - sizeY / 2 * 0.5 + m_strOffsetY,
	//		0.5, 0.5, 
	//		m_text.c_str(), color, m_fontHandle);
	}
}

#if false
void UiItemSimpleBox::onDecide()
{
}

void UiItemSimpleBox::onSelect()
{
	m_isSelect = true;
}

void UiItemSimpleBox::onUnselect()
{
	m_isSelect = false;
}
#endif
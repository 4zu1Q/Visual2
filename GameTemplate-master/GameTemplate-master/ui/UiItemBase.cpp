#include "UiItemBase.h"

UiItemBase::UiItemBase():
	m_isSelect(false),
	m_x(0.0f),
	m_y(0.0f),
	m_w(0.0f),
	m_h(0.0f),
	m_scrollX(0.0f),
	m_scrollY(0.0f)
{
}

UiItemBase::~UiItemBase()
{
}

void UiItemBase::setLayout(int key, std::shared_ptr<UiItemBase> pNext, bool isRepeat)
{
	m_layoutTrg[key] = pNext;
	if (isRepeat)
	{
		m_layoutRep[key] = pNext;
	}
}

std::shared_ptr<UiItemBase> UiItemBase::getTrgNext(int key)
{
	if (m_layoutTrg.count(key) <= 0)	return nullptr;	// ˆÚ“®‚µ‚È‚¢
	return m_layoutTrg[key];
}

std::shared_ptr<UiItemBase> UiItemBase::getRepNext(int key)
{
	if (m_layoutRep.count(key) <= 0)	return nullptr;	// ˆÚ“®‚µ‚È‚¢
	return m_layoutRep[key];
}

void UiItemBase::setPos(int x, int y)
{
	m_x = static_cast<float>(x);
	m_y = static_cast<float>(y);
}

void UiItemBase::setRect(int x, int y, int w, int h)
{
	m_x = static_cast<float>(x);
	m_y = static_cast<float>(y);
	m_w = static_cast<float>(w);
	m_h = static_cast<float>(h);
}

void UiItemBase::setScroll(float x, float y)
{
	m_scrollX = x;
	m_scrollY = y;
}

int UiItemBase::getDispX() const
{
	return static_cast<int>(m_x - m_scrollX);
}

int UiItemBase::getDispY() const
{
	return static_cast<int>(m_y - m_scrollY);
}

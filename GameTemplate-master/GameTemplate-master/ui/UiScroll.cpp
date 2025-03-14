#include "UiScroll.h"
#include "UiItemBase.h"

#include "game.h"

namespace
{
//	constexpr int kDefaultDispRangeMinX = 64;
//	constexpr int kDefaultDispRangeMinY = 64;
//	constexpr int kDefaultDispRangeMaxX = 512;
//	constexpr int kDefaultDispRangeMaxY = 512;
	constexpr int kDefaultDispRangeMinX = 64;
	constexpr int kDefaultDispRangeMinY = 64;
	constexpr int kDefaultDispRangeMaxX = Game::kScreenWidth-64;
	constexpr int kDefaultDispRangeMaxY = Game::kScreenHeight-64;
}

UiScroll::UiScroll():
	m_scrollX(0.0f),
	m_scrollY(0.0f)
{
}

UiScroll::~UiScroll()
{
}

void UiScroll::update()
{
	UiBase::update();

	// 選択中のItemが範囲内に収まるようにItemをすべてスクロールさせる
	if (m_pSelectItem)
	{
		float scrollX = 0.0f;
		float scrollY = 0.0f;

		float top = m_pSelectItem->getY();
		float bottom = top + m_pSelectItem->getH();
		scrollY = m_scrollY;
		if ((top - m_scrollY) < kDefaultDispRangeMinY)
		{
			scrollY = static_cast<float>(top - kDefaultDispRangeMinY);
		}
		if ((bottom - m_scrollY) > kDefaultDispRangeMaxY)
		{
			scrollY = static_cast<float>(bottom-kDefaultDispRangeMaxY);
		}

		float left = m_pSelectItem->getX();
		float right = left + m_pSelectItem->getW();
		scrollX = m_scrollX;
		if ((left - m_scrollX) < kDefaultDispRangeMinX)
		{
			scrollX = static_cast<float>(left - kDefaultDispRangeMinX);
		}
		if ((right - m_scrollX) > kDefaultDispRangeMaxX)
		{
			scrollX = static_cast<float>(right - kDefaultDispRangeMaxX);
		}

		if (abs(m_scrollX - scrollX) > 1.0f)
		{
			m_scrollX = m_scrollX / 2 + scrollX / 2;
		}
		else
		{
			m_scrollX = scrollX;
		}
		if (abs(m_scrollY - scrollY) > 1.0f)
		{
			m_scrollY = m_scrollY / 2 + scrollY / 2;
		}
		else
		{
			m_scrollY = scrollY;
		}
		for (auto& item : m_pItem)
		{
			item->setScroll(m_scrollX, m_scrollY);
		}
	}
}

void UiScroll::draw()
{
	UiBase::draw();

#if false	// UI表示範囲のデバッグ表示
	DrawBox(kDefaultDispRangeMinX, kDefaultDispRangeMinY, kDefaultDispRangeMaxX, kDefaultDispRangeMaxY,0xffffff, false);
#endif
}

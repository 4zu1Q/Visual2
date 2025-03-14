#include "UiBase.h"
#include "DxLib.h"
#include "UiItemBase.h"

#include "Pad.h"

namespace
{
	// キーリーピートフレーム
	constexpr int kFirstRepeatFrame = 30;
	constexpr int kRepeatFrame = 4;
}

UiBase::UiBase():
	m_firstRepeatFrame(kFirstRepeatFrame),
	m_repeatFrame(kRepeatFrame)
{
	m_repeatCount[PAD_INPUT_DOWN] = m_firstRepeatFrame;
	m_repeatCount[PAD_INPUT_LEFT] = m_firstRepeatFrame;
	m_repeatCount[PAD_INPUT_RIGHT] = m_firstRepeatFrame;
	m_repeatCount[PAD_INPUT_UP] = m_firstRepeatFrame;
}

UiBase::~UiBase()
{
}

void UiBase::init()
{
	for (auto& item : m_pItem)
	{
		item->init();
	}
}

void UiBase::update()
{
	for (auto& item : m_pItem)
	{
		item->update();
	}
	std::shared_ptr<UiItemBase> pNext;
	if (m_pSelectItem)
	{
		// すでにいずれかの項目を選択している
		for (auto& item : m_repeatCount)
		{
			int key = item.first;
			if (Pad::isTrigger(key))
			{
				pNext = m_pSelectItem->getTrgNext(key);
				item.second = m_firstRepeatFrame;
			}
			else if (Pad::isPress(key))
			{
				item.second--;
				if (item.second <= 0)
				{
					pNext = m_pSelectItem->getRepNext(key);
					item.second = m_repeatFrame;
				}
			}
		}
	}
	else
	{
		// どの項目も選択されていない
		if (Pad::isTrigger(PAD_INPUT_UP) || Pad::isTrigger(PAD_INPUT_LEFT))
		{
			pNext = m_pItem.back();
		}
		else if (Pad::isTrigger(PAD_INPUT_DOWN) || Pad::isTrigger(PAD_INPUT_RIGHT))
		{
			pNext = m_pItem.front();
		}
	}
	//	if (m_pSelectItem != pNext)
	if (pNext)
	{
		if (m_pSelectItem)	m_pSelectItem->onUnselect();
		if (pNext)	pNext->onSelect();
		m_pSelectItem = pNext;

		onChangeSelect();
	}

	if (Pad::isTrigger(PAD_INPUT_1))
	{
		if (m_pSelectItem)
		{
			m_pSelectItem->onDecide();
		}
	}
}

void UiBase::draw()
{
	for (auto& item : m_pItem)
	{
		item->draw();
	}
}

void UiBase::addItem(std::shared_ptr<UiItemBase> pItem)
{
	m_pItem.push_back(pItem);
}

void UiBase::setupButtonSettingUD(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep)
{
	setupButtonSetting(pItem, isLoopTrg, isLoopRep, PAD_INPUT_DOWN, PAD_INPUT_UP);
}

void UiBase::setupButtonSettingUD(bool isLoopTrg, bool isLoopRep)
{
	setupButtonSetting(isLoopTrg, isLoopRep, PAD_INPUT_DOWN, PAD_INPUT_UP);
}

void UiBase::setupButtonSettingLR(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep)
{
	setupButtonSetting(pItem, isLoopTrg, isLoopRep, PAD_INPUT_RIGHT, PAD_INPUT_LEFT);
}

void UiBase::setupButtonSettingLR(bool isLoopTrg, bool isLoopRep)
{
	setupButtonSetting(isLoopTrg, isLoopRep, PAD_INPUT_RIGHT, PAD_INPUT_LEFT);
}

void UiBase::onChangeSelect()
{
}

void UiBase::setupButtonSetting(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton)
{
	int num = static_cast<int>(pItem.size());

	for (int i = 0; i < num - 1; i++)
	{
		pItem[i]->setLayout(nextButton, pItem[i + 1], true);
	}
	if (isLoopTrg)
	{
		pItem.back()->setLayout(nextButton, pItem.front(), isLoopRep);	// リピートの場合ループなし
	}

	if (isLoopTrg)
	{
		pItem.front()->setLayout(prevButton, pItem.back(), isLoopRep);	// リピートの場合ループなし
	}
	for (int i = 1; i < num; i++)
	{
		pItem[i]->setLayout(prevButton, pItem[i - 1], true);
	}
}

void UiBase::setupButtonSetting(bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton)
{
	setupButtonSetting(m_pItem, isLoopTrg, isLoopRep, nextButton, prevButton);
}

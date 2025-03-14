#include "SceneUiTest.h"
#include "DxLib.h"
#include "UiScroll.h"

#include "SceneDebug.h"
#include "Pad.h"

#include "FontManager.h"
#include "UiItemSimpleBox.h"
#include <vector>

namespace
{
	const char* const kUseFontName = "TEST_24";

//	constexpr int kItemNumV = 8;	// 縦個数
//	constexpr int kItemNumH = 12;	// 横個数
	constexpr int kItemNumV = 16;	// 縦個数
	constexpr int kItemNumH = 24;	// 横個数

	constexpr int kItemIntervalX = 64;
	constexpr int kItemIntervalY = 64;

	constexpr int kItemWidth = kItemIntervalX - 2;
	constexpr int kItemHeight = kItemIntervalY - 2;

	constexpr int kFirstRepeatFrame = 16;
	constexpr int kRepeatFrame = 2;

}

SceneUiTest::SceneUiTest():
	m_frameCount(0)
{
	// シーン設定
	setDispLoading(false);	// ロード中表示を行うか
}

void SceneUiTest::startLoad()
{
	// 非同期読み込みを開始する
	SetUseASyncLoadFlag(true);

	// デフォルトに戻す
	SetUseASyncLoadFlag(false);
}

bool SceneUiTest::isLoaded() const
{
	// １個１個チェックしなくてもこっちでよかった
//	if (GetASyncLoadNum() > 0)	return false;

	return true;
}

void SceneUiTest::init()
{
	m_pTestMenu = std::make_shared<UiScroll>();

#if false
	{
		std::shared_ptr<UiItemSimpleBox> pItem = std::make_shared<UiItemSimpleBox>();
		pItem->setup("0", 32, 32, 32, 32);
		m_pTestMenu->addItem(pItem);
	}
	{
		std::shared_ptr<UiItemSimpleBox> pItem = std::make_shared<UiItemSimpleBox>();
		pItem->setup("1", 32, 32+32, 32, 32);
		m_pTestMenu->addItem(pItem);
	}
	m_pTestMenu->setupButtonSettingUD(true, false);
#else

	int useFont = FontManager::getInstance().getHandle(kUseFontName);
//	int useFont = -1;

	// レイアウト作成用に持っておく
	std::vector < std::shared_ptr<UiItemBase> >	pUiItems;
	for (int i = 0; i < kItemNumV * kItemNumH; i++)
	{
		int x = 64 + (i % kItemNumH) * kItemIntervalX;
		int y = 64 + (i / kItemNumH) * kItemIntervalY;

		std::shared_ptr<UiItemSimpleBox> pItem = std::make_shared<UiItemSimpleBox>();
		pItem->setup(std::to_string(i), x, y, kItemWidth, kItemHeight);
		pItem->setFontHandle(useFont);
		pItem->setStrOffsetY(4);
		m_pTestMenu->addItem(pItem);

		pUiItems.push_back(pItem);
	}
	for (int y = 0; y < kItemNumV; y++)
	{
		for (int x = 0; x < kItemNumH-1; x++)
		{
			int cur = x + kItemNumH * y;
			int nxt = cur+1;
			pUiItems[cur]->setLayout(PAD_INPUT_RIGHT, pUiItems[nxt], true);
		}
		{
			int cur = kItemNumH-1 + kItemNumH * y;
			int nxt = kItemNumH * y;
			pUiItems[cur]->setLayout(PAD_INPUT_RIGHT, pUiItems[nxt], false);
		}

		for (int x = 1; x < kItemNumH; x++)
		{
			int cur = x + kItemNumH * y;
			int nxt = cur - 1;
			pUiItems[cur]->setLayout(PAD_INPUT_LEFT, pUiItems[nxt], true);
		}
		{
			int cur = 0 + kItemNumH * y;
			int nxt = kItemNumH-1 + kItemNumH * y;
			pUiItems[cur]->setLayout(PAD_INPUT_LEFT, pUiItems[nxt], false);
		}
	}
	for (int x = 0; x < kItemNumH; x++)
	{
		for (int y = 0; y < kItemNumV-1; y++)
		{
			int cur = x + kItemNumH * y;
			int nxt = cur + kItemNumH;
			pUiItems[cur]->setLayout(PAD_INPUT_DOWN, pUiItems[nxt], true);
		}
		{
			int cur = x + kItemNumH * (kItemNumV-1);
			int nxt = x + kItemNumH * 0;
			pUiItems[cur]->setLayout(PAD_INPUT_DOWN, pUiItems[nxt], false);
		}
		for (int y = 1; y < kItemNumV; y++)
		{
			int cur = x + kItemNumH * y;
			int nxt = cur - kItemNumH;
			pUiItems[cur]->setLayout(PAD_INPUT_UP, pUiItems[nxt], true);
		}
		{
			int cur = x + kItemNumH * 0;
			int nxt = x + kItemNumH * (kItemNumV-1);
			pUiItems[cur]->setLayout(PAD_INPUT_UP, pUiItems[nxt], false);
		}
	}


#endif
	m_pTestMenu->init();
	// 項目数が多いのでカーソルの移動を早くする
	m_pTestMenu->setFirstRepeatFrame(kFirstRepeatFrame);
	m_pTestMenu->setRepeatFrame(kRepeatFrame);
}

void SceneUiTest::end()
{
}

void SceneUiTest::update()
{
	if (getFadeProgress() > 0.0f)
	{
		return;
	}
	m_frameCount++;
	m_pTestMenu->update();

	if (Pad::isTrigger(PAD_INPUT_2))
	{
		// 一度呼んだらもう一度呼ばないように！
		// 呼んだらフェードアウトが始まるのでこのソースではisFadingではじいている
		setNextScene(new SceneDebug);
	}
}

void SceneUiTest::draw()
{
	if (!isInitialized())	return;

	m_pTestMenu->draw();

	DrawString(640, 8, "SceneUiTest 上下左右方向に選択項目を変更することのできるUIのサンプル", 0xffffff);
	DrawFormatString(640, 8 + 16, 0xffffff, "FRAME:%d", m_frameCount);
}

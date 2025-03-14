#include "UiDebugMenu.h"

#include "UiItemSimpleText.h"
#include "UiItemDebugConfig.h"
#include "UiItemDebugSceneChange.h"

#include "SceneBase.h"

#include "StrUtil.h"

#include <string>
#include <fstream>
#include <sstream>
#include <cassert>

namespace
{
	const char* const kMenuDataFilename = "debugData/csv/debugMenu.csv";

	// printfDx表示をクリアする
	class UiItemClearPrint : public UiItemSimpleText
	{
	public:
		UiItemClearPrint() {}
		virtual ~UiItemClearPrint() {}
		void onDecide() override
		{
			clsDx();
		}
	private:
	};
}

UiDebugMenu::UiDebugMenu()
{
}

UiDebugMenu::~UiDebugMenu()
{
}

void UiDebugMenu::createItems(SceneBase* pCurrentScene)
{
	// ファイル情報の読み込み
	std::ifstream ifs(kMenuDataFilename);
	std::string line;

	getline(ifs, line);	// 1行目は読み捨て

	std::vector<std::shared_ptr<UiItemBase>>	pScrollItemTbl;	// 選択対象となるアイテム
	while (getline(ifs, line))
	{
		// csvデータ1行を','で複数の文字列に分割
		std::vector<std::string>	strvec = StrUtil::split(line, ',');
		// strvec[0]	: ItemID
		// strvec[1]	: 選択可能か	0の場合は選択されず飛ばす
		// strvec[2]	: シーン遷移の場合、遷移先ID
		// strvec[3]	: メモ(この項目の内容は使用しない)

		std::shared_ptr<UiItemBase> pItem = addMenuItem(strvec[0], strvec[2], pCurrentScene);
		if (strvec[1] != "0")
		{
			pScrollItemTbl.push_back(pItem);
		}

	}
	setupButtonSettingUD(pScrollItemTbl, true, false);
}

void UiDebugMenu::draw()
{
	for (auto& item : m_pItem)
	{
		item->draw();
	}
	if (m_pSelectItem)
	{
		int x = static_cast<int>(m_pSelectItem->getX());
		int y = static_cast<int>(m_pSelectItem->getY());
		x -= 17;
		DrawString(x, y, "→", 0xffff00);
	}
}

std::shared_ptr<UiItemBase> UiDebugMenu::addMenuItem(std::string itemId, std::string sceneId, SceneBase* pCurrentScene)
{
	// itemIdに応じた種類のアイテムを追加する
	//	ITEM_SCENECHANGE			: シーン遷移
	//	ITEM_BORDER					: 区切り線	これが選択される事はない(飛ばされる)
	//	ITEM_CONFIG_BGMVOL			: コンフィグ設定変更 BGM音量
	//	ITEM_CONFIG_SEVOL			: コンフィグ設定変更 SE音量
	//	ITEM_CONFIG_WIDOWMODE		: コンフィグ設定変更 ウインドウモード
	//	ITEM_CONFIG_VIBRATION		: コンフィグ設定変更 振動
	//	ITEM_CONFIG_SAVE			: コンフィグ設定変更した内容を保存
	//	ITEM_CLEARPRINT				: printfDxの表示をクリア

	int index = static_cast<int>(m_pItem.size());
	int x = 64;
	int y = 32 + index * 16;

	if (itemId == "ITEM_SCENECHANGE")
	{
		std::shared_ptr<UiItemDebugSceneChange> pItem = std::make_shared<UiItemDebugSceneChange>();
		pItem->setup(sceneId, x, y);
		pItem->setScene(pCurrentScene, sceneId);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_BORDER")
	{
		std::shared_ptr<UiItemSimpleText> pItem = std::make_shared<UiItemSimpleText>();
		pItem->setup("---------------", x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CONFIG_BGMVOL")
	{
		std::shared_ptr<UiItemDebugConfig_BgmVol> pItem = std::make_shared<UiItemDebugConfig_BgmVol>();
		pItem->setPos(x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CONFIG_SEVOL")
	{
		std::shared_ptr<UiItemDebugConfig_SeVol> pItem = std::make_shared<UiItemDebugConfig_SeVol>();
		pItem->setPos(x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CONFIG_WINDOWMODE")
	{
		std::shared_ptr<UiItemDebugConfig_WindowMode> pItem = std::make_shared<UiItemDebugConfig_WindowMode>();
		pItem->setPos(x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CONFIG_VIBRATION")
	{
		std::shared_ptr<UiItemDebugConfig_Vibration> pItem = std::make_shared<UiItemDebugConfig_Vibration>();
		pItem->setPos(x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CONFIG_SAVE")
	{
		std::shared_ptr<UiItemDebugConfig_Save> pItem = std::make_shared<UiItemDebugConfig_Save>();
		pItem->setPos(x, y);
		addItem(pItem);
		return pItem;
	}
	else if (itemId == "ITEM_CLEARPRINT")
	{
		std::shared_ptr<UiItemClearPrint> pItem = std::make_shared<UiItemClearPrint>();
		pItem->setup("デバッグ表示のクリア", x, y);
		addItem(pItem);
		return pItem;
	}

	printfDx(itemId.c_str());

	// 存在しないIDが指定された
	assert(false);
	return nullptr;
}

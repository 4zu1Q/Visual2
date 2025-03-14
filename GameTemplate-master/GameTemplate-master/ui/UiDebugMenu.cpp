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

	// printfDx�\�����N���A����
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
	// �t�@�C�����̓ǂݍ���
	std::ifstream ifs(kMenuDataFilename);
	std::string line;

	getline(ifs, line);	// 1�s�ڂ͓ǂݎ̂�

	std::vector<std::shared_ptr<UiItemBase>>	pScrollItemTbl;	// �I��ΏۂƂȂ�A�C�e��
	while (getline(ifs, line))
	{
		// csv�f�[�^1�s��','�ŕ����̕�����ɕ���
		std::vector<std::string>	strvec = StrUtil::split(line, ',');
		// strvec[0]	: ItemID
		// strvec[1]	: �I���\��	0�̏ꍇ�͑I�����ꂸ��΂�
		// strvec[2]	: �V�[���J�ڂ̏ꍇ�A�J�ڐ�ID
		// strvec[3]	: ����(���̍��ڂ̓��e�͎g�p���Ȃ�)

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
		DrawString(x, y, "��", 0xffff00);
	}
}

std::shared_ptr<UiItemBase> UiDebugMenu::addMenuItem(std::string itemId, std::string sceneId, SceneBase* pCurrentScene)
{
	// itemId�ɉ�������ނ̃A�C�e����ǉ�����
	//	ITEM_SCENECHANGE			: �V�[���J��
	//	ITEM_BORDER					: ��؂��	���ꂪ�I������鎖�͂Ȃ�(��΂����)
	//	ITEM_CONFIG_BGMVOL			: �R���t�B�O�ݒ�ύX BGM����
	//	ITEM_CONFIG_SEVOL			: �R���t�B�O�ݒ�ύX SE����
	//	ITEM_CONFIG_WIDOWMODE		: �R���t�B�O�ݒ�ύX �E�C���h�E���[�h
	//	ITEM_CONFIG_VIBRATION		: �R���t�B�O�ݒ�ύX �U��
	//	ITEM_CONFIG_SAVE			: �R���t�B�O�ݒ�ύX�������e��ۑ�
	//	ITEM_CLEARPRINT				: printfDx�̕\�����N���A

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
		pItem->setup("�f�o�b�O�\���̃N���A", x, y);
		addItem(pItem);
		return pItem;
	}

	printfDx(itemId.c_str());

	// ���݂��Ȃ�ID���w�肳�ꂽ
	assert(false);
	return nullptr;
}

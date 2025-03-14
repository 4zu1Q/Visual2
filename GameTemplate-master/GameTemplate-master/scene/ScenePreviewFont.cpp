#include "ScenePreviewFont.h"
#include "DxLib.h"

#include "UiBase.h"
#include "UiItemSimpleText.h"
#include "FontManager.h"
#include "game.h"
#include "SceneDebug.h"
#include "Pad.h"

#include <string>
#include <cassert>

ScenePreviewFont::ScenePreviewFont():
	m_frameCount(0)
{
}

void ScenePreviewFont::startLoad()
{
}

bool ScenePreviewFont::isLoaded() const
{
	return true;
}

void ScenePreviewFont::init()
{
	m_pFontNameMenu = std::make_shared<UiBase>();

	int x = 64;
	int y = 32;

	int fontNum = FontManager::getInstance().getFontNum();
	for (int i = 0; i < fontNum; i++)
	{
		std::shared_ptr<UiItemSimpleText> pItem = std::make_shared<UiItemSimpleText>();
		pItem->setup(FontManager::getInstance().getKey(i), x, y);
		m_pFontNameMenu->addItem(pItem);
		y += 16;
	}

	m_pFontNameMenu->setupButtonSettingUD(true, false);
	m_pFontNameMenu->init();
}

void ScenePreviewFont::end()
{
}

void ScenePreviewFont::update()
{
	if (getFadeProgress() > 0.0f)
	{
		return;
	}
	m_frameCount++;
	m_pFontNameMenu->update();

	if (Pad::isTrigger(PAD_INPUT_2))
	{
		// ��x�Ă񂾂������x�Ă΂Ȃ��悤�ɁI
		// �Ă񂾂�t�F�[�h�A�E�g���n�܂�̂ł��̃\�[�X�ł�isFading�ł͂����Ă���
		setNextScene(new SceneDebug);
	}
}

void ScenePreviewFont::draw()
{
	if (!isInitialized())	return;

	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xaeaeae, true);

	m_pFontNameMenu->draw();

	std::shared_ptr<UiItemSimpleText> pItem = std::dynamic_pointer_cast<UiItemSimpleText>(m_pFontNameMenu->getSelectItem());
	if (pItem)
	{
		std::string fontName = pItem->getText();

		DrawStringToHandle(280, 100, "����������", 0xffffff, FontManager::getInstance().getHandle(fontName), 0x000000);
		DrawStringToHandle(280, 280, "�J�L�N�P�R", 0xffffff, FontManager::getInstance().getHandle(fontName), 0x000000);
		DrawStringToHandle(280, 460, "�i���̐��E", 0xffffff, FontManager::getInstance().getHandle(fontName), 0x000000);
		// �傫�ȃT�C�Y�̃t�H���g����������̎�ޕ\���g�p�Ƃ���Əd���Ȃ�c
	//	DrawStringToHandle(280, 560, "Windows", 0xffffff, FontManager::getInstance().getHandle(fontName), 0x000000);
	}

	DrawString(640, 8, "ScenePreviewFont �t�H���g�m�F�p�V�[��", 0xffffff);
	DrawFormatString(640, 8 + 16, 0xffffff, "FRAME:%d", m_frameCount);
}

#include "UiItemDebugSceneChange.h"

#include "SceneTitle.h"
#include "SceneTest.h"
#include "SceneUiTest.h"
#include "ScenePreviewFont.h"

#include <cassert>

UiItemDebugSceneChange::UiItemDebugSceneChange():
	m_pScene(nullptr)
{
}

UiItemDebugSceneChange::~UiItemDebugSceneChange()
{
}

void UiItemDebugSceneChange::setScene(SceneBase* pScene, std::string nextId)
{
	m_pScene = pScene;
	m_nextId = nextId;
}

void UiItemDebugSceneChange::onDecide()
{
	SceneBase* pNext = nullptr;
	// TITLE
	// TEST
	// UI_TEST
	// END

	if (m_nextId == "TITLE")
	{
		pNext = new SceneTitle{};
	}
	else if (m_nextId == "TEST")
	{
		pNext = new SceneTest{};
	}
	else if (m_nextId == "UI_TEST")
	{
		pNext = new SceneUiTest{};
	}
	else if (m_nextId == "PREVIEW_FONT")
	{
		pNext = new ScenePreviewFont{};
	}
	else if (m_nextId == "END")
	{
		pNext = nullptr;
	}
	else
	{
		// ‘¶Ý‚µ‚È‚¢ƒV[ƒ“ID‚ªŽw’è‚³‚ê‚½
		assert(0);
	}
	m_pScene->setNextScene(pNext);
}

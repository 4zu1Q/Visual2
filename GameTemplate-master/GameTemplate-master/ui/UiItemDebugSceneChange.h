#pragma once
#include "UiItemSimpleText.h"

class SceneBase;
class UiItemDebugSceneChange : public UiItemSimpleText
{
public:
	UiItemDebugSceneChange();
	virtual ~UiItemDebugSceneChange();
	// 現在のシーンとこのメニューからの遷移先シーンを設定
	void setScene(SceneBase* pScene, std::string nextId);
	void onDecide() override;

private:
	SceneBase* m_pScene;
	std::string m_nextId;
};


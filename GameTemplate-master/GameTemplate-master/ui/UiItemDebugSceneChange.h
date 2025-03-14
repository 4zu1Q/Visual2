#pragma once
#include "UiItemSimpleText.h"

class SceneBase;
class UiItemDebugSceneChange : public UiItemSimpleText
{
public:
	UiItemDebugSceneChange();
	virtual ~UiItemDebugSceneChange();
	// ���݂̃V�[���Ƃ��̃��j���[����̑J�ڐ�V�[����ݒ�
	void setScene(SceneBase* pScene, std::string nextId);
	void onDecide() override;

private:
	SceneBase* m_pScene;
	std::string m_nextId;
};


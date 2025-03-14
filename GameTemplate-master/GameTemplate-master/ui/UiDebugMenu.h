#pragma once
#include "UiBase.h"
#include <string>

class SceneBase;
class UiItemBase;
class UiDebugMenu : public UiBase
{
public:
	UiDebugMenu();
	virtual ~UiDebugMenu();

	// �f�[�^��ǂݍ���Ńf�o�b�O���j���[�̍��ڂ𐶐�����
	void createItems(SceneBase* pCurrentScene);

//	virtual void init();
//	virtual void update();
	virtual void draw() override;

private:
	/// <summary>
	/// �f�o�b�O���j���[���ڂ̒ǉ�
	/// </summary>
	/// <param name="itemId">�ǉ�������item��ID</param>
	/// <param name="sceneId">�V�[���J��item�̏ꍇ�A�J�ڐ�̃V�[��ID</param>
	/// <param name="pCurrentScene">���ݎ��s���̃V�[��</param>
	/// <returns>�ǉ�����item�̃|�C���^</returns>
	std::shared_ptr<UiItemBase> addMenuItem(std::string itemId, std::string sceneId, SceneBase* pCurrentScene);


private:

};


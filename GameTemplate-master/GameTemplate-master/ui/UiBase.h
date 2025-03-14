#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <map>

// ���j���[�̊��N���X
// ����Ă��邱�Ƃ͂قڃ��j���[���ڂ̊Ǘ��̂�
class UiItemBase;
class UiBase
{
public:
	UiBase();
	virtual ~UiBase();

	virtual void init();
	virtual void update();
	virtual void draw();

	// ���j���[���ڂ̒ǉ�
	void addItem(std::shared_ptr<UiItemBase> pItem);
	// �㉺�����ł̃{�^���ړ��ݒ�
	void setupButtonSettingUD(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep);
	void setupButtonSettingUD(bool isLoopTrg, bool isLoopRep);	// �o�^����Item�S�Ă�Ώۂɂ���
	// ���E�����ł̃{�^���ړ��ݒ�
	void setupButtonSettingLR(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep);
	void setupButtonSettingLR(bool isLoopTrg, bool isLoopRep);	// �o�^����Item�S�Ă�Ώۂɂ���

	// �I������Ă��鍀�ڂ��ς�������̏���
	// m_pSelectItem���ύX��̍��ڂɂȂ��Ă���Ă΂��
	virtual void onChangeSelect();

	// ���ݑI������Ă��鍀��
	std::shared_ptr<UiItemBase> getSelectItem() const { return m_pSelectItem; }

	// ���s�[�g�t���[���ύX
	void setFirstRepeatFrame(int frame) { m_firstRepeatFrame = frame; }
	void setRepeatFrame(int frame) { m_repeatFrame = frame; }

private:

	void setupButtonSetting(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton);
	void setupButtonSetting(bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton);

protected:

	// �I������
	std::vector<std::shared_ptr<UiItemBase>>	m_pItem;
	// ���ݑI������Ă��鍀��
	std::shared_ptr<UiItemBase>	m_pSelectItem;

	// �L�[���s�[�g
	// �{�^��(PAD_INPUT_UP�Ȃ�)�A�t���[����
	std::map<int, int> m_repeatCount;

	// �L�[���s�[�g�t���[��
	int m_firstRepeatFrame;	// �g���K�[���Ă��烊�s�[�g�܂ł̃t���[����
	int m_repeatFrame;		// 2��ڈȍ~�̃��s�[�g�t���[����
};


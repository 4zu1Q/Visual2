#pragma once
#include "UiItemBase.h"
#include <string>
// �f�o�b�O���j���[����R���t�B�O�̊e���ڂ�ύX���邽�߂�UI

class UiItemDebugConfigBase : public UiItemBase
{
public:
	UiItemDebugConfigBase() {}
	virtual ~UiItemDebugConfigBase() {}

	virtual void init() override {};

	void drawItemText(std::string text);
};

// BGM�{�����[��
class UiItemDebugConfig_BgmVol : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_BgmVol() {}
	virtual ~UiItemDebugConfig_BgmVol() {}

	virtual void update() override;
	virtual void draw() override;
};

// SE�{�����[��
class UiItemDebugConfig_SeVol : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_SeVol() {}
	virtual ~UiItemDebugConfig_SeVol() {}

	virtual void update() override;
	virtual void draw() override;
};

// �E�C���h�E���[�h
class UiItemDebugConfig_WindowMode : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_WindowMode() {}
	virtual ~UiItemDebugConfig_WindowMode() {}

	virtual void update() override;
	virtual void draw() override;

	virtual void onDecide() override;
	virtual void onUnselect() override;
private:
	// �R���t�B�O�̐ݒ�ƌ��݂̃E�C���h�E���[�h���Ⴄ
	bool isDifferMode();
};

// �U��ONOFF
class UiItemDebugConfig_Vibration : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_Vibration() {}
	virtual ~UiItemDebugConfig_Vibration() {}

	virtual void update() override;
	virtual void draw() override;
};

// ����


// �ݒ��ۑ�
class UiItemDebugConfig_Save : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_Save() {}
	virtual ~UiItemDebugConfig_Save() {}

	virtual void update() override;
	virtual void draw() override;

	virtual void onDecide() override;
};

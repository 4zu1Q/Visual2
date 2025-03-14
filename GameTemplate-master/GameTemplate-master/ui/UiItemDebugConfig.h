#pragma once
#include "UiItemBase.h"
#include <string>
// デバッグメニューからコンフィグの各項目を変更するためのUI

class UiItemDebugConfigBase : public UiItemBase
{
public:
	UiItemDebugConfigBase() {}
	virtual ~UiItemDebugConfigBase() {}

	virtual void init() override {};

	void drawItemText(std::string text);
};

// BGMボリューム
class UiItemDebugConfig_BgmVol : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_BgmVol() {}
	virtual ~UiItemDebugConfig_BgmVol() {}

	virtual void update() override;
	virtual void draw() override;
};

// SEボリューム
class UiItemDebugConfig_SeVol : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_SeVol() {}
	virtual ~UiItemDebugConfig_SeVol() {}

	virtual void update() override;
	virtual void draw() override;
};

// ウインドウモード
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
	// コンフィグの設定と現在のウインドウモードが違う
	bool isDifferMode();
};

// 振動ONOFF
class UiItemDebugConfig_Vibration : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_Vibration() {}
	virtual ~UiItemDebugConfig_Vibration() {}

	virtual void update() override;
	virtual void draw() override;
};

// 言語


// 設定を保存
class UiItemDebugConfig_Save : public UiItemDebugConfigBase
{
public:
	UiItemDebugConfig_Save() {}
	virtual ~UiItemDebugConfig_Save() {}

	virtual void update() override;
	virtual void draw() override;

	virtual void onDecide() override;
};

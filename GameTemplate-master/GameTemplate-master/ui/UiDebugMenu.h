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

	// データを読み込んでデバッグメニューの項目を生成する
	void createItems(SceneBase* pCurrentScene);

//	virtual void init();
//	virtual void update();
	virtual void draw() override;

private:
	/// <summary>
	/// デバッグメニュー項目の追加
	/// </summary>
	/// <param name="itemId">追加したいitemのID</param>
	/// <param name="sceneId">シーン遷移itemの場合、遷移先のシーンID</param>
	/// <param name="pCurrentScene">現在実行中のシーン</param>
	/// <returns>追加したitemのポインタ</returns>
	std::shared_ptr<UiItemBase> addMenuItem(std::string itemId, std::string sceneId, SceneBase* pCurrentScene);


private:

};


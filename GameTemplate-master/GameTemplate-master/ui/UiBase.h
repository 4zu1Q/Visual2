#pragma once
#include "DxLib.h"
#include <memory>
#include <vector>
#include <map>

// メニューの基底クラス
// やっていることはほぼメニュー項目の管理のみ
class UiItemBase;
class UiBase
{
public:
	UiBase();
	virtual ~UiBase();

	virtual void init();
	virtual void update();
	virtual void draw();

	// メニュー項目の追加
	void addItem(std::shared_ptr<UiItemBase> pItem);
	// 上下方向でのボタン移動設定
	void setupButtonSettingUD(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep);
	void setupButtonSettingUD(bool isLoopTrg, bool isLoopRep);	// 登録したItem全てを対象にする
	// 左右方向でのボタン移動設定
	void setupButtonSettingLR(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep);
	void setupButtonSettingLR(bool isLoopTrg, bool isLoopRep);	// 登録したItem全てを対象にする

	// 選択されている項目が変わった時の処理
	// m_pSelectItemが変更後の項目になってから呼ばれる
	virtual void onChangeSelect();

	// 現在選択されている項目
	std::shared_ptr<UiItemBase> getSelectItem() const { return m_pSelectItem; }

	// リピートフレーム変更
	void setFirstRepeatFrame(int frame) { m_firstRepeatFrame = frame; }
	void setRepeatFrame(int frame) { m_repeatFrame = frame; }

private:

	void setupButtonSetting(std::vector<std::shared_ptr<UiItemBase>> pItem, bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton);
	void setupButtonSetting(bool isLoopTrg, bool isLoopRep, int nextButton, int prevButton);

protected:

	// 選択項目
	std::vector<std::shared_ptr<UiItemBase>>	m_pItem;
	// 現在選択されている項目
	std::shared_ptr<UiItemBase>	m_pSelectItem;

	// キーリピート
	// ボタン(PAD_INPUT_UPなど)、フレーム数
	std::map<int, int> m_repeatCount;

	// キーリピートフレーム
	int m_firstRepeatFrame;	// トリガーしてからリピートまでのフレーム数
	int m_repeatFrame;		// 2回目以降のリピートフレーム数
};


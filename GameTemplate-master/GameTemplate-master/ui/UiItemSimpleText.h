#pragma once
#include "UiItemBase.h"
#include <string>

// デフォルトの文字表示を使用したシンプルな選択項目
// 基本的にデバッグ等に使用する想定
class UiItemSimpleText : public UiItemBase
{
public:
	UiItemSimpleText();
	virtual ~UiItemSimpleText();

	void setup(std::string text, int x, int y);
	void setText(std::string text);	// 主にテキストの変更に使用する

	void setSelectColor(unsigned int color) { m_selectColor = color; }
	void setUnselectColor(unsigned int color) { m_unselectColor = color; }

	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;

	virtual void onDecide() override;	// 決定したときの処理
	virtual void onSelect() override;	// 選択状態になったとき
	virtual void onUnselect() override;	// 選択状態が解除されたとき

	std::string getText() const { return m_text; }

private:
	std::string m_text;	// 表示する文字

	unsigned int m_selectColor;
	unsigned int m_unselectColor;
};


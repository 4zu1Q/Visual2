#pragma once
#include "UiItemBase.h"
#include <string>
class UiItemSimpleBox : public UiItemBase
{
public:
	UiItemSimpleBox();
	virtual ~UiItemSimpleBox();

	void setup(std::string text, int x, int y, int w, int h);

	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;

//	virtual void onDecide() override;	// 決定したときの処理
//	virtual void onSelect() override;	// 選択状態になったとき
//	virtual void onUnselect() override;	// 選択状態が解除されたとき

	void setFontHandle(int handle) { m_fontHandle = handle; }
	void setStrOffsetY(int offset) { m_strOffsetY = offset; }	// フォントの都合でいい感じに見えない場合表示位置を上下にずらす

private:
	std::string m_text;	// 表示する文字

	int m_fontHandle;
	int m_strOffsetY;	// 文字表示位置調整用

	int m_selectFrame;
};


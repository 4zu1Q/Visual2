#pragma once
#include "UiBase.h"

// 選択項目が画面内に収まるようスクロールさせることのできるUI
class UiScroll : public UiBase
{
public:
	UiScroll();
	virtual ~UiScroll();

//	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;

private:
	// itemのスクロール量
	float m_scrollX;
	float m_scrollY;


};


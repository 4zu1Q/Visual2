#pragma once
#include "UiBase.h"

// �I�����ڂ���ʓ��Ɏ��܂�悤�X�N���[�������邱�Ƃ̂ł���UI
class UiScroll : public UiBase
{
public:
	UiScroll();
	virtual ~UiScroll();

//	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;

private:
	// item�̃X�N���[����
	float m_scrollX;
	float m_scrollY;


};


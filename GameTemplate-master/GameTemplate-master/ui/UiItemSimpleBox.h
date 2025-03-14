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

//	virtual void onDecide() override;	// ���肵���Ƃ��̏���
//	virtual void onSelect() override;	// �I����ԂɂȂ����Ƃ�
//	virtual void onUnselect() override;	// �I����Ԃ��������ꂽ�Ƃ�

	void setFontHandle(int handle) { m_fontHandle = handle; }
	void setStrOffsetY(int offset) { m_strOffsetY = offset; }	// �t�H���g�̓s���ł��������Ɍ����Ȃ��ꍇ�\���ʒu���㉺�ɂ��炷

private:
	std::string m_text;	// �\�����镶��

	int m_fontHandle;
	int m_strOffsetY;	// �����\���ʒu�����p

	int m_selectFrame;
};


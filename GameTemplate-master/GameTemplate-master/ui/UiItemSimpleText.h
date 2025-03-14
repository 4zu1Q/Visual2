#pragma once
#include "UiItemBase.h"
#include <string>

// �f�t�H���g�̕����\�����g�p�����V���v���ȑI������
// ��{�I�Ƀf�o�b�O���Ɏg�p����z��
class UiItemSimpleText : public UiItemBase
{
public:
	UiItemSimpleText();
	virtual ~UiItemSimpleText();

	void setup(std::string text, int x, int y);
	void setText(std::string text);	// ��Ƀe�L�X�g�̕ύX�Ɏg�p����

	void setSelectColor(unsigned int color) { m_selectColor = color; }
	void setUnselectColor(unsigned int color) { m_unselectColor = color; }

	virtual void init() override;
	virtual void update() override;
	virtual void draw() override;

	virtual void onDecide() override;	// ���肵���Ƃ��̏���
	virtual void onSelect() override;	// �I����ԂɂȂ����Ƃ�
	virtual void onUnselect() override;	// �I����Ԃ��������ꂽ�Ƃ�

	std::string getText() const { return m_text; }

private:
	std::string m_text;	// �\�����镶��

	unsigned int m_selectColor;
	unsigned int m_unselectColor;
};


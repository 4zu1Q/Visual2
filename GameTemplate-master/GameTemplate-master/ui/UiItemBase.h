#pragma once
#include <memory>
#include <map>

// ���j���[���ڂ̊��N���X
class UiItemBase
{
public:
	UiItemBase();
	virtual ~UiItemBase();

	virtual void init() abstract;
	virtual void update() abstract;
	virtual void draw() abstract;

	virtual void onDecide() {};	// ���肵���Ƃ��̏���
	virtual void onSelect() { m_isSelect = true; };	// �I����ԂɂȂ����Ƃ�
	virtual void onUnselect() { m_isSelect = false; };	// �I����Ԃ��������ꂽ�Ƃ�

	/// <summary>
	/// �L�[���������Ƃ��̈ړ���A�C�e����ݒ肷��
	/// </summary>
	/// <param name="key">�ǂ̃{�^�����������Ƃ��̈ړ��悩</param>
	/// <param name="pNext">�ړ���A�C�e��</param>
	/// <param name="isRepeat">���s�[�g���͂ł��ړ����邩</param>
	void setLayout(int key, std::shared_ptr<UiItemBase> pNext, bool isRepeat);

	// ����̃{�^�����������Ƃ��̈ړ���A�C�e�����擾����
	std::shared_ptr<UiItemBase> getTrgNext(int key);	// �w������ւ̃g���K�[���͂ł̑J�ڐ�
	std::shared_ptr<UiItemBase> getRepNext(int key);	// �w������ւ̃��s�[�g���͂ł̑J�ڐ�

	bool isSelect() const { return m_isSelect; }	// �I������Ă��邩�ǂ���

	// �\�����̐ݒ�
	// �p���悪�K�p�ɉ����Ďg�p����
	void setPos(int x, int y);
	void setRect(int x, int y, int w, int h);

	void setScroll(float x, float y);

	float getX() const { return m_x; }
	float getY() const { return m_y; }
	float getW() const { return m_w; }
	float getH() const { return m_h; }

	// �X�N���[���ʂ��v�Z�Ɋ܂߂��\���ʒu
	int getDispX() const;
	int getDispY() const;

private:
	// �����L�[���������Ƃ��̈ړ���A�C�e�����
	std::map<int, std::shared_ptr<UiItemBase>> m_layoutTrg;	// �g���K���͎�
	std::map<int, std::shared_ptr<UiItemBase>> m_layoutRep;	// ���s�[�g���͎�

protected:
	// �I�����
	bool m_isSelect;

	// �\�����	Base�N���X�ł͎g��Ȃ�	�p���悪�K�p�ɉ����Ďg��
	float m_x;
	float m_y;
	float m_w;
	float m_h;
	float m_scrollX;
	float m_scrollY;
};


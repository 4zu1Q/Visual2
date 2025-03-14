#pragma once
#include <memory>
#include <map>

// メニュー項目の基底クラス
class UiItemBase
{
public:
	UiItemBase();
	virtual ~UiItemBase();

	virtual void init() abstract;
	virtual void update() abstract;
	virtual void draw() abstract;

	virtual void onDecide() {};	// 決定したときの処理
	virtual void onSelect() { m_isSelect = true; };	// 選択状態になったとき
	virtual void onUnselect() { m_isSelect = false; };	// 選択状態が解除されたとき

	/// <summary>
	/// キーを押したときの移動先アイテムを設定する
	/// </summary>
	/// <param name="key">どのボタンを押したときの移動先か</param>
	/// <param name="pNext">移動先アイテム</param>
	/// <param name="isRepeat">リピート入力でも移動するか</param>
	void setLayout(int key, std::shared_ptr<UiItemBase> pNext, bool isRepeat);

	// 特定のボタンを押したときの移動先アイテムを取得する
	std::shared_ptr<UiItemBase> getTrgNext(int key);	// 指定方向へのトリガー入力での遷移先
	std::shared_ptr<UiItemBase> getRepNext(int key);	// 指定方向へのリピート入力での遷移先

	bool isSelect() const { return m_isSelect; }	// 選択されているかどうか

	// 表示情報の設定
	// 継承先が必用に応じて使用する
	void setPos(int x, int y);
	void setRect(int x, int y, int w, int h);

	void setScroll(float x, float y);

	float getX() const { return m_x; }
	float getY() const { return m_y; }
	float getW() const { return m_w; }
	float getH() const { return m_h; }

	// スクロール量を計算に含めた表示位置
	int getDispX() const;
	int getDispY() const;

private:
	// 方向キーを押したときの移動先アイテム情報
	std::map<int, std::shared_ptr<UiItemBase>> m_layoutTrg;	// トリガ入力時
	std::map<int, std::shared_ptr<UiItemBase>> m_layoutRep;	// リピート入力時

protected:
	// 選択状態
	bool m_isSelect;

	// 表示情報	Baseクラスでは使わない	継承先が必用に応じて使う
	float m_x;
	float m_y;
	float m_w;
	float m_h;
	float m_scrollX;
	float m_scrollY;
};


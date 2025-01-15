#pragma once
#include "DxLib.h"
#include <memory>
#include "util/Game.h"

class TitleField
{
public:

	TitleField();
	virtual ~TitleField();

	void Initialize();
	//void Update();	//いらない気がする
	void Draw();

	//モデルハンドル取得
	int GetModelHandle() const { return m_modelH; }

	// 自身の当たり判定情報

	virtual int GetColType() const { return m_modelH; }

	VECTOR GetPos() const { return m_pos; }

private:
	int m_modelH;
	VECTOR m_pos;
};


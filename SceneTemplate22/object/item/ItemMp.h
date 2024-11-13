#pragma once
#include "DxLib.h"
#include <memory>

#include "ItemBase.h"

class Player;
class ItemMp : public ItemBase
{
public:
	ItemMp();
	virtual ~ItemMp();

	void Initialize(std::shared_ptr<MyLib::Physics> physics);
	void Finalize(std::shared_ptr<MyLib::Physics> physics);

	void Update(std::shared_ptr<MyLib::Physics> physics);
	void Draw();

private:

	//モデルハンドル
	int m_modelH;
	//座標
	VECTOR m_pos;
	//移動
	VECTOR m_move;

	//回転
	float m_rot;

};


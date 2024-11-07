#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include <memory>

class Colidable;

class ItemHp : public MyLib::Collidable
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Initialize(MyLib::Physics* physics) override;
	void Finalize(MyLib::Physics* physics) override;

	void Update(MyLib::Physics* physics);
	void Draw();

	// 衝突したとき
	void OnCollide(const Collidable& colider) override;

private:

	//モデルハンドル
	int m_modelH;
	//座標
	VECTOR m_pos;
	//移動
	VECTOR m_move;
	//半径
	float m_radius;
	//回転
	float m_rot;

};


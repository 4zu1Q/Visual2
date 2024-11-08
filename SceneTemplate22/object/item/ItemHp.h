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

	void Initialize(std::shared_ptr<MyLib::Physics> physics) override;
	void Finalize(std::shared_ptr<MyLib::Physics> physics) override;

	void Update(std::shared_ptr<MyLib::Physics> physics);
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


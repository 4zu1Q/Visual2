#pragma once
#include "DxLib.h"
#include "myLib/MyLib.h"
#include <memory>

class Colidable;
class Player;

class ItemHp/* : public MyLib::Collidable*/
{
public:
	ItemHp();
	virtual ~ItemHp();

	void Initialize(/*MyLib::Physics* physics*/);
	void Finalize(/*MyLib::Physics* physics*/);

	void Update(/*MyLib::Physics* physics*/);
	void Draw();

	// 衝突したとき
	//void OnCollide(const Collidable& colider) override;

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool HpHit(std::shared_ptr<Player> pPlayer);

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


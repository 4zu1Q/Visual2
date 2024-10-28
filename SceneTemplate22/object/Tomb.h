#pragma once
#include "DxLib.h"
#include <memory>

class Player;
class Tomb
{
public:
	Tomb();
	virtual ~Tomb();

	void Initialize();
	void Finalize();

	virtual void Update();
	virtual void Draw();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <param name="pos">墓のポジション</param>
	/// <returns></returns>
	bool Hit(std::shared_ptr<Player> pPlayer, VECTOR pos);

	/// <summary>
	/// パワーボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombPHit(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// スピードボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombSHit(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// ラスボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombRHit(std::shared_ptr<Player> pPlayer);


private:
	//ボスの当たり判定の半径
	int m_radiusB;

	//ラスボスの当たり判定の半径
	int m_radiusRB;

	//墓のポジション
	VECTOR m_posP;
	VECTOR m_posS;
	VECTOR m_posR;

};


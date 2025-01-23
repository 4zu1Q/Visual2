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
	bool TombPowerHit(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// スピードボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombSpeedHit(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// ショットボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombShotHit(std::shared_ptr<Player> pPlayer);

	/// <summary>
	/// ラスボスシーンに行くための墓の当たり判定
	/// </summary>
	/// <param name="pPlayer"></param>
	/// <returns></returns>
	bool TombRastHit(std::shared_ptr<Player> pPlayer);

	const VECTOR& GetPowerPos() const { return m_posPower; }
	const VECTOR& GetSpeedPos() const { return m_posSpeed; }
	const VECTOR& GetShotPos() const { return m_posShot; }
	const VECTOR& GetRastPos() const { return m_posRast; }

private:
	//ボスの当たり判定の半径
	int m_radiusB;

	//ラスボスの当たり判定の半径
	int m_radiusRB;

	int m_modelTrianglePowerH;
	int m_modelTriangleSpeedH;
	int m_modelTriangleShotH;
	int m_modelTriangleRassH;

	//墓のポジション
	VECTOR m_posPower;
	VECTOR m_posSpeed;
	VECTOR m_posShot;
	VECTOR m_posRast;

	VECTOR m_posTrianglePower;
	VECTOR m_posTriangleSpeed;
	VECTOR m_posTriangleShot;

	//仮の名
	VECTOR m_posTriangleRass;

};


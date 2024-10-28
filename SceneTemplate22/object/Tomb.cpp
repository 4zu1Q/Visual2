#include "Tomb.h"

#include "object/player/Player.h"

namespace
{
	constexpr float kRotSpeed = 0.009f;

	const char* const kItemHpModelFilename = "Data/Model/Obj/Potion.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	constexpr float kAddPosY = 8.0f;

	//モデルのスケール値
	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };

	//モデルのポジション
	constexpr VECTOR kTombPPos = { 30.0f , 0.0f , 0.0f };
	constexpr VECTOR kTombSPos = { -30.0f , 0.0f , 0.0f };
	constexpr VECTOR kTombRPos = { 0.0f , 0.0f , 50.0f };

}

Tomb::Tomb():
	m_radiusB(12),
	m_radiusRB(24),
	m_posP(kTombPPos),
	m_posS(kTombSPos),
	m_posR(kTombRPos)
{
}

Tomb::~Tomb()
{
}

void Tomb::Initialize()
{
}

void Tomb::Finalize()
{
}

void Tomb::Update()
{

}

void Tomb::Draw()
{
	DrawSphere3D(m_posP, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posS, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posR, m_radiusRB, 32, 0xffffff, 0xff0000, false);
}

bool Tomb::Hit(std::shared_ptr<Player> pPlayer, VECTOR pos)
{
	return false;
}

bool Tomb::TombPHit(std::shared_ptr<Player> pPlayer)
{
	/*カプセルと円の当たり判定*/
	//カプセルと球のベクトル
	VECTOR Cap1ToSph = VSub(m_posP, pPlayer->GetPosDown());

	//カプセルの上の座標と下の座標のベクトル
	VECTOR Cap1ToCap2 = VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown());

	//正規化
	VECTOR normal = VNorm(Cap1ToCap2);

	//内積
	float dot = VDot(Cap1ToSph, normal);


	// 球からカプセルへの線上最近点
	VECTOR point = VGet(pPlayer->GetPosDown().x + (normal.x * dot), pPlayer->GetPosDown().y + (normal.y * dot), pPlayer->GetPosDown().z + (normal.z * dot));

	float temp = VSize(VSub(point, pPlayer->GetPosDown())) / VSize(VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown()));
	float distance;


	if (temp < 0) // 球からカプセル線分に垂線をおろせず、GetPosDownに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosDown()));
	}
	else if (temp > 1) // 球からカプセル線分に垂線をおろせず、GetPosUpに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosUp()));
	}
	else // 球からカプセル線分に垂線をおろせる
	{
		distance = VSize(VSub(point, m_posP));
	}

	// 当たっているとき
	if (distance < m_radiusB + pPlayer->GetRadius())
	{

		return true;
	}

	return false;
}

bool Tomb::TombSHit(std::shared_ptr<Player> pPlayer)
{
	/*カプセルと円の当たり判定*/
	//カプセルと球のベクトル
	VECTOR Cap1ToSph = VSub(m_posS, pPlayer->GetPosDown());

	//カプセルの上の座標と下の座標のベクトル
	VECTOR Cap1ToCap2 = VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown());

	//正規化
	VECTOR normal = VNorm(Cap1ToCap2);

	//内積
	float dot = VDot(Cap1ToSph, normal);


	// 球からカプセルへの線上最近点
	VECTOR point = VGet(pPlayer->GetPosDown().x + (normal.x * dot), pPlayer->GetPosDown().y + (normal.y * dot), pPlayer->GetPosDown().z + (normal.z * dot));

	float temp = VSize(VSub(point, pPlayer->GetPosDown())) / VSize(VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown()));
	float distance;


	if (temp < 0) // 球からカプセル線分に垂線をおろせず、GetPosDownに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosDown()));
	}
	else if (temp > 1) // 球からカプセル線分に垂線をおろせず、GetPosUpに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosUp()));
	}
	else // 球からカプセル線分に垂線をおろせる
	{
		distance = VSize(VSub(point, m_posS));
	}

	// 当たっているとき
	if (distance < m_radiusB + pPlayer->GetRadius())
	{

		return true;
	}

	return false;
}

bool Tomb::TombRHit(std::shared_ptr<Player> pPlayer)
{
	/*カプセルと円の当たり判定*/
	//カプセルと球のベクトル
	VECTOR Cap1ToSph = VSub(m_posR, pPlayer->GetPosDown());

	//カプセルの上の座標と下の座標のベクトル
	VECTOR Cap1ToCap2 = VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown());

	//正規化
	VECTOR normal = VNorm(Cap1ToCap2);

	//内積
	float dot = VDot(Cap1ToSph, normal);


	// 球からカプセルへの線上最近点
	VECTOR point = VGet(pPlayer->GetPosDown().x + (normal.x * dot), pPlayer->GetPosDown().y + (normal.y * dot), pPlayer->GetPosDown().z + (normal.z * dot));

	float temp = VSize(VSub(point, pPlayer->GetPosDown())) / VSize(VSub(pPlayer->GetPosUp(), pPlayer->GetPosDown()));
	float distance;


	if (temp < 0) // 球からカプセル線分に垂線をおろせず、GetPosDownに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosDown()));
	}
	else if (temp > 1) // 球からカプセル線分に垂線をおろせず、GetPosUpに近い場所
	{
		distance = VSize(VSub(point, pPlayer->GetPosUp()));
	}
	else // 球からカプセル線分に垂線をおろせる
	{
		distance = VSize(VSub(point, m_posR));
	}

	// 当たっているとき
	if (distance < m_radiusRB + pPlayer->GetRadius())
	{

		return true;
	}

	return false;
}

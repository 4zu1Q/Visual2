#include "Tomb.h"

#include "object/player/Player.h"

namespace
{
	constexpr float kRotSpeed = 0.009f;

	//const char* const kItemHpModelFilename = "Data/Model/Obj/Potion.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	constexpr float kAddPosY = 8.0f;

	//モデルのスケール値
	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };

	//ポジション
	constexpr VECTOR kTombPowerPos = { -460.0f , 38.0f , 264.0f };
	constexpr VECTOR kTombSpeedPos = { 327.0f , 138.0f , 602.0f };
	constexpr VECTOR kTombShotPos = { 0.0f , 68.0f , -50.0f };
	constexpr VECTOR kTombRastPos = { 0.0f , 38.0f , 50.0f };

	//トライアングルのポジション
	constexpr VECTOR kTrianglePowerPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleSpeedPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleShotPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleRastPos = { 0.0f , 38.0f , 50.0f };

}

Tomb::Tomb():
	m_radiusB(12),
	m_radiusRB(24),
	m_posPower(kTombPowerPos),
	m_posSpeed(kTombSpeedPos),
	m_posShot(kTombShotPos),
	m_posRast(kTombRastPos),
	m_posTrianglePower(),
	m_posTriangleSpeed(),
	m_posTriangleShot(),
	m_posTriangleRass()
{
	m_modelTrianglePowerH = 0;
	m_modelTriangleSpeedH = 0;
	m_modelTriangleShotH = 0;
	m_modelTriangleRassH = 0;

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
	DrawSphere3D(m_posPower, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posSpeed, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posShot, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posRast, m_radiusRB, 32, 0xffffff, 0xff0000, false);

	//DrawFormatString(0, 48, 0xff0fff, "PowerPos:%f,%f,%f", m_posPower.x, m_posPower.y, m_posPower.z);
	//DrawFormatString(0, 64, 0xff0fff, "SpeedPos:%f,%f,%f", m_posSpeed.x, m_posSpeed.y, m_posSpeed.z);
	//DrawFormatString(0, 80, 0xff0fff, "ShotPos:%f,%f,%f", m_posShot.x, m_posShot.y, m_posShot.z);
	//DrawFormatString(0, 96, 0xff0fff, "RastPos:%f,%f,%f", m_posRast.x, m_posRast.y, m_posRast.z);
}

bool Tomb::Hit(std::shared_ptr<Player> pPlayer, VECTOR pos)
{
	return false;
}

bool Tomb::TombPowerHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posPower.x - pPlayer->GetPos().x) * (m_posPower.x - pPlayer->GetPos().x);
	float delY = ((m_posPower.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posPower.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posPower.z - pPlayer->GetPos().z) * (m_posPower.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Tomb::TombSpeedHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posSpeed.x - pPlayer->GetPos().x) * (m_posSpeed.x - pPlayer->GetPos().x);
	float delY = ((m_posSpeed.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posSpeed.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posSpeed.z - pPlayer->GetPos().z) * (m_posSpeed.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Tomb::TombShotHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posShot.x - pPlayer->GetPos().x) * (m_posShot.x - pPlayer->GetPos().x);
	float delY = ((m_posShot.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posShot.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posShot.z - pPlayer->GetPos().z) * (m_posShot.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Tomb::TombRastHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posRast.x - pPlayer->GetPos().x) * (m_posRast.x - pPlayer->GetPos().x);
	float delY = ((m_posRast.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posRast.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posRast.z - pPlayer->GetPos().z) * (m_posRast.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusRB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

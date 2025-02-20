#include "Tomb.h"

#include "object/player/Player.h"

namespace
{

	//トライアングルのモデルファイル名
	const char* const kTrianglePowerModelFileName = "Data/Model/Item/PowerClearItem.mv1";
	const char* const kTriangleSpeedModelFileName = "Data/Model/Item/SpeedClearItem.mv1";
	const char* const kTriangleShotModelFileName = "Data/Model/Item/ShotClearItem.mv1";


	constexpr float kAddPosY = 8.0f;

	//モデルのスケール値
	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };

	//ポジション
	constexpr VECTOR kTombPowerPos = { -210.0f , -38.0f , 641.0f };
	constexpr VECTOR kTombSpeedPos = { 1025.0f , -38.0f , 635.0f };
	constexpr VECTOR kTombShotPos = { 414.0f , -38.0f , -413.0f };
	constexpr VECTOR kTombRastPos = { 415.0f , 3.5f , 272.0f };
	//トライアングルのポジション
	constexpr VECTOR kTrianglePowerPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleSpeedPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleShotPos = { 0.0f , 38.0f , 50.0f };
	constexpr VECTOR kTriangleRastPos = { 0.0f , 38.0f , 50.0f };

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.05f;

	//回転速度
	constexpr float kRotSpeed = 0.05f;
}

Tomb::Tomb() :
	m_radiusB(12),
	m_radiusRB(24),
	m_posPower(kTombPowerPos),
	m_posSpeed(kTombSpeedPos),
	m_posShot(kTombShotPos),
	m_posRast(kTombRastPos),
	m_posPowerTriangle(VGet(0, 340, 0)),
	m_posSpeedTriangle(VGet(0, 340, 0)),
	m_posShotTriangle(VGet(0, 340, 0))
{
	m_angle = 0.0f;
	m_moveHeight = 0.0f;

	m_modelTrianglePowerH = MV1LoadModel(kTrianglePowerModelFileName);
	m_modelTriangleSpeedH = MV1LoadModel(kTriangleSpeedModelFileName);
	m_modelTriangleShotH = MV1LoadModel(kTriangleShotModelFileName);
}

Tomb::~Tomb()
{
	Finalize();
}

void Tomb::Initialize()
{
	MV1SetScale(m_modelTrianglePowerH, kModelScale);
	MV1SetScale(m_modelTriangleSpeedH, kModelScale);
	MV1SetScale(m_modelTriangleShotH, kModelScale);

	//モデルに座標をセットする
	MV1SetPosition(m_modelTrianglePowerH, m_posPowerTriangle);
	MV1SetPosition(m_modelTriangleSpeedH, m_posSpeedTriangle);
	MV1SetPosition(m_modelTriangleShotH, m_posShotTriangle);
}

void Tomb::Finalize()
{
	MV1DeleteModel(m_modelTrianglePowerH);
	MV1DeleteModel(m_modelTriangleSpeedH);
	MV1DeleteModel(m_modelTriangleShotH);
	m_modelTrianglePowerH = -1;
	m_modelTriangleSpeedH = -1;
	m_modelTriangleShotH = -1;
}

void Tomb::Update(VECTOR powerPos, VECTOR speedPos, VECTOR shotPos)
{
	//モデルの回転
	//モデルの移動
	m_posPowerTriangle = powerPos;
	m_posSpeedTriangle = speedPos;
	m_posShotTriangle = shotPos;

	m_angle += kRotSpeed;

	//1回転したら値をリセット
	if (m_angle >= DX_PI_F) m_angle = 0.0f;

	//上下に動く処理
	static float SinCount = 0;
	SinCount += kSinSpeed;
	m_moveHeight = sinf(SinCount) * kHeightSize;

	m_posPowerTriangle.y += m_moveHeight;
	m_posSpeedTriangle.y += m_moveHeight;
	m_posShotTriangle.y += m_moveHeight;

	//モデルに座標をセットする
	MV1SetPosition(m_modelTrianglePowerH, m_posPowerTriangle);
	MV1SetPosition(m_modelTriangleSpeedH, m_posSpeedTriangle);
	MV1SetPosition(m_modelTriangleShotH, m_posShotTriangle);

	//
	MV1SetRotationXYZ(m_modelTrianglePowerH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
	MV1SetRotationXYZ(m_modelTriangleSpeedH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
	MV1SetRotationXYZ(m_modelTriangleShotH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

void Tomb::Draw()
{
#ifdef _DEBUG
	DrawSphere3D(m_posPower, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posSpeed, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posShot, m_radiusB, 32, 0xffffff, 0xff0000, false);
	DrawSphere3D(m_posRast, m_radiusRB, 32, 0xffffff, 0xff0000, false);
#endif

}

void Tomb::DrawTriangle(Game::e_BossKind bossKind)
{
	if (bossKind == Game::e_BossKind::kPower)
	{
		MV1DrawModel(m_modelTrianglePowerH);
	}
	else if (bossKind == Game::e_BossKind::kSpeed)
	{
		MV1DrawModel(m_modelTriangleSpeedH);
	}
	else if (bossKind == Game::e_BossKind::kShot)
	{
		MV1DrawModel(m_modelTriangleShotH);
	}
	
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

bool Tomb::TrianglePowerHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posPowerTriangle.x - pPlayer->GetPos().x) * (m_posPowerTriangle.x - pPlayer->GetPos().x);
	float delY = ((m_posPowerTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posPowerTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posPowerTriangle.z - pPlayer->GetPos().z) * (m_posPowerTriangle.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusRB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Tomb::TriangleSpeedHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posSpeedTriangle.x - pPlayer->GetPos().x) * (m_posSpeedTriangle.x - pPlayer->GetPos().x);
	float delY = ((m_posSpeedTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posSpeedTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posSpeedTriangle.z - pPlayer->GetPos().z) * (m_posSpeedTriangle.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusRB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Tomb::TriangleShotHit(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_posShotTriangle.x - pPlayer->GetPos().x) * (m_posShotTriangle.x - pPlayer->GetPos().x);
	float delY = ((m_posShotTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_posShotTriangle.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_posShotTriangle.z - pPlayer->GetPos().z) * (m_posShotTriangle.z - pPlayer->GetPos().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radiusRB + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

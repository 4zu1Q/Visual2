#include "ItemBase.h"

#include "object/player/Player.h"


namespace
{
	constexpr float kRotSpeed = 0.009f;

	const char* const kItemHpModelFilename = "Data/Model/Obj/Potion.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	constexpr float kAddPosY = 8.0f;

	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };

}

ItemBase::ItemBase() :
	m_modelH(-1),
	m_pos(VGet(0, 0, 0)),
	m_move(VGet(0,0,0)),
	m_radius(6.0f),
	m_rot(0.0f)
{
	m_modelH = MV1LoadModel(kItemHpModelFilename);
	MV1SetScale(m_modelH, kModelScale);
}

ItemBase::~ItemBase()
{

}

void ItemBase::Update()
{
	m_rot += kRotSpeed;

	//1回転したら値をリセット
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//上下に動く処理
	static float SinCount = 0;
	SinCount += kSinSpeed;
	m_move.y = sinf(SinCount) * kHeightSize;

	m_pos.y += m_move.y;

	MV1SetRotationXYZ(m_modelH, VGet(0, m_rot, 0));
	MV1SetPosition(m_modelH, m_pos);
}

void ItemBase::Draw()
{
	MV1DrawModel(m_modelH);
}

void ItemBase::ItemLost()
{

}

bool ItemBase::ItemSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Zの距離の成分を取得
	float delX = (m_pos.x - pPlayer->GetPosDown().x) * (m_pos.x - pPlayer->GetPosDown().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPosDown().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPosDown().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPosDown().z) * (m_pos.z - pPlayer->GetPosDown().z);

	//球と球の距離
	float Distance = sqrt(delX + delY + delZ);

	//球と球の距離がプレイヤとエネミーの半径よりも小さい場合
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

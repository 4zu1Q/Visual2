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
	m_radius(3.0f),
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

void ItemBase::Draw(std::shared_ptr<Player> pPlayer)
{
	MV1DrawModel(m_modelH);

	DrawCapsule3D(pPlayer->GetPosDown(), pPlayer->GetPosUp(), pPlayer->GetRadius() , 32, 0xffffff, 0xffffff, false);
	DrawSphere3D(m_pos, m_radius, 32, 0xffffff, 0xff0000, false);


}

void ItemBase::ItemLost()
{

}

bool ItemBase::ItemHit(std::shared_ptr<Player> pPlayer)
{
	
	/*カプセルと円の当たり判定*/

	//カプセルと球のベクトル
	VECTOR Cap1ToSph = VSub(m_pos, pPlayer->GetPosDown());

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
		distance = VSize(VSub(point, m_pos));
	}

	// 当たっているとき
	if (distance < m_radius + pPlayer->GetRadius()) 
	{

		return true;
	}

	return false;

}


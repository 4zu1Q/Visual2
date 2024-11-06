#include "ItemHp.h"

#include "myLib/MyLib.h"

#include "object/player/Player.h"

namespace
{
	//モデルのファイル名
	const char* const kItemHpModelFilename = "Data/Model/Obj/Heart.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	//モデルのスケール値
	constexpr VECTOR kModelScale = { 0.08f , 0.08f , 0.08f };
	//モデルの初期位置
	constexpr VECTOR kModelInitPos = { -10.0f , 0.0f , 50.0f };

	//回転速度
	constexpr float kRotSpeed = 0.009f;

}

ItemHp::ItemHp():
	//Collidable(Collidable::e_Priority::kLow, Game::e_GameObjectTag::kItem, MyLib::ColliderData::e_Kind::kSphere, false),
	m_modelH(-1),
	m_pos(kModelInitPos),
	m_move(VGet(0, 0, 0)),
	m_radius(3.0f),
	m_rot(0.0f)
{
	m_modelH = MV1LoadModel(kItemHpModelFilename);
	MV1SetScale(m_modelH, kModelScale);

	//auto circleColliderData = dynamic_cast<MyLib::ColliderDataSphere*>(m_colliderData);
	//circleColliderData->m_radius = 5.0f;
}

ItemHp::~ItemHp()
{

}

void ItemHp::Initialize(/*MyLib::Physics* physics*/)
{
	//Collidable::Initialize(physics);

	//// 物理挙動の初期化
	//m_rigidbody.Initialize(false);
	//m_rigidbody.SetPos(kModelInitPos);
}

void ItemHp::Finalize(/*MyLib::Physics* physics*/)
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	//Collidable::Finalize(physics);
}

void ItemHp::Update(/*MyLib::Physics* physics*/)
{
	m_rot += kRotSpeed;

	//auto pos = m_rigidbody.GetPos();


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

void ItemHp::Draw()
{
	MV1DrawModel(m_modelH);
	//DrawSphere3D(m_pos, m_radius, 32, 0xffffff, 0xff0000, false);

}

bool ItemHp::HpHit(std::shared_ptr<Player> pPlayer)
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

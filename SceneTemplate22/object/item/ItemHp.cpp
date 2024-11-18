#include "ItemHp.h"


namespace
{
	//モデルのファイル名
	const char* const kItemHpModelFilename = "Data/Model/Obj/ItemHeart.mv1";

	//constexpr float kSinSpeed = 0.01f;
	constexpr float kSinSpeed = 0.05f;
	//constexpr float kHeightSize = 0.01f;
	constexpr float kHeightSize = 0.50f;

	//モデルのスケール値
	constexpr VECTOR kModelScale = { 0.06f , 0.06f , 0.06f };

	//モデルの初期位置
	constexpr VECTOR kModelInitPos = { -10.0f , 0.0f , 50.0f };

	//回転速度
	constexpr float kRotSpeed = 0.009f;

}

ItemHp::ItemHp():
	ItemBase(Collidable::e_Priority::kStatic, Game::e_GameObjectTag::kItemHp, MyLib::ColliderData::e_Kind::kSphere, false)

{
	m_modelH = MV1LoadModel(kItemHpModelFilename);
	MV1SetScale(m_modelH, kModelScale);

	
	m_pColliderData = std::make_shared<MyLib::ColliderDataSphere>(false);

	auto circleColliderData = std::dynamic_pointer_cast<MyLib::ColliderDataSphere>(m_pColliderData);
	circleColliderData->m_radius = 2.0f;

}

ItemHp::~ItemHp()
{

}

void ItemHp::Initialize(std::shared_ptr<MyLib::Physics> physics)
{
	Collidable::Initialize(physics);

	// 物理挙動の初期化
	m_rigidbody.Initialize(true);
	m_rigidbody.SetPos(kModelInitPos);
}

void ItemHp::Finalize(std::shared_ptr<MyLib::Physics> physics)
{
	//モデルをデリートする
	MV1DeleteModel(m_modelH);
	m_modelH = -1;

	Collidable::Finalize(physics);
}

void ItemHp::Update(std::shared_ptr<MyLib::Physics> physics)
{
	m_rot += kRotSpeed;

	auto pos = m_rigidbody.GetPos();


	//1回転したら値をリセット
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//上下に動く処理
	static float SinCount = 0;
	SinCount += kSinSpeed;
	m_move.y = sinf(SinCount) * kHeightSize;

	pos.y += m_move.y;

	MV1SetRotationXYZ(m_modelH, VGet(0, m_rot, 0));
	MV1SetPosition(m_modelH, pos);
}

void ItemHp::Draw()
{
	MV1DrawModel(m_modelH);
}

//void ItemHp::OnCollide(const Collidable& colider)
//{
//	std::string message = "アイテムが";
//	auto tag = colider.GetTag();
//	switch (tag)
//	{
//	case Game::e_GameObjectTag::kPlayer:
//		message += "プレイヤー";
//		break;
//	case Game::e_GameObjectTag::kBoss:
//		message += "ボス";
//		break;
//	case Game::e_GameObjectTag::kStepGround:
//		message += "足場";
//		break;
//	default:
//		break;
//	}
//	message += "と当たった！\n";
//	printfDx(message.c_str());
//}


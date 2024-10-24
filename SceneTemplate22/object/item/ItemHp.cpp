#include "ItemHp.h"

namespace
{
	const char* const kItemHpModelFilename = "Data/Model/Obj/Heart.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.01f;

	constexpr VECTOR kModelScale = { 0.2f , 0.2f , 0.2f };
	constexpr VECTOR kModelInitPos = { 0.0f , 0.0f , 0.0f };
}

ItemHp::ItemHp()
{
	m_modelH = MV1LoadModel(kItemHpModelFilename);
	MV1SetScale(m_modelH, kModelScale);
}

ItemHp::~ItemHp()
{

}

void ItemHp::Initialize()
{

}

void ItemHp::Finalize()
{

}

void ItemHp::Update()
{
	MV1SetRotationXYZ(m_modelH, VGet(0, m_rot, 0));
	MV1SetPosition(m_modelH, m_pos);
}

void ItemHp::Draw()
{
}

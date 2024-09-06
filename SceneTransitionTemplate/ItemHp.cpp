#include "DxLib.h"
#include "ItemHp.h"

namespace
{
	constexpr float kRotSpeed = 0.009f;

	const char* const kItemHpModelFilename = "data/model/item/Heart.mv1";

	constexpr float kSinSpeed = 0.01f;
	constexpr float kHeightSize = 0.1f;
}

ItemHp::ItemHp():
	m_handle(MV1LoadModel(kItemHpModelFilename)),
	m_moveHeight(0.0f),
	m_rot(0.0f)
{
	MV1SetScale(m_handle, VGet(0.2f, 0.2f, 0.2f));
}

ItemHp::~ItemHp()
{
	MV1DeleteModel(m_handle);
	m_handle = -1;
}

void ItemHp::Update()
{
	m_rot += kRotSpeed;

	//1回転したら値をリセット
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//上下に動く処理
	static float SinCount = 0;
	SinCount += kSinSpeed;
	m_moveHeight = sinf(SinCount) * kHeightSize;

	m_pos.y += m_moveHeight;

	MV1SetRotationXYZ(m_handle, VGet(0, m_rot, 0));
	MV1SetPosition(m_handle, m_pos);
}

void ItemHp::Draw()
{
	MV1DrawModel(m_handle);

#ifdef _DEBUG

	DrawSphere3D(m_pos, m_radius, 8, 0x0000ff, 0x0000ff, false);

#endif

}

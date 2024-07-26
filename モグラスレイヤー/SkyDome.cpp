#include "SkyDome.h"

namespace
{
	//回転速度
	constexpr float kRotSpeed = 0.0004f;
	//ファイル名
	const char* const kSkyDomeFileName = "data/skyDome/Dome.mv1";
	//スカイドームのスケール
	const VECTOR kSkyDomeScale = VGet(35.0f, 35.0f, 35.0f);
}

SkyDome::SkyDome():
	m_handle(MV1LoadModel(kSkyDomeFileName)),
	m_rot(0.0f)
{
	MV1SetScale(m_handle, kSkyDomeScale);
}

SkyDome::~SkyDome()
{

}

void SkyDome::Update(VECTOR pos)
{
	//回転
	m_rot += kRotSpeed;

	//1回転したら値をリセット
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//スカイドームを更新
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_rot, 0.0f));
	MV1SetPosition(m_handle, VGet(pos.x, 0.0f, pos.z));

}

void SkyDome::Draw()
{
	MV1DrawModel(m_handle);
}

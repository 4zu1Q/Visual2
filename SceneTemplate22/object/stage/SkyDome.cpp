#include "SkyDome.h"

namespace
{
	//回転速度
	constexpr float kRotSpeed = 0.00009f;
	//ファイル名
	const char* const kSkyDomeFileName = "data/model/skyDome/Dome.mv1";
	//スカイドームのスケール
	const VECTOR kSkyDomeScale = VGet(40, 40, 40);
}

SkyDome::SkyDome() :
	m_handle(MV1LoadModel(kSkyDomeFileName)),
	m_pos(VGet(0, 280, 0)),
	m_rot(0.0f)
{

	MV1SetScale(m_handle, kSkyDomeScale);
}

SkyDome::~SkyDome()
{

}

void SkyDome::Update()
{
	//回転
	m_rot += kRotSpeed;

	//スカイドームを更新
	MV1SetRotationXYZ(m_handle, VGet(0, m_rot, 0));
	MV1SetPosition(m_handle, m_pos);

}

void SkyDome::Draw()
{
	MV1DrawModel(m_handle);

#ifdef _DEBUG

	DrawFormatString(0, 128, 0xffffff, "SkyDome(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}
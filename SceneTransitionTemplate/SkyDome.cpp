#include "SkyDome.h"
#include "Player.h"

namespace
{
	//回転速度
	constexpr float kRotSpeed = 0.0009f;
	//ファイル名
	const char* const kSkyDomeFileName = "data/skyDome/Dome.mv1";
	//スカイドームのスケール
	const VECTOR kSkyDomeScale = VGet(8, 8, 8);
}

SkyDome::SkyDome() :
	m_handle(MV1LoadModel(kSkyDomeFileName)),
	m_pos(VGet(0,0,0)),
	m_rot(0.0f)
{

	MV1SetScale(m_handle, kSkyDomeScale);
}

SkyDome::~SkyDome()
{

}

void SkyDome::Update(Player& player)
{
	//回転
	m_rot += kRotSpeed;

	//1回転したら値をリセット
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//m_pos = player.GetPos();

	//スカイドームを更新
	MV1SetRotationXYZ(m_handle, VGet(0, m_rot, 0) );
	MV1SetPosition(m_handle, m_pos);

}

void SkyDome::Draw()
{
	MV1DrawModel(m_handle);

#ifdef _DEBUG
	
	DrawFormatString(0, 48, 0xffffff, "SkyDome(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}
#include "SkyDome.h"
#include "Player.h"

namespace
{
	//回転速度
	constexpr float kRotSpeed = 0.0004f;
	//ファイル名
	const char* const kSkyDomeFileName = "data/skyDome/Dome.mv1";
	//スカイドームのスケール
	const VECTOR kSkyDomeScale = VGet(300, 300, 300);
}

SkyDome::SkyDome() :
	m_handle(MV1LoadModel(kSkyDomeFileName)),
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
	//if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//スカイドームを更新
	MV1SetRotationXYZ(m_handle, m_pos);
	MV1SetPosition(m_handle, VGet(player.GetPos().x, 0.0f, player.GetPos().z));

}

void SkyDome::Draw()
{
	MV1DrawModel(m_handle);

#ifdef _DEBUG
	
	DrawFormatString(0, 48, 0xffffff, "SkyDome(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

#endif
}
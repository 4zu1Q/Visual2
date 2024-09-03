#include "SkyDome.h"
#include "Player.h"

namespace
{
	//��]���x
	constexpr float kRotSpeed = 0.0004f;
	//�t�@�C����
	const char* const kSkyDomeFileName = "data/skyDome/Dome.mv1";
	//�X�J�C�h�[���̃X�P�[��
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
	//��]
	m_rot += kRotSpeed;

	//1��]������l�����Z�b�g
	//if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//�X�J�C�h�[�����X�V
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
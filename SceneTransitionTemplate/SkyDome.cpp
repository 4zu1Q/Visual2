#include "SkyDome.h"

namespace
{
	//��]���x
	constexpr float kRotSpeed = 0.0004f;
	//�t�@�C����
	const char* const kSkyDomeFileName = "data/skyDome/Dome.mv1";
	//�X�J�C�h�[���̃X�P�[��
	const VECTOR kSkyDomeScale = VGet(100, 100, 100);
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

void SkyDome::Update(VECTOR pos)
{
	//��]
	m_rot += kRotSpeed;

	//1��]������l�����Z�b�g
	if (m_rot >= DX_PI_F) m_rot = 0.0f;

	//�X�J�C�h�[�����X�V
	MV1SetRotationXYZ(m_handle, VGet(0.0f, m_rot, 0.0f));
	MV1SetPosition(m_handle, VGet(pos.x, 0.0f, pos.z));

}

void SkyDome::Draw()
{
	MV1DrawModel(m_handle);
}
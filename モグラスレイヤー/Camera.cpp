#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Pad.h"

#include "DxLib.h"
#include <cmath>



/// <summary>
/// �萔
/// </summary>
namespace
{
	constexpr float kCameraDist = 96;
	constexpr float kCameraHeight = 20;

	constexpr float kAngleSpeed = 0.05f;	//���񑬓x
	constexpr float kCameraPlayerTargetHeight = 400.0f;	//�v���C���[���W����ǂꂾ�������ʒu�𒍎��_�Ƃ��邩
	constexpr float kToPlayerLength = 1600.0f;	//�v���C���[�Ƃ̋���
	constexpr float kCollisionSize = 50.0f;		//�J�����̓����蔻��T�C�Y
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Camera::Camera():
	m_pos(VGet(0,10,0)),
	m_targetPos(VGet(0,0,0)),
	m_angle(-DX_PI_F / 2)
{
	SetCameraNearFar(20, 600);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Camera::~Camera()
{
}

/// <summary>
/// ������
/// </summary>
void Camera::Init()
{
	
	
}

/// <summary>
/// �v���C�V�[���̃A�b�v�f�[�g
/// </summary>
void Camera::PlayCameraUpdate(Player& player)
{
	Pad::Update();

#ifdef _DEBUG

	DrawGrid();

#endif
	
	// �J�����Ɉʒu�𔽉f
	//�����_�̍��W
	VECTOR playerAimPos = VGet(player.GetPos().x, player.GetPos().y + 20.0f, player.GetPos().z - 50.0f);
	//�x�N�g���̕���(�����_-�J�����̃|�W�V����)
	VECTOR posToAim = VSub(playerAimPos, m_pos);

	//���݈ʒu�ɐݒ肵���|�W�V�����𑫂�
	m_pos = VAdd(m_pos, posToAim);
	
	//VECTOR pos = VSub(m_pos,playerAimPos);
	//pos = VNorm(pos);
	//float size = VSize(pos);


	if (Pad::IsPress PAD_INPUT_6)
	{
		m_angle += 0.05f;
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_angle -= 0.05f;
	}
	
	//m_pos.x = cosf(m_angle) * size;
	//m_pos.x = sinf(m_angle) * size;

	

	





	SetCameraPositionAndTarget_UpVecY(m_pos, player.GetPos());

	//VECTOR pPos;
//pPos.x = cosf(m_angle) * size;
//pPos.y = m_pos.y;
//pPos.z = sinf(m_angle) * size;
//player.SetPos(pPos);
}

/// <summary>
/// �^�C�g���V�[���̃A�b�v�f�[�g
/// </summary>
void Camera::TitleCameraUpdate()
{
#ifdef _DEBUG

	DrawGrid();

#endif
	Pad::Update();
	

	if (Pad::IsPress PAD_INPUT_6)
	{
		m_angle += 0.05f;
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_angle -= 0.05f;
	}

	m_pos.x = cosf(m_angle) * kCameraDist;
	m_pos.y = kCameraHeight;
	m_pos.z = sinf(m_angle) * kCameraDist;
	SetCameraPositionAndTarget_UpVecY(m_pos, VGet(0, 0, 0));
}

void Camera::TargetCameraUpadate()
{

}

void Camera::DrawGrid()
{
	for (int x = -100; x <= 100; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -100), VGet(static_cast<float>(x), 0, 100), 0xffff00);
	}
	for (int z = -100; z <= 100; z += 10)
	{
		DrawLine3D(VGet(-100, 0, static_cast<float>(z)), VGet(100, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-50, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 50));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -50));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);

	DrawFormatString(0, 64, 0xffffff, "Camera(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);


}



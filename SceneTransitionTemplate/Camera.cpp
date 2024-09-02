#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Pad.h"

#include "DxLib.h"
#include <cmath>

#define D2R(deg) ((deg)*DX_PI_F/180.0f)

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
Camera::Camera() :
	m_pos(VGet(0, 10, -25)),
	m_targetPos(VGet(0, 0, 0)),
	m_cameraangle(VGet(0, 0, 0)),
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
	m_cameraangle = VGet(D2R(-20.0f), 0.0f, 0.0f);

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
		m_cameraangle.y += D2R(1.0f);
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_cameraangle.y -= D2R(1.0f);
		//m_angle -= 0.05f;
	}

	//�����_�̍��W���v���C���[�̍��W�ɑ��
	m_targetPos = player.GetPos();

	//��x�N�g��
	VECTOR Direction = VGet(0.0f, 0.0f, 10.0f);

	// �w����]�s��
	MATRIX MatrixX = MGetRotX(m_cameraangle.x);
	// �x����]�s��
	MATRIX MatrixY = MGetRotY(m_cameraangle.y);
	// �s��̍���
	MATRIX Matrix = MMult(MatrixX, MatrixY);
	// ��x�N�g�����s��ŕϊ�
	Direction = VTransform(Direction, Matrix);


	//m_pos.y = cosf(m_angle) * player.GetPos().y;
	//m_pos.y = sinf(m_angle) * player.GetPos().y;









	SetCameraPositionAndTarget_UpVecY(m_pos, m_targetPos);
	//SetCameraPositionAndTarget_UpVecY(m_pos, player.GetPos());

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
	for (int x = -200; x <= 200; x += 10)
	{
		DrawLine3D(VGet(static_cast<float>(x), 0, -200), VGet(static_cast<float>(x), 0, 200), 0xffff00);
	}
	for (int z = -200; z <= 200; z += 10)
	{
		DrawLine3D(VGet(-200, 0, static_cast<float>(z)), VGet(200, 0, static_cast<float>(z)), 0xff0000);
	}

	// X+-,Z+-�̕�����������₷���悤�ɕ\����ǉ�����
	VECTOR dispPos = ConvWorldPosToScreenPos(VGet(200, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(-200, 0, 0));
	DrawStringF(dispPos.x, dispPos.y, "X-", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, 200));
	DrawStringF(dispPos.x, dispPos.y, "Z+", 0xffffff);
	dispPos = ConvWorldPosToScreenPos(VGet(0, 0, -200));
	DrawStringF(dispPos.x, dispPos.y, "Z-", 0xffffff);

	DrawFormatString(0, 64, 0xffffff, "Camera(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);


}

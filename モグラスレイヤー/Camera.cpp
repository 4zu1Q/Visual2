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
	m_cameraAngle(- DX_PI_F / 2)
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
	
	//// �J�����Ɉʒu�𔽉f.
	VECTOR aimPos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 50.0f); // b
	VECTOR posToAim = VSub(aimPos, m_pos);	// (b-a)
	VECTOR scaledPosToAim = VScale(posToAim, 0.5f);	// ((b-a) * t)
	m_pos = VAdd(m_pos, scaledPosToAim);	// a + ((b-a) * t)

	SetCameraPositionAndTarget_UpVecY(m_pos, player.GetPos());

	//VECTOR aimPos = VAdd(player.GetPos(), VScale(player.GetPos(), -30));
	//aimPos


	////�����Ă���Ԑ����p�x���}�C�i�X�ɂ���
	//if (Pad::IsPress PAD_INPUT_LEFT)
	//{
	//	m_cameraAngle -= kAngleSpeed;

	//	if (m_cameraAngle < -DX_PI_F)
	//	{
	//		m_cameraAngle += DX_TWO_PI_F;
	//	}
	//}



	////�����Ă���Ԑ����p�x���v���X�ɂ���
	//if (Pad::IsPress PAD_INPUT_RIGHT)
	//{
	//	m_cameraAngle += kAngleSpeed;

	//	if (m_cameraAngle > DX_PI_F)
	//	{
	//		m_cameraAngle -= DX_TWO_PI_F;
	//	}
	//}

	//�J�����̒����_�̓v���C���[���W����K��l���������W
	//m_targetPos = VAdd(player.GetPos(), VGet(0.0f, kCameraPlayerTargetHeight, 0.0f));




	//SetCameraNearFar(20.0f, 400.0f);
	//m_pos.x = cosf(m_cameraAngle) * kCameraDist;
	//m_pos.y = kCameraHeight;
	////m_cameraPos.z = sinf(m_cameraAngle) * kCameraDist;
	//m_pos.z = -80;
	//SetCameraPositionAndTarget_UpVecY(m_pos, VGet(0, 30, 0));

	//�J�����ƃv���C���[�Ƃ̋���
	






	//VECTOR toCameraPosXZ = m_cameraPos;
//float height = toCameraPosXZ.y;
//toCameraPosXZ.y = 0.0f;
//float toCameraPosXZLen = VSize(toCameraPosXZ);
//VNorm(toCameraPosXZ);

//VECTOR target = m_pPlayer->GetPos();
//target.y += 50.0f;

//VECTOR toNewCameraPos = VSub(m_cameraPos,target);
//toNewCameraPos.y = 0.0f;
//VNorm(toNewCameraPos);

//float weight = 0.7f;	//���̃E�F�C�g�̒l��0.0�`1.0�̒l���Ƃ�B1.0�ɋ߂Â��قǒǔ��������Ȃ�
//toNewCameraPos = VAdd(VScale(toNewCameraPos, weight), VScale(toCameraPosXZ,(1.0f - weight)));
//VNorm(toNewCameraPos);
//toNewCameraPos = VScale(toNewCameraPos, toCameraPosXZLen);
//toNewCameraPos.y = height;
//VECTOR camera = VAdd(target, toNewCameraPos);

//m_cameraPos = camera;
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
		m_cameraAngle += 0.05f;
	}
	if (Pad::IsPress PAD_INPUT_5)
	{
		m_cameraAngle -= 0.05f;
	}

	SetCameraNearFar(20.0f, 400.0f);
	m_pos.x = cosf(m_cameraAngle) * kCameraDist;
	m_pos.y = kCameraHeight;
	//m_cameraPos.z = sinf(m_cameraAngle) * kCameraDist;
	m_pos.z = -160;
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
}

void Camera::FixCameraPosition(Stage& stage)
{

}

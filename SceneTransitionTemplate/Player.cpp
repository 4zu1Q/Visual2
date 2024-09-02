#include "Player.h"
#include "Pad.h"
#include <cmath>
#include <cassert>

/// <summary>
/// �萔
/// </summary>
namespace
{

	//���f���̃t�@�C����
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";

	//���f���p�̒萔
	constexpr float kScale = 10.0f;

	//HP�̃t�@�C����
	const char* const kHpFilename = "data/image/Hp.png";
	const char* const kLostHpFilename = "data/image/LostHp.png";


	//�V�F�[�_�̃t�@�C����
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//�A�j���[�V����
	constexpr int kIdleAnimIndex = 1;		//�ҋ@
	constexpr int kWalkAnimIndex = 2;		//����
	constexpr int kRunAnimIndex = 7;		//����
	constexpr int kJumpAnimIndex = 12;		//�W�����v
	constexpr int kJumpingAnimIndex = 13;		//�W�����v��
	constexpr int kAttackAnimIndex = 30;	//�U��
	constexpr int kDamageAnimIndex = 25;	//�_���[�W
	constexpr int kFallAnimIndex = 26;	//�|���
	constexpr int kFallingAnimIndex = 27;	//�|�ꒆ

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�A�i���O�X�e�B�b�N�ɂ��ړ��֘A
	constexpr float kMaxSpeed = 0.5f;		//�v���C���[�̍ő�ړ����x
	constexpr float kAnalogRangeMin = 0.1;	//�A�i���O�X�e�B�b�N�̓��͔���͈�
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�l

	//�n�[�g�̃A�j���[�V����
	constexpr float kHpSinSpeed = 0.05f;
	constexpr float kHpAnimationSize = 3.0f;


	//
	constexpr float kJumpPower = 10.0f;
	constexpr float kGimmickJumpPower = 40.0f;

}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_modelH(-1),
	m_hpH(LoadGraph(kHpFilename)),
	m_losthpH(LoadGraph(kLostHpFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_cameraAngle(0.0f),
	m_angle(0.0f),
	m_radius(6.0f),
	m_isAttack(false),
	m_isJump(false),
	m_hp(10),
	m_losthp(10),
	m_hpAnimationHeight(0.0f),
	m_animIndex(AnimKind::kNone),
	m_animationIndex(-1)
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	Delete();
}

/// <summary>
/// ���[�h
/// </summary>
void Player::Load()
{
	//���f���̃��[�h
	m_modelH = MV1LoadModel(kPlayerModelFilename);
	assert(m_modelH > -1);

	////�A�j���[�V�����̃u�����h����������
	//m_animBlendRate = 1.0f;

	////������Ԃł̓A�j���[�V�����̓A�^�b�`����Ă��Ȃ��ɂ���
	//m_currentPlayAnim = -1;
	//m_prevPlayAnim = -1;

	//�A�j���[�V�������Đ�


	//�V�F�[�_�̃��[�h
	//ShaderLoad();
}

/// <summary>
/// �f���[�g
/// </summary>
void Player::Delete()
{
	MV1DeleteModel(m_modelH);
	m_modelH = -1;
}

/// <summary>
/// ������
/// </summary>
void Player::Init()
{
	//�ҋ@�A�j���[�V������ݒ�
	m_currentAnimNo = MV1AttachAnim(m_modelH, kIdleAnimIndex, -1, false);
	m_prevAnimNo = -1;
	m_animBlendRate = 1.0f;

	//�v���C���[�̏����ʒu�ݒ�
	m_pos = VGet(0.0f, 0.0f, -30.0f);
	m_attackPos = VAdd(m_pos, VGet(0.0f, 7.0f, 4.0f));

	MV1SetPosition(m_modelH, m_pos);
	MV1SetScale(m_modelH, VGet(kScale, kScale, kScale));

	m_isAttack = false;
}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
void Player::Update()
{
	Pad::Update();

	//�A�j���[�V����
	if (m_prevAnimNo != -1)
	{
		// test 8�t���[���؂�ւ�
		m_animBlendRate += kAnimChangeRateSpeed;
		if (m_animBlendRate >= 1.0f) m_animBlendRate = 1.0f;

		//�ύX��̃A�j���[�V����������ݒ肷��
		MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);
		MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
	}

	//�A�j���[�V������i�߂�
	bool isLoop = UpdateAnim(m_currentAnimNo);
	UpdateAnim(m_prevAnimNo);


	//�W�����v
	if (Pad::IsTrigger PAD_INPUT_1 && m_isJump)
	{
		m_pos.y = kJumpPower;
		m_isJump = false;
	}

	//�{�^������������U���A�j���[�V�������Đ�����
	if (!m_isAttack)
	{

		if (Pad::IsTrigger PAD_INPUT_3)
		{

			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);


		}
		else
		{
			//�A�i���O�X�e�B�b�N���g���Ĉړ�
			int analogX = 0;
			int analogZ = 0;

			GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);

			//�A�i���O�X�e�B�b�N�̓��͂�10% ~ 80%�͈̔͂��g�p����

			//�x�N�g���̒������ő��1000�ɂȂ�
			//�v���C���[�̍ő�ړ����x��0.01f / frame

			VECTOR move = VGet(analogX, 0.0f, -analogZ);	//�x�N�g���̒�����0�`1000

			//0.00 ~ 0.01�̒����ɂ���
			//�x�N�g���̒������擾����
			float len = VSize(move);
			//�x�N�g���̒�����0.0~1.0�̊����ɕϊ�����
			float rate = len / kAnalogInputMax;

			//�A�i���O�X�e�B�b�N�����Ȕ͈͂����O����
			rate = (rate - kAnalogRangeMin) / (kAnalogRangeMax - kAnalogRangeMin);
			rate = min(rate, 1.0f);
			rate = max(rate, 0.0f);

			//���x������ł���̂ňړ��x�N�g���ɔ��f����
			move = VNorm(move);
			float speed = kMaxSpeed * rate;
			move = VScale(move, speed);

			//�J�����̂���ꏊ(�p�x)����
			//�R���g���[���[�ɂ��ړ����������肷��
			MATRIX mtx = MGetRotY(-m_cameraAngle - DX_PI_F / 2);
			move = VTransform(move, mtx);

			//�ړ���������v���C���[�̌������������肷��
			//�ړ����Ă��Ȃ��ꍇ(�[���x�N�g��)�̏ꍇ�͕ύX���Ȃ�
			if (VSquareSize(move) > 0.0f)
			{
				m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
			}

			m_pos = VAdd(m_pos, move);

			//B�{�^���������Ă����
			if (Pad::IsPress(PAD_INPUT_1))
			{
				ChangeAnim(kRunAnimIndex);

				//�����X�s�[�h��1.2�{
				move = VScale(move, 1.2f);
				//�����𔽉f
				m_pos = VAdd(m_pos, move);
			}

			VECTOR attackMove = VScale(move, 15.0f);
			m_attackPos = VAdd(m_pos, attackMove);
		}
	}
	else
	{
		//�U���A�j���[�V�������I��������ҋ@�A�j���[�V�������Đ�����
		if (isLoop)
		{
			m_isAttack = false;
			ChangeAnim(kIdleAnimIndex);
		}

	}

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));

	//ShaderUpdate();

	static float SinCount = 0;
	SinCount += kHpSinSpeed;
	m_hpAnimationHeight = sinf(SinCount) * kHpAnimationSize;



}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{
	MV1DrawModel(m_modelH);


#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xff00ff, true);

	DrawFormatString(0, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);

	DrawFormatString(400, 16, 0xffffff, "PlayerHp:%d", m_hp);

#endif

	for (int i = 1; i <= m_losthp; i++)
	{
		DrawExtendGraph(35 * i, 16 + m_hpAnimationHeight, 35 * i + 32, 48, m_losthpH, true);

	}

	for (int i = 1; i <= m_hp; i++)
	{
		DrawExtendGraph(35 * i, 16 + m_hpAnimationHeight, 35 * i + 32, 48, m_hpH, true);
	}

}

/// <summary>
/// �M�~�b�N�ɓ����������̃A�b�v�f�[�g����
/// </summary>
void Player::OnGimmickHitUpdate()
{
	Pad::Update();

	int frame = 0;
	m_isJump = true;

	if (m_isJump)
	{
		m_pos.y = kGimmickJumpPower;
	}

	if (Pad::IsTrigger PAD_INPUT_1)
	{


	}


}

/// <summary>
/// �A�j���[�V�����̃A�b�v�f�[�g����
/// </summary>
/// <param name="attachNo"></param>
/// <returns></returns>
bool Player::UpdateAnim(int attachNo)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelH, attachNo);	//���݂̍Đ��J�E���g���擾
	bool isLoop = false;
	now += 0.5f;	//�A�j���[�V������i�߂�

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_modelH, attachNo);

	while (now >= total)
	{
		now -= total;
		isLoop = true;
	}

	//�i�߂����Ԃɐݒ�
	MV1SetAttachAnimTime(m_modelH, attachNo, now);

	return isLoop;
}

/// <summary>
/// �A�j���[�V������ύX����֐�
/// </summary>
/// <param name="animIndex"></param>
void Player::ChangeAnim(int animIndex)
{
	//����ɌÂ��A�j���[�V�������A�^�b�`����Ă���ꍇ�͂��̎��_�ō폜���Ă���
	if (m_prevAnimNo != -1)
	{
		MV1DetachAnim(m_modelH, m_prevAnimNo);
	}


	//���ݍĐ����̑ҋ@�A�j���[�V�����͕ύX�O�̃A�j���[�V����������
	m_prevAnimNo = m_currentAnimNo;

	//�ύX��̃A�j���[�V�����Ƃ��čU���A�j���[�V���������߂Đݒ肷��
	m_currentAnimNo = MV1AttachAnim(m_modelH, animIndex, -1, false);

	//�؂�ւ��̏u�Ԃ͕ύX�O�̃A�j���[�V�������Đ�������Ԃɂ���
	m_animBlendRate = 0.0f;

	//�ύX�O�̃A�j���[�V����100%
	MV1SetAttachAnimBlendRate(m_modelH, m_prevAnimNo, 1.0f - m_animBlendRate);

	//�ύX��̃A�j���[�V����0%
	MV1SetAttachAnimBlendRate(m_modelH, m_currentAnimNo, m_animBlendRate);
}

/// <summary>
/// �V�F�[�_�����[�h����֐�
/// </summary>
void Player::ShaderLoad()
{
	/*�V�F�[�_*/
	//������V�F�[�_���Ăяo�����Ăяo���Ȃ���
	MV1SetUseOrigShader(true);

	//�V�F�[�_�̃��[�h
	m_outlineVsH = LoadVertexShader(kOutlineVsFilename);
	m_outlinePsH = LoadPixelShader(kOutlinePsFilename);
}

/// <summary>
/// �V�F�[�_�̃A�b�v�f�[�g����
/// </summary>
void Player::ShaderUpdate()
{
	//���b�V���̐����擾
	auto m_meshNum = MV1GetMeshNum(m_modelH);

	/*���]�@*/
	SetUseVertexShader(m_outlineVsH);
	SetUsePixelShader(m_outlinePsH);
	MV1DrawModel(m_modelH);


}

void Player::PlayAnim(AnimKind playAnim)
{
	if (m_prevPlayAnim != -1)
	{
		MV1DetachAnim(m_modelH, m_prevPlayAnim);
		m_prevPlayAnim = -1;
	}

	//���܂ōĐ����̃��[�V�������������̂̏���Prev�Ɉړ�����
	m_prevPlayAnim = m_currentPlayAnim;
	m_prevAnimCount = m_currentAnimCount;

	//�V���Ɏw��̃��[�V���������f���ɃA�^�b�`���āA�A�^�b�`�ԍ���ۑ�����
	m_currentPlayAnim = MV1AttachAnim(m_modelH, static_cast<int>(playAnim));
	m_currentAnimCount = 0.0f;

	//�u�����h����Prev���L���ł͂Ȃ��ꍇ��1.0f(���݃��[�V������100%�̏��)�ɂ���
	m_animBlendRate = m_prevPlayAnim == -1 ? 1.0f : 0.0f;

}

void Player::AttackCol(VECTOR pos)
{
}
#include "Player.h"
#include "Pad.h"
#include "Game.h"
#include <cmath>
#include <cassert>
#include <EffekseerForDXLib.h>

/// <summary>
/// �萔
/// </summary>
namespace
{

	//���f���̃t�@�C����
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";

	//���f���̌����Ă�ʒu�̏�����
	constexpr float kInitAngle = 3.143059f;

	//���f���p�̒萔
	constexpr float kHitEffectScale = 10.0f;

	//HP�̃t�@�C����
	const char* const kHpFilename = "data/image/Hp.png";
	const char* const kLostHpFilename = "data/image/LostHp.png";
	const char* const kHpFlameFilename = "data/image/HpFlame.png";

	//�T�E���h�̃t�@�C����
	const char* const kAttackFilename = "data/sound/se/AxeSlashSe.mp3";
	const char* const kSkillFilename = "data/sound/se/AxeSkillSe.mp3";
	const char* const kDamageFilename = "data/sound/se/DamageSe.mp3";

	//�V�F�[�_�̃t�@�C����
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//�A�j���[�V����
	constexpr int kIdleAnimIndex = 1;		//�ҋ@
	constexpr int kWalkAnimIndex = 2;		//����
	constexpr int kDashAnimIndex = 7;		//����
	constexpr int kAttackAnimIndex = 31;	//�U��
	constexpr int kSkillAnimIndex = 41;	//�X�L��
	constexpr int kDamageAnimIndex = 25;	//�_���[�W
	constexpr int kFallAnimIndex = 26;		//�|���
	constexpr int kFallingAnimIndex = 27;	//�|�ꒆ
	constexpr int kGameClearAnimIndex = 10;	//�|������̃��[�V����
	constexpr int kGameOverAnimIndex = 10;

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�_���[�W
	constexpr int kDamageCount = 60;


	//�A�i���O�X�e�B�b�N�ɂ��ړ��֘A
	constexpr float kMaxSpeed = 0.5f;		//�v���C���[�̍ő�ړ����x
	constexpr float kAnalogRangeMin = 0.1;	//�A�i���O�X�e�B�b�N�̓��͔���͈�
	constexpr float kAnalogRangeMax = 0.8;
	constexpr float kAnalogInputMax = 1000.0f;	//�A�i���O�X�e�B�b�N������͂����x�N�g���̍ő�l

	//�n�[�g�̃A�j���[�V����
	constexpr float kHpSinSpeed = 0.05f;
	constexpr float kHpAnimationSize = 3.0f;

	constexpr float kMaxStamina = 400.0f;

	//��
	constexpr float kWall = 475;

	constexpr float kSelectSpeed = 3.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Player::Player() :
	m_modelH(-1),
	m_pos(VGet(0, 0, 0)),
	m_attackPos(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_shadowMapH(-1),
	m_hpH(LoadGraph(kHpFilename)),
	m_losthpH(LoadGraph(kLostHpFilename)),
	m_hpFlameH(LoadGraph(kHpFlameFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_cameraAngle(0.0f),
	m_angle(kInitAngle),
	m_radius(7.0f),
	m_attackRadius(15.0f),
	m_skillRadius(10.0f),
	m_isWalk(false),
	m_isDamage(false),
	m_isDash(false),
	m_isAttack(false),
	m_isSkill(false),
	m_isDown(false),
	m_isAttackGeneration(false),
	m_isSkillGeneration(false),
	m_isAnimWalk(false),
	m_isAnimDash(false),
	m_isAnimDamage(false),
	m_isAnimDown(false),
	m_isStamina(false),
	m_isAvoid(false),
	m_isCommand(false),
	m_isAnimIdle(false),
	m_damageFrame(0),
	m_hp(10),
	m_losthp(10),
	m_stamina(kMaxStamina),
	m_hpAnimationHeight(0.0f),
	m_animKind(AnimKind::kIdle),
	m_analogX(0.0f),
	m_analogZ(0.0f),
	m_damageSeH(-1),
	m_axeAttackSeH(-1),
	m_axeSkillSeH(-1),
	m_shadowAngle(0.0f),
	m_effectPH(-1),
	m_effectSH(-1),
	m_lightDirection(VGet(0, 0, 0))
{
	m_damageSeH = LoadSoundMem(kDamageFilename);	  //�_���[�W��
	m_axeAttackSeH = LoadSoundMem(kAttackFilename);	  //�U����
	m_axeSkillSeH = LoadSoundMem(kSkillFilename);	  //�X�L����

	//m_effectSH = LoadEffekseerEffect("");

	ChangeVolumeSoundMem(128, m_damageSeH);
	ChangeVolumeSoundMem(128, m_axeAttackSeH);
	ChangeVolumeSoundMem(128, m_axeSkillSeH);

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Player::~Player()
{
	Delete();
	ShadowMapDelete(m_modelH);

	DeleteSoundMem(m_damageSeH);
	DeleteSoundMem(m_axeAttackSeH);
	DeleteSoundMem(m_axeSkillSeH);

}

/// <summary>
/// ���[�h
/// </summary>
void Player::Load()
{
	//���f���̃��[�h
	m_modelH = MV1LoadModel(kPlayerModelFilename);
	assert(m_modelH > -1);

	//�V���h�E�}�b�v���f�������[�h
	//ShadowMapLoad();

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
	m_pos = VGet(0.0f, 0.0f, -170.0f);

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
	MV1SetScale(m_modelH, VGet(kHitEffectScale, kHitEffectScale, kHitEffectScale));

	//�V���h�E�}�b�v�̏�����
	//ShadowMapInit();

}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
void Player::Update()
{
	Pad::Update();

	//ShadowMapUpdate();

	if (m_stamina >= kMaxStamina)
	{
		m_stamina = kMaxStamina;
	}

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
	if (isLoop)
	{
		ChangeAnim(m_animIndex);
	}
	UpdateAnim(m_prevAnimNo);

	//�����ꂽ��
	if (m_isAvoid)
	{

	}

	//���񂾂珈��
	if (m_isDown)
	{
		if (!m_isAnimDown)
		{
			ChangeAnim(kFallAnimIndex);
			m_animIndex = kFallingAnimIndex;
		}
		m_isAnimDown = true;

	}

	if (m_isCommand)
	{
		if (!m_isAnimIdle)
		{
			ChangeAnim(kGameClearAnimIndex);
			m_animIndex = kGameClearAnimIndex;
		}
		m_isAnimIdle = true;
	}


	//�{�^������������U���A�j���[�V�������Đ�����
	if (!m_isAttack && !m_isSkill && !m_isDown && !m_isCommand)
	{
		//�U��
		if (Pad::IsPress(PAD_INPUT_3) && !m_isStamina)
		{
			
			m_isAttack = true;
			ChangeAnim(kAttackAnimIndex);
			m_isMove = true;
			PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//�A�^�b�N
		}
		else
		{
			if (m_isMove)
			{
				Move();
			}
		}

		//�X�L���U��
		if (Pad::IsPress(PAD_INPUT_4) && m_stamina >= 100.0f && !m_isStamina)
		{
			m_isSkill = true;
			ChangeAnim(kSkillAnimIndex);
			m_isMove = false;
			PlaySoundMem(m_axeSkillSeH, DX_PLAYTYPE_BACK, true);//�X�L��
			m_stamina -= 100.0f;
		}
		else
		{
			if (!m_isMove)
			{
				Move();
			}
		}

	}
	else
	{

		//�U���A�j���[�V�������I��������ҋ@�A�j���[�V�������Đ�����
		if (isLoop)
		{
			m_isAttack = false;
			m_isSkill = false;
			m_isDash = false;
			m_isDown = false;
		}
		
	}


	

	//ShaderUpdate();

	//Hp�̃A�j���[�V����
	static float SinCount = 0;
	SinCount += kHpSinSpeed;
	m_hpAnimationHeight = sinf(SinCount) * kHpAnimationSize;


	//�ړ��͈�
	if (m_pos.x >= kWall) m_pos.x = kWall;
	if (m_pos.x <= -kWall) m_pos.x = -kWall;
	if (m_pos.z >= kWall) m_pos.z = kWall;
	if (m_pos.z <= -kWall) m_pos.z = -kWall;
	
	//�_���[�W�_�Ŏ���
	if (m_isDamage)
	{
		m_damageFrame++;
		if (m_damageFrame > kDamageCount)
		{
			m_isDamage = false;
			m_damageFrame = 0;
		}
	}

	//int soundAttackFrame = 0;
	//int soundSkillFrame = 0;

	if (m_isAttack)
	{
		//soundAttackFrame++;
		//if (soundAttackFrame == 5)
		//{
			PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//�A�^�b�N

		//}

		//�v���C���[���U�������琶�����邽�߂̃t���O��true�ɂȂ�
		m_isAttackGeneration = true;
	}
	else m_isAttackGeneration = false;

	if (m_isSkill)
	{
		//soundSkillFrame++;
		//if (soundSkillFrame == 30)
		//{
			PlaySoundMem(m_axeAttackSeH, DX_PLAYTYPE_BACK, true);//�A�^�b�N

		//}
		//�v���C���[���X�L�����g�p�����琶�����邽�߂̃t���O��true�ɂȂ�
		m_isSkillGeneration = true;
	}
	else m_isSkillGeneration = false;

	//HP���[����艺�ɂ������ꍇ
	if (m_hp <= 0)
	{
		m_hp = 0;
	}


	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0, m_angle, 0));
}

/// <summary>
/// �`��
/// </summary>
void Player::Draw()
{


	DrawExtendGraph(10, 10, 420, 50, m_hpFlameH,true);

	for (int i = 1; i <= m_losthp; i++)
	{
		DrawExtendGraph(35 * i + 20, 16 + m_hpAnimationHeight, 35 * i + 52, 48, m_losthpH, true);

	}

	for (int i = 1; i <= m_hp; i++)
	{
		DrawExtendGraph(35 * i + 20, 16 + m_hpAnimationHeight, 35 * i + 52, 48, m_hpH, true);
	}

	DrawBox(19, 54, 22 + kMaxStamina, 66, 0xdc143c, true);

	float alpha = 0;
	alpha += kSelectSpeed;


	if (!m_isStamina)
	{
		DrawBox(20, 55, 20 + m_stamina, 65, 0x3cb371, true);
	}
	else
	{
		DrawBox(20, 55, 20 + m_stamina, 65, 0xffa500, true);
	}
	


#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	//�����蔻�肪��������Ă��邩�ǂ���
	if (!m_isSkill)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_skillRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_skillRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (!m_isAttack)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_attackRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_attackRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	DrawFormatString(0, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(800, 16, 0xffffff, "Player(x:%f,y:%f,z:%f)", m_attackPos.x, m_attackPos.y, m_attackPos.z);

	DrawFormatString(400, 16, 0xffffff, "PlayerHp:%d", m_hp);
	DrawFormatString(400, 48, 0xffffff, "AttackFlag:%f", m_isSkill);
	DrawFormatString(400, 64, 0xffffff, "PlayerAngle:%f", m_angle);


	DrawFormatString(400, 96, 0xffffff, "PlayerLeftAngle(x:%f,z:%f)", m_analogX, m_analogZ);


#endif
	// �_���[�W���o  2�t���[���Ԋu�ŕ\����\���؂�ւ�
	// 0: �\�������
	// 1: �\�������
	// 2: ��\��
	// 3: ��\��
	// 4: �\�������	...
	// % 4 ���邱�Ƃ�012301230123... �ɕϊ�����
	if (m_damageFrame % 8 >= 4) return;


	// �`��ɃL�����N�^�[���f���p�̃V���h�E�}�b�v��
	// �X�e�[�W���f���p�̃V���h�E�}�b�v�̂ǂ�����g�p����
	//SetUseShadowMap(0, m_modelH);

	////�V���h�E�}�b�v�ւ̕`��̏���
	//ShadowMap_DrawSetup(m_shadowMapH);

	MV1DrawModel(m_modelH);

	////�V���h�E�}�b�v�̐ݒ������
	//SetUseShadowMap(0, -1);
}

/// <summary>
/// �M�~�b�N�ɓ����������̃A�b�v�f�[�g����
/// </summary>
void Player::OnGimmickHitUpdate()
{
	Pad::Update();

	int frame = 0;


}

void Player::SetAnimDamage(const bool animDamage)
{
	m_isAnimDamage = animDamage;
	ChangeAnim(kDamageAnimIndex);
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
	now += 0.7f;	// �A�j���[�V������i�߂�

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

/// <summary>
/// �v���C���[�̈ړ��֐�
/// </summary>
void Player::Move()
{

	//�A�i���O�X�e�B�b�N���g���Ĉړ�
	int analogX = 0;
	int analogZ = 0;

	GetJoypadAnalogInput(&analogX, &analogZ, DX_INPUT_PAD1);
	//�A�i���O�X�e�B�b�N�̓��͂�10% ~ 80%�͈̔͂��g�p����

	//�x�N�g���̒������ő��1000�ɂȂ�
	//�v���C���[�̍ő�ړ����x��0.01f / frame

	VECTOR move = VGet(analogX, 0.0f, -analogZ);	//�x�N�g���̒�����0�`1000

	m_analogX = analogX;
	m_analogZ = analogZ;

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

	if (VSize(move) != 0.0f)
	{
		if (!m_isWalk)
		{
			ChangeAnim(kWalkAnimIndex);
			m_animIndex = kWalkAnimIndex;
		}
		m_isWalk = true;
	}
	else
	{
		m_isWalk = false;
	}

	//�ړ���������v���C���[�̌������������肷��
	//�ړ����Ă��Ȃ��ꍇ(�[���x�N�g��)�̏ꍇ�͕ύX���Ȃ�
	if (VSquareSize(move) > 0.0f)
	{
		m_angle = -atan2f(move.z, move.x) - DX_PI_F / 2;
		m_attackDir = VNorm(move);

	}

	m_pos = VAdd(m_pos, move);


	//A�{�^���������Ă����
	if (!m_isStamina)
	{
		if (Pad::IsPress(PAD_INPUT_1) && m_stamina >= 0.0f && VSize(move) != 0.0f)
		{
			//�����X�s�[�h��1.5�{
			move = VScale(move, 2.0f);

			m_stamina--;

			if (m_stamina <= 0.0f)
			{
				m_isStamina = true;
			}

			//�����𔽉f
			m_pos = VAdd(m_pos, move);
			if (!m_isDash)
			{
				ChangeAnim(kDashAnimIndex);
				m_animIndex = kDashAnimIndex;
			}
			m_isDash = true;
		}
		else
		{
			m_stamina++;

			if (m_isDash && m_isWalk)
			{
				ChangeAnim(kWalkAnimIndex);
				m_animIndex = kWalkAnimIndex;
			}
			m_isDash = false;
		}
	}
	else
	{
		m_isDash = false;

		m_stamina += 0.5f;

		if (!m_isDash && !m_isWalk)
		{
			ChangeAnim(kWalkAnimIndex);
			m_animIndex = kWalkAnimIndex;
		}
		m_isWalk = true;

		if (m_stamina == kMaxStamina)
		{
			m_isStamina = false;
		}

	}


	if (!m_isDash && !m_isWalk)
	{
		ChangeAnim(kIdleAnimIndex);
		m_animIndex = kIdleAnimIndex;
	}


	VECTOR attackMove = VScale(m_attackDir, 15.0f);
	m_attackPos = VAdd(m_pos, attackMove);
}

/// <summary>
/// �V���h�E�}�b�v�̃��[�h
/// </summary>
void Player::ShadowMapLoad()
{
	m_shadowMapH = MakeShadowMap(1024, 1024);
}
/// <summary>
/// �V���h�E�}�b�v�̏�����
/// </summary>
void Player::ShadowMapInit()
{
	SetShadowMapDrawArea(m_shadowMapH, VGet(-1000.0f, -1.0f, -1000.0f), VGet(1000.0f, 1000.0f, 1000.0f));
	m_shadowAngle = 0.0f;
}
/// <summary>
/// �V���h�E�}�b�v�̃A�b�v�f�[�g
/// </summary>
void Player::ShadowMapUpdate()
{
	m_shadowAngle += 0.0125f;
	if (m_shadowAngle > DX_PI_F * 2.0f)
	{
		m_shadowAngle -= DX_PI_F * 2.0f;
	}

	//���C�g�̕����x�N�g���̎Z�o
	m_lightDirection.x = sin(m_shadowAngle);
	m_lightDirection.y = -1.0f;
	m_lightDirection.z = cos(m_shadowAngle);

	//���C�g�̕�����ݒ�
	SetLightDirection(m_lightDirection);

	//�V���h�E�}�b�v���z�肷�郉�C�g�̕������Z�b�g
	SetShadowMapLightDirection(m_shadowMapH, m_lightDirection);



}
/// <summary>
/// �V���h�E�}�b�v�̕`��
/// </summary>
void Player::ShadowMapDraw(int handle)
{

}
/// <summary>
/// �V���h�E�}�b�v�̃f���[�g
/// </summary>
void Player::ShadowMapDelete(int handle)
{
	// �V���h�E�}�b�v�̍폜
	DeleteShadowMap(handle);
	m_shadowMapH = -1;
}




#include "Enemy.h"
#include "Player.h"
#include <cmath>

#include <EffekseerForDXLib.h>
#include <list>

/// <summary>
/// �萔
/// </summary>
namespace
{
	//���f���̃t�@�C����
	const char* const kEnemyModelFilename = "data/model/enemy/Enemy.mv1";
	const char* const kWeponModelFilename = "data/model/enemy/Skeleton_Blade.mv1";
	const char* const kHpFlameFilename = "data/image/EnemyHpFlame.png";

	//�V�F�[�_�̃t�@�C����
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//�A�j���[�V����
	constexpr int kIdleAnimIndex = 42;		//�ҋ@
	constexpr int kDashAnimIndex = 55;		//����
	constexpr int kAttackAnimIndex = 5;	//�U��
	constexpr int kSkillAnimIndex = 12;	//�X�L��
	constexpr int kDamageAnimIndex = 25;	//�_���[�W
	constexpr int kFallAnimIndex = 27;		//�|���
	constexpr int kFallingAnimIndex = 28;	//�|�ꒆ

	//�A�j���[�V�����̐؂�ւ��ɂ�����t���[����
	constexpr float kAnimChangeFrame = 8.0f;
	constexpr float kAnimChangeRateSpeed = 1.0f / kAnimChangeFrame;

	//�����蔻��
	constexpr float kAddPosY = 8.0f;

	//�_���[�W
	constexpr int kDamageCount = 40;

	//�X�s�[�h
	constexpr float kSpeed = 0.5f;

	constexpr float kWall = 475;

	constexpr int kMaxHp = 500;

	constexpr int kMax = 2;
	constexpr int kHalf = 5;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy() :
	m_modelH(-1),
	m_modelWeponH(-1),
	m_hpFlameH(LoadGraph(kHpFlameFilename)),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_sH(-1),
	m_pH(-1),
	m_radius(15.0f),
	m_skillRadius(43.0f),
	m_searchRadius(300.0f),
	m_stopRadius(40.0f),
	m_hp(kMaxHp),
	m_pos(VGet(0, 0, 0)),
	m_attackPos(VGet(0,0,0)),
	m_velocity(VGet(0, 0, 0)),
	m_direction(VGet(0, 0, 0)),
	m_attackDir(VGet(0, 0, 0)),
	m_dirPos(VGet(0, 0, 0)),
	m_damageFrame(0),
	m_isEffect(false),
	m_isDamage(false),
	m_isIdleAnim(false),
	m_isRunAnim(false),
	m_isAttackAnim(false),
	m_isSkillAnim(false),
	m_isDamageAnim(false),
	m_isDeadAnim(false),
	m_isDeadingAnim(false),
	m_isAttackGeneration(false),
	m_isSkillGeneration(false),
	m_isRand(false),
	m_isMove(false),
	m_state(kRun),
	m_angle(0.0f),
	m_frame(0),
	m_attackFrame(0),
	m_currentAnimNo(-1),
	m_prevAnimNo(-1),
	m_animBlendRate(0.0f),
	m_random(0),
	m_rand(0),
	m_workFrame(0),
	m_animIndex(kDashAnimIndex),
	m_attachFramePosition(VGet(0,0,0))
{

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Enemy::~Enemy()
{
	Delete();
}

/// <summary>
/// ���[�h
/// </summary>
void Enemy::Load()
{
	//���f���̃��[�h
	//m_modelHandle[0] = MV1LoadModel(kEnemyModelFilename);
	m_modelH = MV1LoadModel(kEnemyModelFilename);
	m_modelWeponH = MV1LoadModel(kWeponModelFilename);

	//�G�t�F�N�g�����[�h
	m_sH = LoadEffekseerEffect("data/effect/syou1.efkefc");

	//ShaderLoad();
}

/// <summary>
/// �f���[�g
/// </summary>
void Enemy::Delete()
{
	MV1DeleteModel(m_modelH);
	MV1DeleteModel(m_modelWeponH);
	m_modelH = -1;
	m_modelWeponH = -1;

	//�G�t�F�N�g���f���[�g
	DeleteEffekseerEffect(m_sH);

}

/// <summary>
/// ������
/// </summary>
void Enemy::Init()
{
	m_pos = VGet(-60.0f, 0.0f, 0.0f);
	m_attackPos = VGet(m_pos.x, m_pos.y, m_pos.z - 10);
	m_animIndex = kDashAnimIndex;

	MV1SetPosition(m_modelH, m_pos);
	//�G�̃X�P�[��
	MV1SetScale(m_modelH, VGet(20, 20, 20));
	MV1SetScale(m_modelWeponH, VGet(0.1, 0.1, 0.1));



}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
void Enemy::Update(std::shared_ptr<Player> pPlayer)
{

	//�G�t�F�N�g�̃A�b�v�f�[�g����
	UpdateEffekseer3D();



	//����̃A�^�b�`
	MATRIX transMat;
	MATRIX attachFrameMat;
	MATRIX mixMatrix;

	//�A�^�b�`���郂�f����MV1SetMatrix�̐ݒ�𖳌��ɂ���
	MV1SetMatrix(m_modelWeponH, MGetIdent());

	m_attachFramePosition = MV1GetFramePosition(m_modelWeponH, 0);

	transMat = MGetTranslate(VScale(m_attachFramePosition, -1.0f));

	attachFrameMat = MV1GetFrameLocalWorldMatrix(m_modelH, 14);

	mixMatrix = MMult(transMat, attachFrameMat);

	MV1SetMatrix(m_modelWeponH, mixMatrix);


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
	bool isLoop = UpdateAnim(m_currentAnimNo, m_isDeadAnim);
	if (isLoop)
	{
		ChangeAnim(m_animIndex);
	}
	UpdateAnim(m_prevAnimNo);

	if (!m_isRunAnim)
	{
		ChangeAnim(kDashAnimIndex);
	}
	m_animIndex = kDashAnimIndex;



//#if 0
	if (m_state == kDeath)
	{
		if (!m_isDeadAnim)
		{
			ChangeAnim(kFallAnimIndex);
			m_animIndex = kFallingAnimIndex;
			m_isDeadAnim = true;
		}

	}
	else
	{

		//�v���C���[�ւ̌������擾
		m_direction = VSub(pPlayer->GetPos(), m_pos);
		m_direction = VNorm(m_direction);


		if (m_state == kIdle)			//�~�܂��Ă�����
		{
			if (!m_isIdleAnim)
			{
				ChangeAnim(kIdleAnimIndex);
			}
			m_isIdleAnim = true;

			m_animIndex = kIdleAnimIndex;
		}
		else
		{
			m_isIdleAnim = false;
		}

		if (!m_isAttackAnim && !m_isSkillAnim)
		{

			if (m_state == kRun)		//�v���C���[��ǂ��Ă�����
			{


				m_angle = atan2f(m_direction.x, m_direction.z);

				m_animIndex = kDashAnimIndex;

				//�x�N�g�����A���K�����A����������ۑ�������
				m_velocity = VScale(m_direction, kSpeed);

				//�G�̈ړ�
				m_pos = VAdd(m_pos, m_velocity);

				if (!m_isRunAnim)
				{
					ChangeAnim(kDashAnimIndex);
				}
				m_isRunAnim = true;
			}
		}



		if (m_state == kAttack)	//�U���̏��
		{

			if (!m_isRand)
			{
				m_rand = GetRand(2);
				m_isRand = true;
			}




			if (!m_isAttackAnim && !m_isSkillAnim)
			{

				m_attackFrame++;

				if (m_attackFrame > 150)
				{
					if (m_rand <= 1)
					{
						ChangeAnim(kAttackAnimIndex);
						m_isAttackAnim = true;
						m_isRand = false;
						m_isMove = true;
					}
					else
					{
						ChangeAnim(kSkillAnimIndex);
						m_isSkillAnim = true;
						m_isRand = false;
						m_isMove = true;
					}

					m_attackFrame = 0;
				}



			}
			else
			{
				if (isLoop)
				{

					m_isAttackAnim = false;
					m_isSkillAnim = false;

					m_isAttackGeneration = false;
					m_isSkillGeneration = false;
				}
			}


		}
		else
		{
			m_isIdleAnim = false;
			m_isMove = false;
		}

		//�A�j���[�V�������I������瓖���蔻��̐�������߂�
		if (m_isAttackAnim && !m_isSkillAnim)
		{
			m_isAttackGeneration = true;
		}
		else
		{
			m_isAttackGeneration = false;
		}
		//�A�j���[�V�������I������瓖���蔻��̐�������߂�
		if (m_isSkillAnim && !m_isAttackAnim)
		{
			m_isSkillGeneration = true;
		}
		else
		{
			m_isSkillGeneration = false;
		}
	}
//#endif // 0

#if 0

	m_isRunAnim = true;


	//�v���C���[�ւ̌������擾
	m_direction = VSub(pPlayer->GetPos(), m_pos);
	m_direction = VNorm(m_direction);

	m_angle = atan2f(m_direction.x, m_direction.z);


	//�x�N�g�����A���K�����A����������ۑ�������
	m_velocity = VScale(m_direction, kSpeed);

	if (m_state != kAttack)
	{
		//�G�̈ړ�
		m_pos = VAdd(m_pos, m_velocity);
	}

	VECTOR Pos = VAdd(m_pos, pPlayer->GetPos());

	if (m_state == kAttack)
	{
		m_isIdleAnim = true;
		if (!m_isAttackAnim && !m_isSkillAnim)
		{

			m_attackFrame++;

			if (m_attackFrame > 150)
			{
				if (m_rand <= 1)
				{
					ChangeAnim(kAttackAnimIndex);
					m_isAttackAnim = true;
					m_isRand = false;
					m_isMove = true;
				}
				else
				{
					ChangeAnim(kSkillAnimIndex);
					m_isSkillAnim = true;
					m_isRand = false;
					m_isMove = true;
				}

				m_attackFrame = 0;
			}



		}
		else
		{
			if (isLoop)
			{

				m_isAttackAnim = false;
				m_isSkillAnim = false;

				m_isAttackGeneration = false;
				m_isSkillGeneration = false;
			}
		}
	}

#endif // 0

	

	
	if (m_isDeadAnim)
	{
		m_frame++;
	}




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


	if (!m_isAttackAnim && !m_isSkillAnim)
	{
		m_angle = atan2f(m_direction.x, m_direction.z);

		VECTOR m_attackDir = VScale(m_direction, 28.0f);
		m_attackPos = VAdd(m_pos, m_attackDir);
	}

	//HP���}�C�i�X�ɂ����Ȃ�����
	if (m_hp <= 0) m_hp = 0;
	

	if (m_frame >= 150)
	{
		if (!m_isEffect)
		{
			m_pH = PlayEffekseer3DEffect(m_sH);
			SetScalePlayingEffekseer3DEffect(m_pH, 10, 10, 10);
			SetPosPlayingEffekseer3DEffect(m_pH, m_pos.x, m_pos.y, m_pos.z);
			m_isEffect = true;
		}

	}
	

	MV1SetPosition(m_modelH, m_pos);
	MV1SetRotationXYZ(m_modelH, VGet(0.0f, m_angle + DX_PI_F, 0.0f));
}

/// <summary>
/// �`��
/// </summary>
void Enemy::Draw()
{
	// �J�����̂RD�������킹�Ă���
	Effekseer_Sync3DSetting();
	// �G�t�F�N�V�A�̏����X�V���Ă���
	UpdateEffekseer3D();
	// �G�t�F�N�V�A��`�悵�Ă���
	DrawEffekseer3D();


	DrawBox(700, 13, 700 + kMaxHp, 50, 0xdc143c, true);
	DrawBox(700, 13, 700 + m_hp, 50, 0x3cb371, true);
	DrawExtendGraph(620, 10,1205, 50, m_hpFlameH, true);

#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_searchRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_stopRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xf00fff, 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Enemy(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(400, 32, 0xffffff, "EnemyHp:%d", m_hp);
	DrawFormatString(400, 332, 0xffffff, "EnemyState:%d", m_state);
	DrawFormatString(400, 352, 0xffffff, "EnemyRand:%d", m_rand);

	DrawFormatString(400, 382, 0xffffff, "EnemyAttack:%d", m_isAttackAnim);
	DrawFormatString(400, 412, 0xffffff, "EnemySkill:%d", m_isSkillAnim);


	if (!m_isAttackAnim)
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (!m_isSkillAnim)
	{
		DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_skillRadius, 8, 0xff00ff, 0xff00ff, false);
	}
	else
	{
		DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_skillRadius, 8, 0x0000ff, 0x0000ff, false);
	}

	if (m_state == kDeath)
	{

	}

#endif

	// �_���[�W���o  2�t���[���Ԋu�ŕ\����\���؂�ւ�
// 0: �\�������
// 1: �\�������
// 2: ��\��
// 3: ��\��
// 4: �\�������	...
// % 4 ���邱�Ƃ�012301230123... �ɕϊ�����
	if (m_damageFrame % 8 >= 4) return;

	if (m_frame <= 150)
	{
		MV1DrawModel(m_modelWeponH);
		MV1DrawModel(m_modelH);
	}


	
}

/// <summary>
/// ���̓����蔻��
/// </summary>
bool Enemy::SphereHitFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

/// <summary>
/// 
/// </summary>
/// <param name="pPlayer"></param>
/// <returns></returns>
bool Enemy::PlayerAttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetAttackRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::PlayerSkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetSkillRadius())
	{
		return true;
	}

	return false;

}

bool Enemy::EnemyAttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_attackPos.x - pPlayer->GetPos().x) * (m_attackPos.x - pPlayer->GetPos().x);
	float delY = ((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_attackPos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_attackPos.z - pPlayer->GetPos().z) * (m_attackPos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::EnemySkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_skillRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::SearchSphereFlag(std::shared_ptr<Player> pPlayer)
{

	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::StopSphereFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetPos().x) * (m_pos.x - pPlayer->GetPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetPos().z) * (m_pos.z - pPlayer->GetPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_stopRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

/// <summary>
/// �A�j���[�V�����̃A�b�v�f�[�g����
/// </summary>
/// <param name="attachNo"></param>
/// <returns></returns>
bool Enemy::UpdateAnim(int attachNo, bool isStayEndAnim)
{
	//�A�j���[�V�������ݒ肳��Ă��Ȃ��̂ŏI��
	if (attachNo == -1) return false;

	//�A�j���[�V������i�s������
	float now = MV1GetAttachAnimTime(m_modelH, attachNo);	//���݂̍Đ��J�E���g���擾
	bool isLoop = false;
	now += 0.5f;	//�A�j���[�V������i�߂�

	//���ݍĐ����̃A�j���[�V�����̑��J�E���g���擾����
	float total = MV1GetAttachAnimTotalTime(m_modelH, attachNo);

	while (now > total)
	{
		if (isStayEndAnim)	now = total;
		else				now -= total;
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
void Enemy::ChangeAnim(int animIndex)
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

void Enemy::Move()
{

}


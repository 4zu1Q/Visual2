#include "Enemy.h"
#include "Player.h"
#include <cmath>

/// <summary>
/// �萔
/// </summary>
namespace
{
	//���f���̃t�@�C����
	const char* const kEnemyModelFilename = "data/model/enemy/Enemy.mv1";

	//�V�F�[�_�̃t�@�C����
	const char* const kOutlinePsFilename = "./OutlinePs.pso";
	const char* const kOutlineVsFilename = "./OutlineVs.vso";

	//�����蔻��
	constexpr float kAddPosY = 8.0f;

	//�_���[�W
	constexpr int kDamageCount = 120;

	//�X�s�[�h



}

/// <summary>
/// �R���X�g���N�^
/// </summary>
Enemy::Enemy() :
	m_modelHandle(),
	m_outlinePsH(-1),
	m_outlineVsH(-1),
	m_radius(6.0f),
	m_searchRadius(70.0f),
	m_stopRadius(20.0f),
	m_hp(20),
	m_pos(VGet(0, 0, 0)),
	m_attackPos(VGet(0,0,0)),
	m_velocity(VGet(0, 0, 0)),
	m_damageFrame(0),
	m_isDamage(false),
	m_state(kIdle)
{
	m_pPlayer = std::make_shared<Player>();
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
	m_modelHandle = MV1LoadModel(kEnemyModelFilename);

	//ShaderLoad();
}

/// <summary>
/// �f���[�g
/// </summary>
void Enemy::Delete()
{
	MV1DeleteModel(m_modelHandle);
	m_modelHandle = -1;
}

/// <summary>
/// ������
/// </summary>
void Enemy::Init()
{
	m_pos = VGet(-60.0f, 0.0f, 0.0f);
	m_attackPos = VGet(m_pos.x, m_pos.y, m_pos.z - 10);

	MV1SetPosition(m_modelHandle, m_pos);
	//�G�̃X�P�[��
	MV1SetScale(m_modelHandle, VGet(10, 10, 10));
}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
void Enemy::Update()
{
	VECTOR enemyToPlayer = VSub(m_pPlayer->GetPos(), m_pos);
	bool isSearch = SearchSphereFlag(m_pPlayer);
	bool isStop = StopSphereFlag(m_pPlayer);

#ifdef _DEBUG
	m_isDebugFlag = isSearch;
#endif

	VECTOR direction;
	//�v���C���[�ւ̌������擾
	direction = VSub(m_pPlayer->GetPos(), m_pos);
	//�x�N�g�����A���K�����A����������ۑ�������
	direction = VNorm(direction);

	float speed = 1.1f;
	m_velocity = VScale(direction, speed);


	if (m_state == kIdle)			//�~�܂��Ă�����
	{

	}
	else if (m_state == kRun)		//�v���C���[��ǂ��Ă�����
	{


		//�G�̈ړ�
		m_pos = VAdd(m_pos,m_velocity);

	}
	else if (m_state == kAttack)	//�U���̏��
	{


	}
	else if (m_state == kPowerAttack)//���ߍU���̏��
	{

	}

	//m_attackPos = m_pos;

	//�ړ��͈�
	if (m_pos.x >= 195) m_pos.x = 195;
	if (m_pos.x <= -195) m_pos.x = -195;
	if (m_pos.z >= 195) m_pos.z = 195;
	if (m_pos.z <= -195) m_pos.z = -195;

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

	MV1SetPosition(m_modelHandle, m_pos);
}

/// <summary>
/// �`��
/// </summary>
void Enemy::Draw()
{


#ifdef _DEBUG

	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_radius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_searchRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_pos, VGet(0, 8, 0)), m_stopRadius, 8, 0xffffff, 0xffffff, false);
	DrawSphere3D(VAdd(m_attackPos, VGet(0, 8, 0)), m_radius, 8, 0xf00fff, 0xffffff, false);
	DrawFormatString(0, 32, 0xffffff, "Enemy(x:%f,y:%f,z:%f)", m_pos.x, m_pos.y, m_pos.z);
	DrawFormatString(400, 32, 0xffffff, "EnemyHp:%d", m_hp);
	DrawFormatString(400, 332, 0xffffff, "EnemyState:%d", m_state);

#endif

	// �_���[�W���o  2�t���[���Ԋu�ŕ\����\���؂�ւ�
// 0: �\�������
// 1: �\�������
// 2: ��\��
// 3: ��\��
// 4: �\�������	...
// % 4 ���邱�Ƃ�012301230123... �ɕϊ�����
	if (m_damageFrame % 8 >= 4) return;

	MV1DrawModel(m_modelHandle);
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
bool Enemy::AttackSphereHitFlag(std::shared_ptr<Player> pPlayer)
{
	//X,Y,Z�̋����̐������擾
	float delX = (m_pos.x - pPlayer->GetAttackPos().x) * (m_pos.x - pPlayer->GetAttackPos().x);
	float delY = ((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY)) *
		((m_pos.y + kAddPosY) - (pPlayer->GetAttackPos().y + kAddPosY));
	float delZ = (m_pos.z - pPlayer->GetAttackPos().z) * (m_pos.z - pPlayer->GetAttackPos().z);

	//���Ƌ��̋���
	float Distance = sqrt(delX + delY + delZ);

	//���Ƌ��̋������v���C���ƃG�l�~�[�̔��a�����������ꍇ
	if (Distance < m_radius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}

bool Enemy::SkillSphereHitFlag(std::shared_ptr<Player> pPlayer)
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

bool Enemy::DamageSphereHitFlag(std::shared_ptr<Player> pPlayer)
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
	if (Distance < m_searchRadius + pPlayer->GetRadius())
	{
		return true;
	}

	return false;
}


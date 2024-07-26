#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "DxLib.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "GimmickObj.h"

#include "Stage.h"
#include "SkyDome.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScenePlaying::ScenePlaying() :
	m_isPlayerHit(false),
	m_isAttackHit(false)
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pGimmick = std::make_shared<GimmickObj>();
	m_pStage = std::make_shared<Stage>();
	m_pSkyDome = std::make_shared<SkyDome>();


	m_pPlayer->Load();
	m_pEnemy->Load();
	m_pStage->Load();

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePlaying::~ScenePlaying()
{
}

/// <summary>
/// ������
/// </summary>
void ScenePlaying::Init()
{
	m_pCamera->Init();

	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pGimmick->Init();
	m_pStage->Init();
}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	//�J�����̃A���O�����Z�b�g����
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	m_pPlayer->Update();
	m_pCamera->PlayCameraUpdate(*m_pPlayer);
	//m_pCamera->TitleCameraUpdate();
	m_pEnemy->Update(*m_pPlayer);
	m_pGimmick->Update();

	m_pStage->Update();
	m_pSkyDome->Update(m_pPlayer->GetPos());

	//���������ꍇ�̃t���O�̎擾
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);
	m_isAttackHit;

	//�M�~�b�N�ɓ��������ꍇ�̃t���O�擾
	m_isGimmickHit = m_pGimmick->SphereHitFlag(m_pPlayer);

	VECTOR toEnemy = VSub(m_pEnemy->GetPos(),m_pPlayer->GetPos() );
	float length = VSize(toEnemy);

	//�v���C���[��hp���擾
	int playerHp = m_pPlayer->GetHp();

	if (m_isGimmickHit)
	{
		printfDx("d");
		m_pPlayer->OnGimmickHitUpdate();
	}

	//�v���C���[�ƓG�����������ꍇ
	if (m_isPlayerHit)
	{
		VECTOR posVec;
		VECTOR moveVec;


		//�G�l�~�[�̃x�N�g�����W����v���C���[�̃x�N�g�����W���������x�N�g��
		posVec = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());

		//
		moveVec = VScale(posVec, length - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
		m_pPlayer->SetPos(VAdd(m_pPlayer->GetPos(), moveVec));


		playerHp -= 1;
		m_pPlayer->SetHp(playerHp);
		
	}

	//�v���C���[�̍U���ƓG�����������ꍇ
	if (m_isAttackHit)
	{
		printfDx("��������");
	}

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void ScenePlaying::Draw()
{

	m_pPlayer->Draw();
	m_pEnemy->Draw();
	m_pGimmick->Draw();
	m_pStage->Draw();
	m_pSkyDome->Draw();

	DrawString(0, 0, "Scene Playing", 0xffffff, false);
}

/// <summary>
/// �������̉��
/// </summary>
void ScenePlaying::End()
{
	m_pPlayer->Delete();
	m_pEnemy->Delete();
	m_pStage->Delete();
}

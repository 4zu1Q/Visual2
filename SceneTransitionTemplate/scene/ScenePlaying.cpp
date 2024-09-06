#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "SceneLose.h"
#include "SceneWin.h"
#include "DxLib.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
#include "Stage.h"
#include "SkyDome.h"
#include "ItemBase.h"
#include "ItemHp.h"

#include "Game.h"
#include "Pad.h"


namespace
{
	//�t�F�[�h�֘A
	constexpr int kFadeTime = 120;
	constexpr int kBlend = 255;

	constexpr int kSelectLeft = Game::kScreenWidth * 0.3;
	constexpr int kSelectRight = Game::kScreenWidth * 0.4;

	constexpr int kLeft = Game::kScreenWidth * 0.45;
	constexpr int kRight = Game::kScreenWidth * 0.55;

	constexpr int kStartTop = 220;
	constexpr int kStartDown = 300;

	constexpr int kOptionTop = 320;
	constexpr int kOptionDown = 400;

	constexpr int kEndTop = 420;
	constexpr int kEndDown = 500;

	constexpr float kSelectSpeed = 0.05f;
	constexpr float kSelectAnimationSize = 9.0f;

	constexpr int kPlayerMaxHp = 10;

	//�A�C�e���̃t�@�C����
	const char* const kItemHpModelFilename = "data/model/item/Heart.mv1";
	const char* const kPlayerModelFilename = "data/model/player/barbarian.mv1";
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScenePlaying::ScenePlaying() :
	m_isInterval(false),
	m_isPlayerHit(false),
	m_isAttackHit(false),
	m_isMenu(false),
	m_isCommand(false),
	m_isTitle(false),
	m_isDamageCount(false),
	m_isAttackHitCount(false),
	m_isSkillHitCount(false),
	m_isSkillHit(false),
	m_isItemHit(false),
	m_isEnemySearch(false),
	m_isEnemyStop(false),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_restartH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
	m_titleH(LoadGraph("data/image/Title.png")),
	m_select(kRestart),
	m_frameScene(0),
	m_attackFrameHit(0),
	m_skillFrameHit(0),
	m_frameDamage(0)
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pStage = std::make_shared<Stage>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pItem.resize(20);

	CreateItemHp(VGet(100, 5, 100));
	CreateItemHp(VGet(60, 5, 60));
	CreateItemHp(VGet(100, 5, 200));
	CreateItemHp(VGet(-100, 5, 100));
	

	m_pPlayer->Load();
	m_pEnemy->Load();
	m_pStage->Load();

}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ScenePlaying::~ScenePlaying()
{
	DeleteGraph(m_selectH);
	DeleteGraph(m_restartH);
	DeleteGraph(m_optionH);
	DeleteGraph(m_titleH);

	MV1DeleteModel(m_modelHeartH);
	m_modelHeartH = -1;

}

/// <summary>
/// ������
/// </summary>
void ScenePlaying::Init()
{
	m_pCamera->Init();

	m_pPlayer->Init();
	m_pEnemy->Init();
	m_pStage->Init();
}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	Pad::Update();
	if (Pad::IsTrigger(PAD_INPUT_8)) m_isMenu = true;

	//�v���C���[��hp���擾
	int playerHp = m_pPlayer->GetHp();
	int enemyHp = m_pEnemy->GetHp();


	//�J�����̃A���O�����Z�b�g����
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	//���j���[��\�����Ă��Ȃ��ꍇ
	if (!m_isMenu)
	{
		m_pPlayer->Update();
		m_pEnemy->Update();
		m_pStage->Update();
		m_pSkyDome->Update(*m_pPlayer);

		for (int i = 0; i < m_pItem.size(); i++)
		{
			if (m_pItem[i])
			{
				m_pItem[i]->Update();
			}
		}

	}
	else //���j���[��\�����Ă�ꍇ
	{

		if (!m_isCommand)
		{
			//��������������Ƃ�
			if (Pad::IsTrigger(PAD_INPUT_UP))
			{

				if (m_select == kRestart)
				{
					m_select = kTitle;
				}
				else if (m_select == kOption)
				{
					m_select = kRestart;
				}
				else if (m_select == kTitle)
				{
					m_select = kOption;
				}
			}

			//���������������Ƃ�
			if (Pad::IsTrigger(PAD_INPUT_DOWN))
			{
				if (m_select == kRestart)
				{
					m_select = kOption;
				}
				else if (m_select == kOption)
				{
					m_select = kTitle;
				}
				else if (m_select == kTitle)
				{
					m_select = kRestart;
				}
			}

			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				if (m_select == kRestart)
				{
					m_isMenu = false;
					m_isCommand = false;
				}
				else if (m_select == kOption)
				{
					m_isCommand = true;
				}
				else if (m_select == kTitle)
				{
					m_isInterval = true;
					m_isTitle = true;
				}
			}
		}

	}

	m_pCamera->PlayCameraUpdate(*m_pPlayer);



	//�v���C���[�ƓG�����������ꍇ�̃t���O�̎擾
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);

	//�v���C���[�̍U���ɓ��������ꍇ�̃t���O�擾
	m_isAttackHit = m_pEnemy->AttackSphereHitFlag(m_pPlayer);

	//�v���C���[�̃X�L���ɓ��������ꍇ�̃t���O�擾
	m_isSkillHit = m_pEnemy->SkillSphereHitFlag(m_pPlayer);

	//�G�̍U���ɓ��������ꍇ�̃t���O�擾
	m_isDamageHit = m_pEnemy->DamageSphereHitFlag(m_pPlayer);

	m_isEnemySearch = m_pEnemy->SearchSphereFlag(m_pPlayer);

	m_isEnemyStop = m_pEnemy->StopSphereFlag(m_pPlayer);

	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());
	float length = VSize(toEnemy);


	//�G�̍s���̓����蔻��
	//�G�̍��G�Ƀv���C���[�����Ȃ�������
	if (!m_isEnemySearch && !m_isEnemyStop)
	{
		m_pEnemy->SetState(Enemy::kIdle);
	}
	//�G�̍��G�Ƀv���C���[��������
	if (m_isEnemySearch && !m_isEnemyStop)
	{
		m_pEnemy->SetState(Enemy::kRun);
	}
	//�G�̍U������͈͂Ƀv���C���[��������
	if (m_isEnemyStop)
	{
		m_pEnemy->SetState(Enemy::kAttack);
	}




	//�A�C�e���ƃv���C���[�̓����蔻��
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (m_pItem[i])
		{
			//�A�C�e���ɓ���������
			if (m_isItemHit = m_pItem[i]->ItemSphereFlag(m_pPlayer))
			{
				if (m_pPlayer->GetHp() == kPlayerMaxHp)
				{
					//�������Ȃ�
				}
				else
				{
					playerHp += 1;
					m_pPlayer->SetHp(playerHp);
					m_pItem[i]->ItemLost();

					delete m_pItem[i];
					m_pItem[i] = nullptr;
				}
			}
		}
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

	}

	//�U��������������������
	if (m_pPlayer->GetAttackGeneration())
	{
		//�U���̃N�[���^�C��
		if (!m_isAttackHitCount)
		{
			//�v���C���[�̍U�����G�ɓ������ꍇ
			if (m_isAttackHit)
			{
				enemyHp -= 1;
				m_pEnemy->SetHp(enemyHp);
				m_isAttackHitCount = true;
				m_pEnemy->SetDamage(true);
			}

		}
		else
		{
			m_attackFrameHit++;

			if (m_attackFrameHit >= 60)
			{
				m_isAttackHitCount = false;
				m_attackFrameHit = 0;
			}
		}
	}

	//�X�L�����g�p�������̂ݔ�������
	if (m_pPlayer->GetSkillGeneration())
	{
		//�X�L���̃N�[���^�C��
		if (!m_isSkillHitCount)
		{

			//�v���C���[�̃X�L�����G�ɓ������ꍇ
			if (m_isSkillHit)
			{
				enemyHp -= 10;
				m_pEnemy->SetHp(enemyHp);
				m_isSkillHitCount = true;
				m_pEnemy->SetDamage(true);
			}

		}
		else
		{
			m_skillFrameHit++;

			if (m_skillFrameHit >= 180)
			{
				m_isSkillHitCount = false;
				m_skillFrameHit = 0;
			}
		}
	}




	//�_���[�W�̃N�[���^�C��

	if (!m_isDamageCount)
	{
		//�G�̍U�����󂯂��ꍇ
		if (m_isDamageHit)
		{
			playerHp -= 1;
			m_pPlayer->SetHp(playerHp);
			m_isDamageCount = true;
			m_pPlayer->SetDamage(true);
			m_pPlayer->SetAnimDamage(true);
		}
	}
	else
	{
		m_frameDamage++;

		if (m_frameDamage >= 120)
		{
			m_isDamageCount = false;
			m_frameDamage = 0;
		}
	}

	//�v���C���[��HP���[���ɂȂ�����
	if (m_pPlayer->GetHp() <= 0)
	{
		m_isInterval = true;
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneLose>();
		}
	}

	//�{�X��HP���[���ɂȂ�����
	if (m_pEnemy->GetHp() <= 0)
	{
		m_isInterval = true;
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneWin>();
		}
	}



	//���j���[����^�C�g����
	if (m_isTitle)
	{
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneTitle>();
		}
	}

	//�Z���N�g�̃A�j���[�V����
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;

	return shared_from_this();
}

/// <summary>
/// �`��
/// </summary>
void ScenePlaying::Draw()
{

	m_pSkyDome->Draw();
	m_pStage->Draw();
	m_pPlayer->Draw();
	m_pEnemy->Draw();
	
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (m_pItem[i])
		{
			m_pItem[i]->Draw();
		}
	}

	if (m_isMenu)
	{
		// �������ɂ��ă��j���[�w�i�̎l�p
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawFillBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x000000);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		// �s�����ɖ߂��Ĕ����g
		DrawLineBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x00ffff);


		//�Z���N�g
		if (m_select == kRestart)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
		}
		else if (m_select == kOption)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
		}
		else if (m_select == kTitle)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
		}


		DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_restartH, true); //�X�^�[�g
		DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//�I�v�V����
		DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_titleH, true);//�Q�[���I��
	}

	//�t�F�[�h�Ö�
	if (m_isInterval)
	{
		int alpha = kBlend * m_frameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#ifdef _DEBUG
	DrawFormatString(700, 16, 0xffffff, "Select:%d", m_select);
	DrawString(0, 0, "Scene Playing", 0xffffff, false);
#endif
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

void ScenePlaying::CreateItemHp(VECTOR pos)
{
	for (int i = 0; i < m_pItem.size(); i++)
	{
		if (!m_pItem[i])
		{
			m_pItem[i] = new ItemHp;
			m_pItem[i]->Start(pos);
			return;
		}
	}
}


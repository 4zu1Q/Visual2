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
#include <EffekseerForDXLib.h>


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

	//SE�̃t�@�C����
	const char* const kSelectFilename = "data/sound/se/SelectSe.mp3";
	const char* const kDecisionFilename = "data/sound/se/DecisionSe.mp3";
	const char* const kCancelFilename = "data/sound/se/CancelSe.mp3";

	const char* const kADamageFilename = "data/sound/se/EnemyADamageSe.mp3";
	const char* const kSDamageFilename = "data/sound/se/EenemySDamageSe.mp3";
	const char* const kPlayerDamageFilename = "data/sound/se/PlayerDamageSe.mp3";
	const char* const kItemFilename = "data/sound/se/HealSe.mp3";


	//BGM�̃t�@�C����
	const char* const kBgmFilename = "data/sound/bgm/GamePlayBgm.mp3";

	constexpr float kWall = 475;


	constexpr int kGameClearSceneTime = 600;
	constexpr int kGameOverSceneTime = 240;
	constexpr int kOptionSceneTime = 120;

	constexpr float kHitEffectScale = 5.0f;
	constexpr float kItemEffectScale = 2.0f;
	constexpr float kAddPos = 8.0f;
}

/// <summary>
/// �R���X�g���N�^
/// </summary>
ScenePlaying::ScenePlaying() :
	m_hitSH(-1),
	m_hitPH(-1),
	m_itemSH(-1),
	m_itemPH(-1),
	m_isInterval(false),
	m_isPlayerHit(false),
	m_isPlayerAttackHit(false),
	m_isMenu(false),
	m_isCommand(false),
	m_isTitle(false),
	m_isEnemyAttackHitCount(false),
	m_isEnemySkillHitCount(false),
	m_isPlayerAttackHitCount(false),
	m_isPlayerSkillHitCount(false),
	m_isPlayerSkillHit(false),
	m_isItemHit(false),
	m_isEnemySearch(false),
	m_isEnemyStop(false),
	m_isEnemyAttackHit(false),
	m_isEnemySkillHit(false),
	m_isOption(false),
	m_isLose(false),
	m_isWin(false),
	m_isEnemyDeath(false),
	m_isPlayerDeath(false),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_restartH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
	m_titleH(LoadGraph("data/image/Title.png")),
	m_operatorH(LoadGraph("data/image/Operator.png")),
	m_select(kRestart),
	m_frameScene(0),
	m_playerAttackHitFrame(0),
	m_playerSkillHitFrame(0),
	m_playerFrame(0),
	m_enemyAttackHitFrame(0),
	m_enemySkillHitFrame(0),
	m_enemyFrame(0),
	m_fadeFrameScene(0),
	m_soundBgmH(-1),
	m_soundCancelH(-1),
	m_soundDecsionH(-1),
	m_soundSelectH(-1),
	m_soundPlayerDamageH(-1),
	m_soundADamageH(-1),
	m_soundSDamageH(-1)
{

	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	m_pStage = std::make_shared<Stage>();
	m_pSkyDome = std::make_shared<SkyDome>();
	m_pItem.resize(20);

	CreateItemHp(VGet(460, 5, 460));
	CreateItemHp(VGet(-460, 5, 460));
	CreateItemHp(VGet(460, 5, -460));
	CreateItemHp(VGet(-460, 5, -460));

	CreateItemHp(VGet(460, 5, 0));
	CreateItemHp(VGet(0, 5, 460));
	CreateItemHp(VGet(-460, 5, 0));
	CreateItemHp(VGet(0, 5, -460));

	CreateItemHp(VGet(230, 5, 0));
	CreateItemHp(VGet(0, 5, -230));
	CreateItemHp(VGet(-230, 5, 0));
	CreateItemHp(VGet(0, 5, 230));


	CreateItemHp(VGet(230, 5, 230));
	CreateItemHp(VGet(-230, 5, 230));
	CreateItemHp(VGet(230, 5, -230));
	CreateItemHp(VGet(-230, 5, -230));

	m_soundBgmH = LoadSoundMem(kBgmFilename);	  //BGM

	m_soundSelectH = LoadSoundMem(kSelectFilename);	  //�I����
	m_soundDecsionH = LoadSoundMem(kDecisionFilename);	  //���艹
	m_soundCancelH = LoadSoundMem(kCancelFilename);	  //�L�����Z����
	m_soundPlayerDamageH = LoadSoundMem(kPlayerDamageFilename);		//�v���C���[�_���[�W��
	m_soundADamageH = LoadSoundMem(kADamageFilename);		//�G�_���[�W��
	m_soundSDamageH = LoadSoundMem(kSDamageFilename);		//�G�_���[�W��
	m_soundItemH = LoadSoundMem(kItemFilename);		//�A�C�e����

	ChangeVolumeSoundMem(64, m_soundBgmH);
	ChangeVolumeSoundMem(128, m_soundSelectH);
	ChangeVolumeSoundMem(128, m_soundDecsionH);
	ChangeVolumeSoundMem(128, m_soundCancelH);
	ChangeVolumeSoundMem(128, m_soundPlayerDamageH);
	ChangeVolumeSoundMem(128, m_soundADamageH);
	ChangeVolumeSoundMem(128, m_soundSDamageH);
	ChangeVolumeSoundMem(128, m_soundItemH);

	m_hitSH = LoadEffekseerEffect("data/effect/ToonHit.efkefc");
	m_itemSH = LoadEffekseerEffect("data/effect/Hit.efkefc");


	m_pPlayer->Load();
	m_pEnemy->Load();
	m_pStage->Load();

	PlaySoundMem(m_soundBgmH, DX_PLAYTYPE_LOOP);

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

	DeleteSoundMem(m_soundSelectH);
	DeleteSoundMem(m_soundDecsionH);
	DeleteSoundMem(m_soundCancelH);
	DeleteSoundMem(m_soundBgmH);
	DeleteSoundMem(m_soundPlayerDamageH);
	DeleteSoundMem(m_soundADamageH);
	DeleteSoundMem(m_soundSDamageH);
	DeleteSoundMem(m_soundItemH);

	MV1DeleteModel(m_modelHeartH);
	m_modelHeartH = -1;

	//�G�t�F�N�g���f���[�g
	DeleteEffekseerEffect(m_hitSH);
	DeleteEffekseerEffect(m_itemSH);
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
	//�G�t�F�N�g�̃A�b�v�f�[�g����
	UpdateEffekseer3D();

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
		ChangeVolumeSoundMem(64, m_soundBgmH);

		m_pPlayer->Update();
		m_pEnemy->Update(m_pPlayer);
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
			ChangeVolumeSoundMem(32, m_soundBgmH);


			//��������������Ƃ�
			if (Pad::IsTrigger(PAD_INPUT_UP))
			{

				if (m_select == kRestart)
				{
					m_select = kTitle;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
				else if (m_select == kOption)
				{
					m_select = kRestart;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
				else if (m_select == kTitle)
				{
					m_select = kOption;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
			}

			//���������������Ƃ�
			if (Pad::IsTrigger(PAD_INPUT_DOWN))
			{
				if (m_select == kRestart)
				{
					m_select = kOption;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
				else if (m_select == kOption)
				{
					m_select = kTitle;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
				else if (m_select == kTitle)
				{
					m_select = kRestart;
					PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				}
			}

			if (Pad::IsTrigger(PAD_INPUT_1))
			{
				if (m_select == kRestart)
				{
					m_isMenu = false;
					m_isCommand = false;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹
				}
				else if (m_select == kOption)
				{
					m_isCommand = true;
					m_isOption = true;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹
				}
				else if (m_select == kTitle)
				{
					m_isInterval = true;
					m_isCommand = false;
					m_isTitle = true;
					PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹
					StopSoundMem(m_soundBgmH);
				}
			}

		}

		if (m_isOption)
		{
			if (Pad::IsTrigger PAD_INPUT_2)
			{
				m_isOption = false;
				m_isCommand = false;
			}
		}

	}

	m_pCamera->PlayCameraUpdate(*m_pPlayer);
	//m_pCamera->TargetCameraUpadate(*m_pPlayer, *m_pEnemy);


	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());
	float length = VSize(toEnemy);

	VECTOR posVec = VGet(0, 0, 0);
	VECTOR moveVec = VGet(0, 0, 0);

	//�v���C���[�ƓG�����������ꍇ�̃t���O�̎擾
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);

	if (!m_isPlayerDeath)
	{
		//�G�̍U���ɓ��������ꍇ�̃t���O�擾
		m_isEnemyAttackHit = m_pEnemy->EnemyAttackSphereHitFlag(m_pPlayer);

		//�G�̃X�L���ɓ��������ꍇ�̃t���O�擾
		m_isEnemySkillHit = m_pEnemy->EnemySkillSphereHitFlag(m_pPlayer);
	}

	//�G�����񂾂瓖���蔻�������
	if (!m_isEnemyDeath)
	{
		//�v���C���[�̍U���ɓ��������ꍇ�̃t���O�擾
		m_isPlayerAttackHit = m_pEnemy->PlayerAttackSphereHitFlag(m_pPlayer);

		//�v���C���[�̃X�L���ɓ��������ꍇ�̃t���O�擾
		m_isPlayerSkillHit = m_pEnemy->PlayerSkillSphereHitFlag(m_pPlayer);

		//�G�̍��G�͈͂ɓ������ꍇ�̃t���O�擾
		m_isEnemySearch = m_pEnemy->SearchSphereFlag(m_pPlayer);

		//�G�̎~�܂�͈͂ɓ������ꍇ�̃t���O�擾
		m_isEnemyStop = m_pEnemy->StopSphereFlag(m_pPlayer);

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

		//�v���C���[�ƓG�����������ꍇ
		if (m_isPlayerHit)
		{
			Knockback(posVec, moveVec, length);
		}
	}
	else
	{
		m_pEnemy->SetState(Enemy::kDeath);
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
					m_itemPH = PlayEffekseer3DEffect(m_itemSH);
					SetScalePlayingEffekseer3DEffect(m_itemPH, kItemEffectScale, kItemEffectScale, kItemEffectScale);
					SetPosPlayingEffekseer3DEffect(m_itemPH, m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + kAddPos, m_pPlayer->GetPos().z);


					PlaySoundMem(m_soundItemH, DX_PLAYTYPE_BACK, true);//�A�C�e����

					playerHp += 1;
					m_pPlayer->SetHp(playerHp);
					m_pItem[i]->ItemLost();

					delete m_pItem[i];
					m_pItem[i] = nullptr;
				}
			}
		}
	}

	if (!m_isEnemyDeath)
	{
		//�v���C���[�U��������������������
		if (m_pPlayer->GetAttackGeneration())
		{
			//�U���̃N�[���^�C��
			if (!m_isPlayerAttackHitCount)
			{
				m_playerFrame++;
				if (m_playerFrame > 20)
				{
					//�v���C���[�̍U�����G�ɓ������ꍇ
					if (m_isPlayerAttackHit)
					{
						PlaySoundMem(m_soundADamageH, DX_PLAYTYPE_BACK, true);//�_���[�W��

						//enemyHp -= 100;
						enemyHp -= 10;
						m_pEnemy->SetHp(enemyHp);
						m_isPlayerAttackHitCount = true;
						m_pEnemy->SetDamage(true);

						m_hitPH = PlayEffekseer3DEffect(m_hitSH);
						SetScalePlayingEffekseer3DEffect(m_hitPH, kHitEffectScale, kHitEffectScale, kHitEffectScale);
						SetPosPlayingEffekseer3DEffect(m_hitPH, m_pEnemy->GetPos().x, m_pEnemy->GetPos().y + kAddPos, m_pEnemy->GetPos().z);

						m_playerFrame = 0;
					}
				}
			}
			else
			{
				m_playerAttackHitFrame++;

				if (m_playerAttackHitFrame >= 50)
				{
					m_isPlayerAttackHitCount = false;
					m_playerAttackHitFrame = 0;
				}
			}
		}

		//�v���C���[�X�L�����g�p�������̂ݔ�������
		if (m_pPlayer->GetSkillGeneration())
		{
			//�X�L���̃N�[���^�C��
			if (!m_isPlayerSkillHitCount)
			{
				m_playerFrame++;
				if (m_playerFrame > 30)
				{
					//�v���C���[�̃X�L�����G�ɓ������ꍇ
					if (m_isPlayerSkillHit)
					{
						PlaySoundMem(m_soundSDamageH, DX_PLAYTYPE_BACK, true);//�_���[�W��
						enemyHp -= 50;
						//enemyHp -= 300;

						m_pEnemy->SetHp(enemyHp);
						m_isPlayerSkillHitCount = true;
						m_pEnemy->SetDamage(true);

						m_hitPH = PlayEffekseer3DEffect(m_hitSH);
						SetScalePlayingEffekseer3DEffect(m_hitPH, kHitEffectScale, kHitEffectScale, kHitEffectScale);
						SetPosPlayingEffekseer3DEffect(m_hitPH, m_pEnemy->GetPos().x, m_pEnemy->GetPos().y + kAddPos, m_pEnemy->GetPos().z);

						m_playerFrame = 0;
					}
				}
			}
			else
			{
				m_playerSkillHitFrame++;

				if (m_playerSkillHitFrame >= 50)
				{
					m_isPlayerSkillHitCount = false;
					m_playerSkillHitFrame = 0;
				}
			}
		}
	}


	if (m_pEnemy->GetAttackGeneration() && !m_pEnemy->GetSkillGeneration())
	{
		//�_���[�W�̃N�[���^�C��
		if (!m_isEnemyAttackHitCount)
		{
			m_enemyFrame++;
			if (m_enemyFrame > 30)
			{
				//�G�̍U�����󂯂��ꍇ
				if (m_isEnemyAttackHit)
				{
					PlaySoundMem(m_soundPlayerDamageH, DX_PLAYTYPE_BACK, true);//�_���[�W��
					playerHp -= 1;
					m_pPlayer->SetHp(playerHp);
					m_isEnemyAttackHitCount = true;
					m_pPlayer->SetDamage(true);
					m_pPlayer->SetAnimDamage(true);

					m_hitPH = PlayEffekseer3DEffect(m_hitSH);
					SetScalePlayingEffekseer3DEffect(m_hitPH, kHitEffectScale, kHitEffectScale, kHitEffectScale);
					SetPosPlayingEffekseer3DEffect(m_hitPH, m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + kAddPos, m_pPlayer->GetPos().z);
					
					m_enemyFrame = 0;

				}
			}

		}
		else
		{
			m_enemyAttackHitFrame++;
			if (m_enemyAttackHitFrame >= 120)
			{
				m_pEnemy->SetAttackAnim(false);
				m_isEnemyAttackHitCount = false;
				m_enemyAttackHitFrame = 0;
				//m_pEnemy->SetAttackGeneration(false);

				
			}
		}
	}




	if (m_pEnemy->GetSkillGeneration() && !m_pEnemy->GetAttackGeneration())
	{

		m_enemyFrame++;
		if (m_enemyFrame > 50)
		{
			//�_���[�W�̃N�[���^�C��
			if (!m_isEnemySkillHitCount)
			{
				//�G�̍U�����󂯂��ꍇ
				if (m_isEnemySkillHit)
				{

					PlaySoundMem(m_soundPlayerDamageH, DX_PLAYTYPE_BACK, true);//�_���[�W��
					playerHp -= 2;
					m_pPlayer->SetHp(playerHp);
					m_isEnemySkillHitCount = true;
					m_pPlayer->SetDamage(true);
					m_pPlayer->SetAnimDamage(true);
					m_enemyFrame = 0;

					m_hitPH = PlayEffekseer3DEffect(m_hitSH);
					SetScalePlayingEffekseer3DEffect(m_hitPH, kHitEffectScale, kHitEffectScale, kHitEffectScale);
					SetPosPlayingEffekseer3DEffect(m_hitPH, m_pPlayer->GetPos().x, m_pPlayer->GetPos().y + kAddPos, m_pPlayer->GetPos().z);

				}
			}
			else
			{
				m_enemySkillHitFrame++;

				if (m_enemySkillHitFrame >= 120)
				{
					m_pEnemy->SetSkillAnim(false);
					m_isEnemySkillHitCount = false;
					m_enemySkillHitFrame = 0;
					//m_pEnemy->SetSkillGeneration(false);
				}
			}

			
		}
		
	}



	






	//�v���C���[��HP���[���ɂȂ�����
	if (m_pPlayer->GetHp() <= 0)
	{
		m_isLose = true;
		m_frameScene++;

		m_isPlayerDeath = true;
		m_pPlayer->SetIsDown(true);

		if (m_frameScene >= kGameOverSceneTime - 120)
		{
			m_fadeFrameScene++;
			m_isInterval = true;
		}
		if (m_frameScene >= kGameOverSceneTime)
		{
			return std::make_shared<SceneLose>();
		}
	}
	else
	{
		m_isLose = false;
	}

	//�{�X��HP���[���ɂȂ�����
	if (m_pEnemy->GetHp() <= 0)
	{
		m_isEnemyDeath = true;
		m_isWin = true;
		m_frameScene++;

		if (m_frameScene >= 240)
		{
			m_pPlayer->SetCommand(true);

		}

		if (m_frameScene >= kGameClearSceneTime - 120)
		{
			m_fadeFrameScene++;
			m_isInterval = true;
		}

		if (m_frameScene >= kGameClearSceneTime)
		{
			return std::make_shared<SceneWin>();
		}
	}
	else
	{
		m_isWin = false;
	}




	//���j���[����^�C�g����
	if (m_isTitle)
	{
		m_fadeFrameScene++;
		m_frameScene++;
		if (m_frameScene >= kOptionSceneTime)
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

	// �J�����̂RD�������킹�Ă���
	Effekseer_Sync3DSetting();
	// �G�t�F�N�V�A�̏����X�V���Ă���
	UpdateEffekseer3D();
	// �G�t�F�N�V�A��`�悵�Ă���
	DrawEffekseer3D();

	m_pSkyDome->Draw();
	m_pStage->Draw();
	m_pEnemy->Draw();
	m_pPlayer->Draw();
	
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

		if (!m_isOption)
		{
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
		else
		{
			DrawExtendGraph(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, m_operatorH, true);
		}

	}

	//�t�F�[�h�Ö�
	if (m_isInterval)
	{
		int alpha = kBlend * m_fadeFrameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//if (m_isInterval)
	//{
	//	int alpha2 = kBlend * m_frameScene / kFadeTime;
	//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0xffffff, true);
	//	SetDrawBlendMode(DX_BLENDMODE_MULA, alpha2);
	//}

#ifdef _DEBUG
	DrawFormatString(700, 16, 0xffffff, "Select:%d", m_select);

	DrawFormatString(700, 96, 0x0000ff, "EnemyAtk:%d", m_enemyAttackHitFrame);
	DrawFormatString(700, 128, 0x0000ff, "EnemySkl:%d", m_enemySkillHitFrame);


	DrawFormatString(700, 160, 0x0000ff, "EnemyAtkGen:%d", m_pEnemy->GetAttackGeneration());
	DrawFormatString(700, 196, 0x0000ff, "EnemySklGen:%d", m_pEnemy->GetSkillGeneration());
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

void ScenePlaying::Knockback(VECTOR pos, VECTOR move, float length)
{
	//�G�l�~�[�̃x�N�g�����W����v���C���[�̃x�N�g�����W���������x�N�g��
	pos = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());

	//
	move = VScale(pos, length - (m_pPlayer->GetRadius() + m_pEnemy->GetRadius()));
	m_pPlayer->SetPos(VAdd(m_pPlayer->GetPos(), move));
}


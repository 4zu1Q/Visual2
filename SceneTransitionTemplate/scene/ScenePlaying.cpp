#include "ScenePlaying.h"
#include "SceneTitle.h"
#include "DxLib.h"

#include "Camera.h"
#include "Player.h"
#include "Enemy.h"
//#include "GimmickObj.h"

#include "Stage.h"
#include "SkyDome.h"
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
	m_selectH(LoadGraph("data/image/Select.png")),
	m_restartH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
	m_titleH(LoadGraph("data/image/End.png")),
	m_select(kRestart),
	m_frameScene(0)
{
	m_pCamera = std::make_shared<Camera>();
	m_pPlayer = std::make_shared<Player>();
	m_pEnemy = std::make_shared<Enemy>();
	//m_pGimmick = std::make_shared<GimmickObj>();
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
	DeleteGraph(m_selectH);
	DeleteGraph(m_restartH);
	DeleteGraph(m_optionH);
	DeleteGraph(m_titleH);

}

/// <summary>
/// ������
/// </summary>
void ScenePlaying::Init()
{
	m_pCamera->Init();

	m_pPlayer->Init();
	m_pEnemy->Init();
	//m_pGimmick->Init();
	m_pStage->Init();
}

/// <summary>
/// �A�b�v�f�[�g
/// </summary>
std::shared_ptr<SceneBase> ScenePlaying::Update()
{
	Pad::Update();
	if (Pad::IsTrigger(PAD_INPUT_9)) m_isMenu = true;


	//�J�����̃A���O�����Z�b�g����
	m_pPlayer->SetCameraAngle(m_pCamera->GetAngle());

	if (!m_isMenu)
	{
		m_pPlayer->Update();
		m_pEnemy->Update(*m_pPlayer);
	}
	else
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
	//m_pGimmick->Update();

	m_pStage->Update();
	m_pSkyDome->Update(m_pPlayer->GetPos());

	//���������ꍇ�̃t���O�̎擾
	m_isPlayerHit = m_pEnemy->SphereHitFlag(m_pPlayer);
	m_isAttackHit;

	//�M�~�b�N�ɓ��������ꍇ�̃t���O�擾
	//m_isGimmickHit = m_pGimmick->SphereHitFlag(m_pPlayer);

	VECTOR toEnemy = VSub(m_pEnemy->GetPos(), m_pPlayer->GetPos());
	float length = VSize(toEnemy);

	//�v���C���[��hp���擾
	int playerHp = m_pPlayer->GetHp();


	//�M�~�b�N�ɓ��������ꍇ
	//if (m_isGimmickHit)
	//{
	//	printfDx("d");
	//	m_pPlayer->OnGimmickHitUpdate();
	//}

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

	//�v���C���[��HP���[���ɂȂ�����
	if (m_pPlayer->GetHp() <= 0)
	{
		m_isInterval = true;
		m_frameScene++;
		if (m_frameScene >= kFadeTime)
		{
			return std::make_shared<SceneTitle>();
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
	//m_pGimmick->Draw();
	m_pStage->Draw();
	m_pSkyDome->Draw();

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
			DrawExtendGraph(kSelectLeft, kStartTop, kSelectRight, kStartDown, m_selectH, true);
		}
		else if (m_select == kOption)
		{
			DrawExtendGraph(kSelectLeft, kOptionTop, kSelectRight, kOptionDown, m_selectH, true);
		}
		else if (m_select == kTitle)
		{
			DrawExtendGraph(kSelectLeft, kEndTop, kSelectRight, kEndDown, m_selectH, true);
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

	DrawFormatString(700, 16, 0xffffff, "Select:%d", m_select);
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


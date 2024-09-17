#include "SceneTitle.h"
#include "DxLib.h"
#include "Pad.h"
#include "Game.h"

#include "SceneManager.h"
#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

/// <summary>
/// �萔
/// </summary>
namespace
{
	//�萔��
	constexpr int kLogoRight = Game::kScreenWidth * 0.8;
	constexpr int kLogoLeft = Game::kScreenWidth * 0.2;
	constexpr int kLogoTop = 30;
	constexpr int kLogoDown = 380;

	constexpr int kSelectLeft = Game::kScreenWidth * 0.24;
	constexpr int kSelectRight = Game::kScreenWidth * 0.34;

	constexpr int kLeft = Game::kScreenWidth * 0.45;
	constexpr int kRight = Game::kScreenWidth * 0.55;

	constexpr int kOperationLeft = Game::kScreenWidth * 0.35;
	constexpr int kOperationRight = Game::kScreenWidth * 0.65;


	constexpr int kStartTop = 420;
	constexpr int kStartDown = 500;

	constexpr int kOptionTop = 520;
	constexpr int kOptionDown = 600;

	constexpr int kEndTop = 620;
	constexpr int kEndDown = 700;

	constexpr int kFadeTime = 120;

	constexpr int kBlend = 255;

	constexpr float kSelectSpeed = 0.05f;
	constexpr float kSelectAnimationSize = 9.0f;

	//SE�̃t�@�C����
	const char* const kSelectFilename = "data/sound/se/SelectSe.mp3";
	const char* const kDecisionFilename = "data/sound/se/DecisionSe.mp3";
	const char* const kCancelFilename = "data/sound/se/CancelSe.mp3";

	//BGM�̃t�@�C����
	const char* const kBgmFilename = "data/sound/bgm/TitleBgm.mp3";
}

SceneTitle::SceneTitle() :
	m_frameCount(0),
	m_frameScene(0),
	m_isInterval(false),
	m_isCommand(false),
	m_isOption(false),
	m_isOperator(false),
	m_isSound(false),
	m_isFullScreen(false),
	m_select(kStart),
	m_option(kOperator),
	m_selectPosY(0),
	m_logoH(LoadGraph("data/image/TitleLogo.png")),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_startH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Operation.png")),
	m_operationH(LoadGraph("data/image/Operator.png")),
	m_endH(LoadGraph("data/image/End.png")),
	m_soundBgmH(-1),
	m_soundCancelH(-1),
	m_soundDecsionH(-1),
	m_soundSelectH(-1)
{
	m_soundBgmH = LoadSoundMem(kBgmFilename);	  //BGM

	m_soundSelectH = LoadSoundMem(kSelectFilename);	  //�I����
	m_soundDecsionH = LoadSoundMem(kDecisionFilename);	  //���艹
	m_soundCancelH = LoadSoundMem(kCancelFilename);	  //�L�����Z����

	ChangeVolumeSoundMem(64, m_soundBgmH);
	ChangeVolumeSoundMem(128, m_soundSelectH);
	ChangeVolumeSoundMem(128, m_soundDecsionH);
	ChangeVolumeSoundMem(128, m_soundCancelH);

	PlaySoundMem(m_soundBgmH, DX_PLAYTYPE_LOOP);
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_logoH);
	DeleteGraph(m_selectH);
	DeleteGraph(m_startH);
	DeleteGraph(m_optionH);
	DeleteGraph(m_endH);

	DeleteSoundMem(m_soundSelectH);
	DeleteSoundMem(m_soundDecsionH);
	DeleteSoundMem(m_soundCancelH);
	DeleteSoundMem(m_soundBgmH);
}

void SceneTitle::Init()
{
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	//m_pManager->m_pSoundManager.PlayBGM("TitleBgm", true);

	if (!m_isCommand && !m_isOption)
	{
		//��������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{

			if (m_select == kStart)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kGameEnd;
			}
			else if (m_select == kOperation)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kStart;
			}
			else if (m_select == kGameEnd)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kOperation;
			}
		}

		//���������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_select == kStart)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kOperation;
			}
			else if (m_select == kOperation)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kGameEnd;
			}
			else if (m_select == kGameEnd)
			{
				//m_pManager->m_pSoundManager.PlaySE("SelectSe");
				PlaySoundMem(m_soundSelectH, DX_PLAYTYPE_BACK, true);//�I����
				m_select = kStart;
			}
		}

		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_select == kStart)
			{
				//m_pManager->m_pSoundManager.PlaySE("DecisionSe");
				m_isInterval = true;
				m_isCommand = true;
				PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹

			}
			else if (m_select == kOperation)
			{
				//m_pManager->m_pSoundManager.PlaySE("DecisionSe");
				m_isOption = true;
				PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹
			}
			else if (m_select == kGameEnd)
			{
				//m_pManager->m_pSoundManager.PlaySE("DecisionSe");
				bool isClose = true;
				PlaySoundMem(m_soundDecsionH, DX_PLAYTYPE_BACK, true);//���艹
				DxLib_End();
				//m_pManager->m_isClose = true;
			}
		}
	}

	//�I�v�V������ʂ�I��������
	if (m_isOption)
	{
		//B�{�^���Ŗ߂�
		if (Pad::IsTrigger(PAD_INPUT_2)) m_isOption = false;

		////��������������Ƃ�
		//if (Pad::IsTrigger(PAD_INPUT_UP))
		//{

		//	if (m_option == kOperator)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kFullScreen;
		//	}
		//	else if (m_option == kSound)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kOperator;
		//	}
		//	else if (m_option == kFullScreen)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kSound;
		//	}
		//}

		////���������������Ƃ�
		//if (Pad::IsTrigger(PAD_INPUT_DOWN))
		//{
		//	if (m_option == kOperator)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kSound;
		//	}
		//	else if (m_option == kSound)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kFullScreen;
		//	}
		//	else if (m_option == kFullScreen)
		//	{
		//		//m_pManager->m_pSoundManager.PlaySE("SelectSe");
		//		m_option = kOperator;
		//	}
		//}

		////����{�^�����������Ƃ�
		//if (Pad::IsTrigger(PAD_INPUT_1))
		//{
		//	if (m_option == kOperator)
		//	{
		//		m_isOperator = true;
		//		//m_pManager->m_pSoundManager.PlaySE("DecisionSe");

		//	}
		//	else if (m_option == kSound)
		//	{
		//		m_isSound = true;
		//		//m_pManager->m_pSoundManager.PlaySE("DecisionSe");
		//	}
		//	else if (m_option == kFullScreen)
		//	{
		//		m_isFullScreen = true;
		//		//m_pManager->m_pSoundManager.PlaySE("DecisionSe");
		//	}
		//}
		
	}

	//if (Pad::IsTrigger(PAD_INPUT_2))
	//{
	//	//�������
	//	if (m_isOperator)
	//	{
	//		//m_pManager->m_pSoundManager.PlaySE("CancelSe");
	//		PlaySoundMem(m_soundCancelH, DX_PLAYTYPE_BACK, true);//�L�����Z����
	//		m_isOperator = false;
	//	}
	//	//�T�E���h�֌W
	//	else if (m_isSound)
	//	{
	//		//m_pManager->m_pSoundManager.PlaySE("CancelSe");
	//		PlaySoundMem(m_soundCancelH, DX_PLAYTYPE_BACK, true);//�L�����Z����
	//		m_isSound = false;
	//	}
	//	//�t���X�N���[��
	//	else if (m_isFullScreen)
	//	{
	//		//m_pManager->m_pSoundManager.PlaySE("CancelSe");
	//		PlaySoundMem(m_soundCancelH, DX_PLAYTYPE_BACK, true);//�L�����Z����
	//		m_isFullScreen = false;

	//	}
	//}


	


	if (m_isInterval)
	{
		m_frameScene++;
		if (m_select == kStart)
		{
			if (m_frameScene >= kFadeTime) 
			{
				//m_pManager->m_pSoundManager.StopBGM("TitleBgm");
				StopSoundMem(m_soundBgmH);

				return std::make_shared<ScenePlaying>();
			}
		}
	}

	//�Z���N�g�̃A�j���[�V����
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;

	return shared_from_this();
}

void SceneTitle::Draw()
{

#ifdef _DEBUG
	DrawString(0, 0, "Scene Title", 0xffffff, false);
	DrawFormatString(0, 16, 0xffffff, "Select:%d", m_select);
	DrawFormatString(0, 32, 0xffffff, "Option:%d", m_option);
#endif

	//�I�v�V�����t���O��false�̏ꍇ
	if (!m_isOption)	
	{
		//�Z���N�g
		if (m_select == kStart)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
		}
		else if (m_select == kOperation)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
		}
		else if (m_select == kGameEnd)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
		}

		DrawExtendGraph(kLogoLeft, kLogoTop, kLogoRight, kLogoDown, m_logoH, true);	//���S
		DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_startH, true); //�X�^�[�g
		DrawExtendGraph(kOperationLeft, kOptionTop, kOperationRight, kOptionDown, m_optionH, true);//�I�v�V����
		DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//�Q�[���I��
	}
	else	
	{
			// �������ɂ��ă��j���[�w�i�̎l�p
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawFillBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x000000);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �s�����ɖ߂��Ĕ����g
			DrawLineBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x00ffff);

			DrawExtendGraph(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, m_operationH, true);

			////�Z���N�g
			//if (m_option == kOperator)
			//{
			//	DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
			//}
			//else if (m_option == kSound)
			//{
			//	DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
			//}
			//else if (m_option == kFullScreen)
			//{
			//	DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
			//}

			//DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_startH, true); //�X�^�[�g
			//DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//�I�v�V����
			//DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//�Q�[���I��
	}




	//�t�F�[�h�Ö�
	if (m_isInterval)
	{
		int alpha = kBlend * m_frameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	
}

void SceneTitle::End()
{
}

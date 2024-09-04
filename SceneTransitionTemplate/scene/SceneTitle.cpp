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

	constexpr int kSelectLeft = Game::kScreenWidth * 0.3;
	constexpr int kSelectRight = Game::kScreenWidth * 0.4;

	constexpr int kLeft = Game::kScreenWidth * 0.45;
	constexpr int kRight = Game::kScreenWidth * 0.55;

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
	m_optionH(LoadGraph("data/image/Option.png")),
	m_operatorH(LoadGraph("data/image/Operator.png")),
	m_endH(LoadGraph("data/image/End.png"))
{
}

SceneTitle::~SceneTitle()
{
	DeleteGraph(m_logoH);
	DeleteGraph(m_selectH);
	DeleteGraph(m_startH);
	DeleteGraph(m_optionH);
	DeleteGraph(m_endH);

}

void SceneTitle::Init()
{
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	if (!m_isCommand && !m_isOption)
	{
		//��������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{

			if (m_select == kStart)
			{
				m_select = kGameEnd;
			}
			else if (m_select == kOption)
			{
				m_select = kStart;
			}
			else if (m_select == kGameEnd)
			{
				m_select = kOption;
			}
		}

		//���������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_select == kStart)
			{
				m_select = kOption;
			}
			else if (m_select == kOption)
			{
				m_select = kGameEnd;
			}
			else if (m_select == kGameEnd)
			{
				m_select = kStart;
			}
		}

		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_select == kStart)
			{
				m_isInterval = true;
				m_isCommand = true;

			}
			else if (m_select == kOption)
			{
				m_isOption = true;
			}
			else if (m_select == kGameEnd)
			{
				bool isClose = true;
				m_isCommand = true;
			}
		}
	}

	//�I�v�V������ʂ�I��������
	if (m_isOption)
	{
		//B�{�^���Ŗ߂�
		if (Pad::IsTrigger(PAD_INPUT_2)) m_isOption = false;

		//��������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{

			if (m_option == kOperator)
			{
				m_option = kFullScreen;
			}
			else if (m_option == kSound)
			{
				m_option = kOperator;
			}
			else if (m_option == kFullScreen)
			{
				m_option = kSound;
			}
		}

		//���������������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_option == kOperator)
			{
				m_option = kSound;
			}
			else if (m_option == kSound)
			{
				m_option = kFullScreen;
			}
			else if (m_option == kFullScreen)
			{
				m_option = kOperator;
			}
		}

		//����{�^�����������Ƃ�
		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_option == kOperator)
			{
				m_isOperator = true;

			}
			else if (m_option == kSound)
			{
				m_isSound = true;
			}
			else if (m_option == kFullScreen)
			{
				m_isFullScreen = true;
			}
		}
		
	}

	//�������
	if (m_isOperator)
	{
		if (Pad::IsTrigger(PAD_INPUT_2)) m_isOperator = false;
	}
	
	if (m_isSound)
	{
		if (Pad::IsTrigger(PAD_INPUT_2)) m_isSound = false;

	}

	if (m_isFullScreen)
	{
		if (Pad::IsTrigger(PAD_INPUT_2)) m_isFullScreen = false;

	}

	if (m_isInterval)
	{
		m_frameScene++;
		if (m_select == kStart)
		{
			if (m_frameScene >= kFadeTime) {
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
		else if (m_select == kOption)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
		}
		else if (m_select == kGameEnd)
		{
			DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
		}

		DrawExtendGraph(kLogoLeft, kLogoTop, kLogoRight, kLogoDown, m_logoH, true);	//���S
		DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_startH, true); //�X�^�[�g
		DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//�I�v�V����
		DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//�Q�[���I��
	}
	
	if(m_isOption)
	{
			// �������ɂ��ă��j���[�w�i�̎l�p
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
			DrawFillBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x000000);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// �s�����ɖ߂��Ĕ����g
			DrawLineBox(Game::kScreenWidth * 0.1, Game::kScreenHeight * 0.1, Game::kScreenWidth * 0.9, Game::kScreenHeight * 0.9, 0x00ffff);

			//�Z���N�g
			if (m_option == kOperator)
			{
				DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
			}
			else if (m_option == kSound)
			{
				DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
			}
			else if (m_option == kFullScreen)
			{
				DrawExtendGraph(kSelectLeft + m_selectAnimation, kEndTop, kSelectRight + m_selectAnimation, kEndDown, m_selectH, true);
			}

			DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_startH, true); //�X�^�[�g
			DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//�I�v�V����
			DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//�Q�[���I��
	}

	//if (m_isOperator)
	//{
	//	DrawGraph(100, 100, m_operatorH, true); //�X�^�[�g
	//}

	//if (m_isSound)
	//{
	//	DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//�I�v�V����
	//}

	//if (m_isFullScreen)
	//{
	//	DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//�Q�[���I��
	//}


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

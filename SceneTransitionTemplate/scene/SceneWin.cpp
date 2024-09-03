#include "SceneWin.h"
#include "SceneTitle.h"
#include "ScenePlaying.h"

#include "Pad.h"
#include "Game.h"

/// <summary>
/// 定数
/// </summary>
namespace
{
	//定数化
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

SceneWin::SceneWin() :
	m_frameScene(0),
	m_isInterval(false),
	m_isCommand(false),
	m_select(kRetry),
	m_gameclearH(LoadGraph("data/image/Gameclear.png")),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_retryH(LoadGraph("data/image/Retry.png")),
	m_titleH(LoadGraph("data/image/Title.png"))
{

}

SceneWin::~SceneWin()
{
	DeleteGraph(m_selectH);
	DeleteGraph(m_gameclearH);
	DeleteGraph(m_retryH);
	DeleteGraph(m_titleH);
}

void SceneWin::Init()
{
}

std::shared_ptr<SceneBase> SceneWin::Update()
{

	Pad::Update();

	if (!m_isCommand)
	{
		//上方向を押したとき
		if (Pad::IsTrigger(PAD_INPUT_UP))
		{

			if (m_select == kRetry)
			{
				m_select = kTitle;
			}
			else if (m_select == kTitle)
			{
				m_select = kRetry;
			}

		}

		//下方向を押したとき
		if (Pad::IsTrigger(PAD_INPUT_DOWN))
		{
			if (m_select == kRetry)
			{
				m_select = kTitle;
			}
			else if (m_select == kTitle)
			{
				m_select = kRetry;
			}

		}

		if (Pad::IsTrigger(PAD_INPUT_1))
		{
			if (m_select == kRetry)
			{
				m_isInterval = true;
				m_isCommand = true;

			}
			else if (m_select == kTitle)
			{
				m_isInterval = true;
				m_isCommand = true;
			}

		}
	}



	if (m_isInterval)
	{
		m_frameScene++;
		if (m_select == kRetry)
		{
			if (m_frameScene >= kFadeTime)
			{
				return std::make_shared<ScenePlaying>();
			}
		}
		else if (m_select == kTitle)
		{
			if (m_frameScene >= kFadeTime)
			{
				return std::make_shared<SceneTitle>();
			}
		}
	}

	//セレクトのアニメーション
	static float SinCount = 0;
	SinCount += kSelectSpeed;
	m_selectAnimation = sinf(SinCount) * kSelectAnimationSize;

	return shared_from_this();
}

void SceneWin::Draw()
{

	//セレクト
	if (m_select == kRetry)
	{
		DrawExtendGraph(kSelectLeft + m_selectAnimation, kStartTop, kSelectRight + m_selectAnimation, kStartDown, m_selectH, true);
	}
	else if (m_select == kTitle)
	{
		DrawExtendGraph(kSelectLeft + m_selectAnimation, kOptionTop, kSelectRight + m_selectAnimation, kOptionDown, m_selectH, true);
	}

	DrawExtendGraph(kLogoLeft, kLogoTop, kLogoRight, kLogoDown, m_gameclearH, true);	//ロゴ
	DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_retryH, true); //スタート
	DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_titleH, true);//オプション

	//フェード暗幕
	if (m_isInterval)
	{
		int alpha = kBlend * m_frameScene / kFadeTime;
		SetDrawBlendMode(DX_BLENDMODE_MULA, alpha);
		DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, 0x000000, true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#ifdef _DEBUG
	DrawString(0, 0, "Scene Win", 0xffffff, false);
	DrawFormatString(0, 16, 0xffffff, "Select:%d", m_select);
#endif
}

void SceneWin::End()
{
}
#include "SceneTitle.h"
#include "DxLib.h"
#include "Pad.h"
#include "Game.h"

#include "SceneManager.h"
#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

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
}

SceneTitle::SceneTitle() :
	m_frameCount(0),
	m_frameScene(0),
	m_isInterval(false),
	m_isCommand(false),
	m_select(kStart),
	m_selectPosY(0),
	m_logoH(LoadGraph("data/image/TitleLogo.png")),
	m_selectH(LoadGraph("data/image/Select.png")),
	m_startH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Option.png")),
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

	if (!m_isCommand)
	{
		//上方向を押したとき
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

		//下方向を押したとき
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
				m_isCommand = true;
			}
			else if (m_select == kGameEnd)
			{
				m_isCommand = true;
			}
		}
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
		else if (m_select == kOption)
		{
			//オプション
		}

	}

	return shared_from_this();
}

void SceneTitle::Draw()
{

	DrawString(0, 0, "Scene Title", 0xffffff, false);
	DrawFormatString(0, 16, 0xffffff, "Select:%d", m_select);

	//セレクト
	if (m_select == kStart)
	{
		DrawExtendGraph(kSelectLeft, kStartTop, kSelectRight, kStartDown, m_selectH, true);
	}
	else if (m_select == kOption)
	{
		DrawExtendGraph(kSelectLeft, kOptionTop, kSelectRight, kOptionDown, m_selectH, true);
	}
	else if (m_select == kGameEnd)
	{
		DrawExtendGraph(kSelectLeft, kEndTop, kSelectRight, kEndDown, m_selectH, true);
	}



	DrawExtendGraph(kLogoLeft, kLogoTop, kLogoRight, kLogoDown, m_logoH, true);	//ロゴ
	DrawExtendGraph(kLeft, kStartTop, kRight, kStartDown, m_startH, true); //スタート
	DrawExtendGraph(kLeft, kOptionTop, kRight, kOptionDown, m_optionH, true);//オプション
	DrawExtendGraph(kLeft, kEndTop, kRight, kEndDown, m_endH, true);//ゲーム終了


	//フェード暗幕
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

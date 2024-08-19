#include "SceneTitle.h"
#include "DxLib.h"
#include "Pad.h"
#include "Game.h"

#include "ScenePlaying.h"
#include "SceneWin.h"
#include "SceneLose.h"

/// <summary>
/// 定数
/// </summary>
namespace
{
	//定数化
	constexpr int kLogoRight = 300;
	constexpr int kLogoUp = 30;
	constexpr int kLogoLeft = 960;
	constexpr int kLogoDown = 440;

	constexpr int kLeftPosX = 500;
	constexpr int kRightPosX = 700;

	constexpr int kTopStartPosY = 450;
	constexpr int kDownStartPosY = 550;

	constexpr int kTopOptionPosY = 590;
	constexpr int kDownOptionPosY = 690;

	constexpr int kTopEndPosY = 730;
	constexpr int kDownEndPosY = 830;

	constexpr int kLeftSelectPosX = 400;
	constexpr int kRightSelectPosX = 450;

	constexpr int kFadeTime = 120;

	constexpr int kBlend = 255;
}

SceneTitle::SceneTitle():
	m_frameCount(0),
	m_frameScene(0),
	m_isInterval(false),
	m_select(kStart),
	m_logoH(LoadGraph("data/image/GameOver.png")),
	m_startH(LoadGraph("data/image/Start.png")),
	m_optionH(LoadGraph("data/image/Start.png")),
	m_endH(LoadGraph("data/image/Start.png"))
{
}

SceneTitle::~SceneTitle()
{

}

void SceneTitle::Init()
{
}

std::shared_ptr<SceneBase> SceneTitle::Update()
{
	Pad::Update();

	if (Pad::IsTrigger(PAD_INPUT_1))
	{
		m_isInterval = true;
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
		else if (m_select == kGameEnd)
		{
			if (m_frameScene >= kFadeTime) {
				DxLib_End();				// ＤＸライブラリ使用の終了処理
			}
		}
	}

	return shared_from_this();
}

void SceneTitle::Draw()
{

	DrawString(0, 0, "Scene Title", 0xffffff, false);

	
	DrawExtendGraph(kLogoRight, kLogoUp, kLogoLeft, kLogoDown, m_logoH, true);	//ロゴ
	DrawExtendGraph(kLeftPosX, kTopStartPosY, kRightPosX,kDownStartPosY, m_startH, true); //スタート
	DrawExtendGraph(kLeftPosX, kTopOptionPosY, kRightPosX, kDownOptionPosY, m_endH, true);//オプション
	DrawExtendGraph(kLeftPosX, kTopEndPosY, kRightPosX, kDownEndPosY, m_endH, true);//ゲーム終了
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

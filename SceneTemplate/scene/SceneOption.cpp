#include "DxLib.h"

#include "SceneOption.h"
#include "SceneManager.h"

#include "util/Pad.h"


SceneOption::SceneOption(SceneManager& manager) :
	SceneBase(manager),
	m_bgmScale(0),
	m_seScale(0),
	m_sensitivityScale(0),
	m_isFullScreen(false)
{
	FadeInSkip();

}

SceneOption::~SceneOption()
{
}

void SceneOption::Update()
{
	UpdateFade();

}

void SceneOption::Draw()
{
#ifdef _DEBUG
	DrawFormatString(0, 0, 0xffffff, "Scene Option");
#endif

	DrawFade();
}

void SceneOption::BgmUpdate()
{
	Pad::Update();
	
	if (Pad::IsTrigger(PAD_INPUT_DOWN))
	{

	}

	if (Pad::IsTrigger(PAD_INPUT_RIGHT))
	{

	}

	if (Pad::IsTrigger(PAD_INPUT_LEFT))
	{

	}

}

void SceneOption::SeUpdate()
{
}

void SceneOption::SensitivityUpdate()
{
}

void SceneOption::FullScreenUpdate()
{
}

#include "SceneBase.h"
#include "util/Game.h"

namespace
{
	//フェード速度
	constexpr int kFadeSpeed = 5;

	constexpr int kFadeColor = 0xffffff;
}


SceneBase::SceneBase(SceneManager& manager):
	m_pManager(manager)
{
	//初期設定はフェードアウト状態
	m_fadeColor = 0x000000;
	m_fadeBright = 255;
	m_fadeSpeed = -kFadeSpeed;
	m_isToNextScene = false;
	m_isFadeColor = false;
}

void SceneBase::UpdateFade()
{

	m_fadeBright += m_fadeSpeed;

	//
	if (m_fadeBright >= 255)
	{
		m_fadeBright = 255;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}

	//
	if (m_fadeBright <= 0)
	{
		m_fadeBright = 0;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}

	}
}

void SceneBase::DrawFade(int fadeColor) const
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

//void SceneBase::DrawWhiteFade() const
//{
//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, kFadeColor, true);
//	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}

void SceneBase::StartFadeIn()
{
	m_fadeSpeed = -kFadeSpeed;
}

bool SceneBase::IsFadingIn() const
{
	if (m_fadeSpeed < 0) return true;
	return false;
}

bool SceneBase::IsFadingOut() const
{
	if (m_fadeSpeed > 0) return true;
	return false;
}

bool SceneBase::IsFinishFadeOut() const
{
	if (m_fadeBright == 255 && m_fadeSpeed == 0)
	{
		return true;
	}

	return false;
}

void SceneBase::FadeInSkip()
{
	m_fadeBright = 0;
	m_fadeSpeed = -kFadeSpeed;
}

void SceneBase::FadeOutSkip()
{
	m_fadeBright = 255;
	m_fadeSpeed = kFadeSpeed;
}

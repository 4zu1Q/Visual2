#include "SceneBase.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	//フェード速度
	constexpr int kFadeSpeed = 8;
}

void SceneBase::Init()
{
	// 初期設定はフェードアウト状態
	m_fadeColor = GetColor(0, 0, 0);
	m_fadeBright = 255;
	m_fadeSpeed = -kFadeSpeed;
}

std::shared_ptr<SceneBase> SceneBase::Update()
{
	return shared_from_this();
}

void SceneBase::Draw()
{

}

void SceneBase::UpdateFade()
{
	m_fadeBright += m_fadeSpeed;
	if (m_fadeBright >= 255)
	{
		m_fadeBright = 255;
		if (m_fadeSpeed > 0)
		{
			m_fadeSpeed = 0;
		}
	}
	if (m_fadeBright <= 0)
	{
		m_fadeBright = 0;
		if (m_fadeSpeed < 0)
		{
			m_fadeSpeed = 0;
		}
	}
}

void SceneBase::DrawFade()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeBright);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, m_fadeColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void SceneBase::SkipFadeIn()
{
	m_fadeBright = 0;
	m_fadeSpeed = kFadeSpeed;
}

void SceneBase::SkipFadeOut()
{
	m_fadeBright = 255;
	m_fadeSpeed = -kFadeSpeed;
}

bool SceneBase::IsFadeingIn() const
{
	if (m_fadeSpeed < 0)	return true;
	return false;
}

bool SceneBase::IsFadeingOut() const
{
	if (m_fadeSpeed > 0)	return true;
	return false;
}

void SceneBase::StartFadeOut()
{
	m_fadeSpeed = kFadeSpeed;
}
